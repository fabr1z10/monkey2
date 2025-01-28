#include "room.h"
#include <list>
#include <list>
#include <algorithm>
#include "glsl/glsl.h"
#include "game.h"

Room::Room() {
	_root = std::make_shared<Node>();

	_blitShader = std::make_shared<Shader>(0, blit_vs, blit_fs, "2f2f");

	auto deviceSize = Game::instance().getDeviceSize();

	float quadVertices[] = {
			// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			1.0f, -1.0f,  1.0f, 0.0f,
			1.0f,  1.0f,  1.0f, 1.0f
	};
	// screen quad VAO
	glGenVertexArrays(1, &_quadVAO);
	glGenBuffers(1, &_quadVBO);
	glBindVertexArray(_quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// create frame buffer
	glGenFramebuffers(1, &_fb);
	glBindFramebuffer(GL_FRAMEBUFFER, _fb);
	glGenTextures(1, &_color);
	glBindTexture(GL_TEXTURE_2D, _color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, deviceSize.x, deviceSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _color, 0);

	// We also want to make sure OpenGL is able to do depth testing (and optionally stencil testing) so we have to make
	// sure to add a depth (and stencil) attachment to the framebuffer. Since we'll only be sampling the color buffer
	// and not the other buffers we can create a renderbuffer object for this purpose.

	// Creating a renderbuffer object isn't too hard. The only thing we have to remember is that we're creating it as a
	// depth and stencil attachment renderbuffer object. We set its internal format to GL_DEPTH24_STENCIL8 which
	// is enough precision for our purposes:

	glGenTextures(1, &_depth);
	glBindTexture(GL_TEXTURE_2D, _depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, deviceSize.x, deviceSize.y, 0,  GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Room::addCamera(std::shared_ptr<Camera> cam) {
	_cameras.push_back(cam);
}

void Room::update(double dt) {
	int nUpdates{0};


	// update children recursively
	std::list<Node*> li{_root.get()};

	while (!li.empty()) {
		auto current = li.front();
		li.pop_front();

		current->update(dt);

		std::transform(current->getChildren().begin(), current->getChildren().end(), std::back_inserter(li),
				 [](const std::shared_ptr<Node>& ptr) { return ptr.get(); });

	}

}

void Room::draw() {
	// render on the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, _fb);
	glEnable(GL_DEPTH_TEST);
	//glClearColor(0.f, 0.4f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (size_t i = 0; i< _shaders.size(); ++i) {
		_shaders[i]->use();
		for (const auto& b : _batches[i]) {
			b->setupUniforms(_shaders[i]);
			b->draw();
		}


	}

	auto vp = Game::instance().getWindowViewport();
	//std::vector<float> ccc(256*240);
	//glReadPixels(0,0,256,240, GL_DEPTH_COMPONENT, GL_FLOAT, &ccc[0]);
	// frame buffer rendering start
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glViewport(vp.x, vp.y, vp.z, vp.w);
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_blitShader->use();
	glBindVertexArray(_quadVAO);
	_blitShader->setInt("screenTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _color);	// use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Room::addBatch(std::shared_ptr<IBatch> b) {
	auto shaderType = b->getShaderType();
	_refBatch.push_back(b.get());

	// check if we already have this shader - otherwise add it!
	int j = -1;
	for (size_t i = 0; i < _shaders.size(); ++i) {
		if (_shaders[i]->getId() == shaderType) {
			j = i;
			break;
		}
	}
	if (j != -1) {
		_batches[j].push_back(b);
	} else {
		auto* s = Game::instance().getShader(shaderType);
		_shaders.push_back(s);
		_batches.push_back({b});
	}
	// maybe we should store the shaders in the game

	// then add the batch to the batch set of this shader


}

void Room::initialize() {

	for (auto& s : _batches) {
		for (auto& b : s) {
			b->configure();
		}
	}
}

std::shared_ptr<Node> Room::getRoot() {
	return _root;
}

IBatch* Room::getBatch(int id) {
	return _refBatch[id];
}

Camera* Room::getCamera(int id) {
	return _cameras[id].get();
}

void Room::start() {
	for (const auto& b : _refBatch) {
		b->start();
	}


    for (size_t i = 0; i< _refBatch.size(); ++i) {
        _refBatch[i]->configure();
    }
	std::list<Node*> li{_root.get()};

	while (!li.empty()) {
		auto current = li.front();
		li.pop_front();

		current->start();

		std::transform(current->getChildren().begin(), current->getChildren().end(), std::back_inserter(li),
					   [](const std::shared_ptr<Node>& ptr) { return ptr.get(); });

	}




}