#pragma once

#include <glm/glm.hpp>
#include <array>
#include <pybind11/pybind11.h>
#include "shader.h"


namespace py = pybind11;

class Camera {
public:
	Camera(glm::vec4 viewport = glm::vec4(0.f));
	~Camera() =default;
	void setPosition(glm::vec3 eye, glm::vec3 direction, glm::vec3 up = glm::vec3(0, 1, 0));
	void move(glm::vec3);
	void setFwd(glm::vec3);
	//void setBounds(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);

	const glm::mat4& getViewMatrix() const;
	const glm::mat4& getProjectionMatrix() const;

	glm::vec4 getViewport() const;
	glm::vec2 getPosition() const;

	glm::vec3 getFwd() const;

protected:
	glm::vec3 _fwd;
	glm::vec3 _up;
	glm::vec3 _eye;
	glm::vec2 _xBounds;
	glm::vec2 _yBounds;
	glm::vec2 _zBounds;

	glm::mat4 _projectionMatrix;
	glm::mat4 _viewMatrix;
	glm::vec4 _viewport;
	glm::vec4 _screenViewport;

};

inline glm::vec4 Camera::getViewport() const {
	return _viewport;
}

inline glm::vec2 Camera::getPosition() const {
	return _eye;
}

inline const glm::mat4 & Camera::getViewMatrix() const {
	return _viewMatrix;
}

inline const glm::mat4 & Camera::getProjectionMatrix() const {
	return _projectionMatrix;
}


class OrthoCamera : public Camera {
public:
	OrthoCamera(float width, float height, glm::vec4 viewport = glm::vec4(0.f));
	glm::vec2 getSize() const;
private:
	float _orthoWidth;
	float _orthoHeight;
	glm::vec3 _halfSize;
};

inline glm::vec2 OrthoCamera::getSize() const {
	return glm::vec2(_orthoWidth, _orthoHeight);
}


class PerspectiveCamera : public Camera {
public:
	explicit PerspectiveCamera(glm::vec4 viewport = glm::vec4(0.f), float fov = 45, float near = 0.1f, float far = 100.f);
private:
	float _fov;
	float _near;
	float _far;
	float _aspectRatio;
};