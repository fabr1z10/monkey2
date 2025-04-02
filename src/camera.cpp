#include "camera.h"
#include "game.h"
#include <glm/gtc/matrix_transform.hpp>


Camera::Camera(Vec4 viewport) {

    if (viewport.isZero()) {
		auto deviceSize = Game::instance().getDeviceSize();
        _viewport = glm::vec4(0.f, 0.f, deviceSize.x, deviceSize.y);
    } else {
        _viewport = viewport.toGlm();
    }
	auto t = std::numeric_limits<float>::infinity();

	_xBounds = glm::vec2(-t, t);
	_yBounds = _xBounds;
	_zBounds = _xBounds;
}

void Camera::setBounds(float xm, float xM, float ym, float yM, float zm, float zM) {
    _xBounds = { xm, xM };
    _yBounds = { ym, yM };
    _zBounds = { zm, zM };
    setPosition(_eye, _fwd, _up);

}

void Camera::setPosition(Vec3 eye, Vec3 dir, Vec3 up) {
    _fwd = dir.toGlm();
    _up = up.toGlm();
    _eye = eye.toGlm();
	_eye.x = std::clamp(_eye.x, _xBounds[0], _xBounds[1]);
	_eye.y = std::clamp(_eye.y, _yBounds[0], _yBounds[1]);
	_eye.z = std::clamp(_eye.z, _zBounds[0], _zBounds[1]);
    _viewMatrix = glm::lookAt(_eye, _eye + _fwd, _up);
}


void Camera::setPos(glm::vec3 eye, glm::vec3 dir, glm::vec3 up) {
    _eye = eye;
    _fwd = dir;
    _up = up;

    _eye.x = std::clamp(_eye.x, _xBounds[0], _xBounds[1]);
    _eye.y = std::clamp(_eye.y, _yBounds[0], _yBounds[1]);
    _eye.z = std::clamp(_eye.z, _zBounds[0], _zBounds[1]);
    _viewMatrix = glm::lookAt(_eye, _eye + _fwd, _up);
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


OrthoCamera::OrthoCamera(float width, float height, Vec4 viewport) : Camera(viewport), _orthoWidth(width), _orthoHeight(height) {
	float hw = _orthoWidth / 2.0f;
	float hh = _orthoHeight / 2.0f;
	_projectionMatrix = glm::ortho(-hw, hw, -hh, hh, -100.0f, 100.0f);
	_halfSize = glm::vec3(hw, hh, 100.f);
}

glm::vec2 OrthoCamera::getWorldCoordinates(glm::vec2 deviceCoordinates) const {
    float xw = -_viewMatrix[3][0] - (_orthoWidth * 0.5f) + (deviceCoordinates.x - _viewport.x) * (_orthoWidth / _viewport[2]);
    float yw = -_viewMatrix[3][1] - (_orthoHeight * 0.5f) + (deviceCoordinates.y - _viewport.y) * (_orthoHeight / _viewport[3]);
    return glm::vec2(xw, yw);
}


PerspectiveCamera::PerspectiveCamera(Vec4 viewport, float fov, float near, float far) : Camera(viewport), _fov(fov), _near(near), _far(far) {
	_projectionMatrix = glm::perspective (_fov, _viewport[2]/_viewport[3], _near, _far);
}
