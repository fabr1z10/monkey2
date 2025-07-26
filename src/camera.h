#pragma once

#include <glm/glm.hpp>
#include "vec.h"
#include <array>
#include <pybind11/pybind11.h>
#include "shader.h"


namespace py = pybind11;

class Camera {
public:
    Camera(Vec4 viewport = Vec4());
	virtual ~Camera() =default;
    void setPosition(Vec3 eye, Vec3 direction, Vec3 up = Vec3(0, 1, 0));
	void move(Vec3);
	void setFwd(glm::vec3);
	void setBounds(float xMin, float xMax, 
		float yMin, float yMax, 
		float zMin, float zMax);

	const glm::mat4& getViewMatrix() const;
	const glm::mat4& getProjectionMatrix() const;

	glm::vec4 getViewport() const;
	glm::vec2 getPosition() const;

	glm::vec3 getFwd() const;
    void setPos(glm::vec3 eye, glm::vec3 dir, glm::vec3 up);

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
	OrthoCamera(float width, float height);

    OrthoCamera(float width, float height, Vec4 viewport);

	OrthoCamera(float width, float height, Vec4 viewport, float zNear, float zFar);

	glm::vec2 getSize() const;

    /* Transform device coordinates into world coordinates
     */
    [[nodiscard]] glm::vec2 getWorldCoordinates(glm::vec2) const;
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
	PerspectiveCamera();
    explicit PerspectiveCamera(Vec4 viewport, float fov, float near, float far);
private:
	float _fov;
	float _near;
	float _far;
	float _aspectRatio;
};
