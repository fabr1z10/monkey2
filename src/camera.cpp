#include "camera.h"
#include "game.h"
#include <glm/gtc/matrix_transform.hpp>
#include "pyhelper.h"

Camera::Camera(glm::vec4 viewport) {

	if (viewport == glm::vec4(0.f)) {
		auto deviceSize = Game::instance().getDeviceSize();
		viewport = glm::vec4(0.f, 0.f, deviceSize.x, deviceSize.y);
	}
	_viewport = viewport;
	auto t = std::numeric_limits<float>::infinity();

	_xBounds = glm::vec2(-t, t);
	_yBounds = _xBounds;
	_zBounds = _xBounds;
}

void Camera::setPosition(glm::vec3 eye, glm::vec3 dir, glm::vec3 up) {
	_fwd = dir;
	_up = up;
	_eye = eye;
	_eye.x = std::clamp(_eye.x, _xBounds[0], _xBounds[1]);
	_eye.y = std::clamp(_eye.y, _yBounds[0], _yBounds[1]);
	_eye.z = std::clamp(_eye.z, _zBounds[0], _zBounds[1]);
	_viewMatrix = glm::lookAt(_eye, _eye + dir, up);
}

void Camera::setFwd(glm::vec3 dir) {
    _fwd = dir;
    _viewMatrix = glm::lookAt(_eye, _eye+_fwd, _up);
}

glm::vec3 Camera::getFwd() const {
    return _fwd;
}



void Camera::move(glm::vec3 delta) {
	_eye += delta;
	_eye.x = std::clamp(_eye.x, _xBounds[0], _xBounds[1]);
	_eye.y = std::clamp(_eye.y, _yBounds[0], _yBounds[1]);
	_eye.z = std::clamp(_eye.z, _zBounds[0], _zBounds[1]);
	_viewMatrix = glm::lookAt(_eye, _eye + _fwd, _up);
}


OrthoCamera::OrthoCamera(float width, float height, glm::vec4 viewport) : Camera(viewport), _orthoWidth(width), _orthoHeight(height) {
	float hw = _orthoWidth / 2.0f;
	float hh = _orthoHeight / 2.0f;
	_projectionMatrix = glm::ortho(-hw, hw, -hh, hh, -100.0f, 100.0f);
	_halfSize = glm::vec3(hw, hh, 100.f);
}

PerspectiveCamera::PerspectiveCamera(glm::vec4 viewport, float fov, float near, float far) : Camera(viewport), _fov(fov), _near(near), _far(far) {
	_projectionMatrix = glm::perspective (_fov, _viewport[2]/_viewport[3], _near, _far);
}
