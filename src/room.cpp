#include "room.h"
#include <list>
#include <algorithm>
#include "glsl/glsl.h"
#include "game.h"
#include "renderer.h"
#include "assetmanager.h"


Room::Room() : _clearColor(glm::vec3(0.f)) {
	_root = std::make_shared<Node>();

	_root->setInRoom(true);

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


		if (current->active()) {
			// if node is not active, then we don't need to update
			// it and also we don't care about updating its children
			current->update(dt);
			std::transform(current->getChildren().begin(), current->getChildren().end(), std::back_inserter(li),
						   [](const std::shared_ptr<Node> &ptr) { return ptr.get(); });
		}


	}

	if (_collisionEngine) {
		_collisionEngine->checkCollisions();
	}

}

void Room::draw() {
	// render on the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, _fb);
	glEnable(GL_DEPTH_TEST);
	glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto deviceSize = Game::instance().getDeviceSize();
	for (size_t i = 0; i< _shaders.size(); ++i) {
		_shaders[i]->use();
        glViewport(0,0,deviceSize.x, deviceSize.y);
		if (_batches[i].size() > 0) {
			for (const auto &b: _batches[i]) {
				b->setupUniforms(_shaders[i]);
				b->draw();
			}
		} else {

			// disable this if batch only mode
			std::list<Node *> li{_root.get()};

			while (!li.empty()) {
				auto current = li.front();
				li.pop_front();
				auto model = current->getModel();
				if (model != nullptr && model->getShaderId() == _shaders[i]->getId()) {
					model->draw();
				}

				//    auto* r = current->getRenderer();
				//    if (r != nullptr) r->draw(_shaders[i]);

				std::transform(current->getChildren().begin(), current->getChildren().end(), std::back_inserter(li),
							   [](const std::shared_ptr<Node> &ptr) { return ptr.get(); });

			}
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
	if (shaderType == -1) {
		GLIB_FAIL("Batch doesn't define a shader type!");
	}
    _refBatch[b->getId()] = b.get();

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
    return _refBatch.at(id);
}

Camera* Room::getCamera(int id) {
	return _cameras[id].get();
}

void Room::end() {
	_root->dispose();
	_root = nullptr;
	_hotSpotManager = nullptr;
	std::cout << "room ended.\n";
}


void Room::start() {
	setup();

    if (_startUpFunction) {
        _startUpFunction();
    }

    //AssetManager::instance().startUp();


	for (const auto& b : _refBatch) {
        b.second->start();
        b.second->configure();
		std::cerr << "started batch " << b.first << "\n";
	}


    //for (size_t i = 0; i< _refBatch.size(); ++i) {
    //    _refBatch[i]->configure();
    //}
	_root->start();
	std::cerr << "aa\n";



}


void Room::setClearColor(Vec3 color) {
    _clearColor = color.toGlm();
}

void Room::setCollisionEngine(std::shared_ptr<ICollisionEngine> e) {
    _collisionEngine = e;
}

ICollisionEngine* Room::getCollisionEngine() {
    return _collisionEngine.get();
}

void Room::setHotSpotManager(Node* e) {
    _hotSpotManager = e;
}

Node* Room::getHotSpotManager() {
    return _hotSpotManager;
}
