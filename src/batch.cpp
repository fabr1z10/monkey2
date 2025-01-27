#include "batch.h"
#include "game.h"
#include "room.h"

#include <glm/gtc/type_ptr.hpp>

IBatch::IBatch(int size, int camId) : _size(size), _camId(camId), _nPrimitive(0) {
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

void IBatch::addLight(std::shared_ptr<Light> light) {
    _lights.push_back(light);
}


void IBatch::release(int id) {
	_deallocated.push_back(id);
}

void IBatch::setupUniforms(Shader* s) {
	// setup viewport and projection-view matrix
	auto vp = _cam->getViewport();
	glViewport(vp.x, vp.y, vp.z, vp.w);
	int jointMatrixLoc = glGetUniformLocation(s->getProgId(), "pv_mat");
	auto pvMatrix =  _cam->getProjectionMatrix() * _cam->getViewMatrix();
	glUniformMatrix4fv(jointMatrixLoc, 1, GL_FALSE, glm::value_ptr(pvMatrix[0]));

	for (auto& light : _lights) {
	    light->setup(s);
	}
}