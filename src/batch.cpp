#include "batch.h"
#include "game.h"
#include "room.h"

#include <glm/gtc/type_ptr.hpp>

int IBatch::_gId = 0;

IBatch::IBatch(int shaderType, int camId) : _camId(camId), _nPrimitive(0), _id(IBatch::_gId++),
	_texWidth(0), _texHeight(0), _invWidth(0.f), _invHeight(0.f), _shaderType(shaderType) {
}

void IBatch::start() {
	_cam = Game::instance().getRoom()->getCamera(_camId);
}

int IBatch::getPrimitiveId() {
	int next;
	if (_deallocated.empty()) {
		next = _nPrimitive;
		_nPrimitive++;
	} else {
		next = _deallocated.front();
		_deallocated.pop_front();
	}
	return next;
}

//void IBatch::addLight(std::shared_ptr<Light> light) {
//    _lights.push_back(light);
//}


void IBatch::release(int id) {
	_deallocated.push_back(id);
	std::cout << " now deallocated has size " << _deallocated.size() << "\n";
}

glm::vec4 IBatch::pixelToTextureCoords(glm::ivec4 px) {
	return glm::vec4(px[0] * _invWidth, px[1] * _invHeight, px[2] * _invWidth, px[3] * _invHeight);
}

void IBatch::setupUniforms(Shader* s) {
	// setup viewport and projection-view matrix
	auto vp = _cam->getViewport();
	glViewport(vp.x, vp.y, vp.z, vp.w);
	int jointMatrixLoc = glGetUniformLocation(s->getProgId(), "pv_mat");
	auto pvMatrix =  _cam->getProjectionMatrix() * _cam->getViewMatrix();
	glUniformMatrix4fv(jointMatrixLoc, 1, GL_FALSE, glm::value_ptr(pvMatrix[0]));

//	for (auto& light : _lights) {
//	    light->setup(s);
//	}
}
