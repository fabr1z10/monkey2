#pragma once

#include <memory>
#include "node.h"
#include "shader.h"
#include "camera.h"
#include "batch.h"
#include "collisionengine.h"

class Room {
public:
	Room();

	void initialize();

	void update(double dt);

	void addBatch(std::shared_ptr<IBatch>);

	void draw();

	void addCamera(std::shared_ptr<Camera>);

	std::shared_ptr<Node> getRoot();

	IBatch* getBatch(int);

	void start();

	Camera* getCamera(int camId);

	void setClearColor(glm::vec3);

    void addShader(Shader*);

    void setStartUpFunction(pybind11::function);

    void setCollisionEngine(std::shared_ptr<CollisionEngine>);

    CollisionEngine* getCollisionEngine();
private:
	std::shared_ptr<Node> _root;
	std::vector<Shader*> _shaders;
	std::vector<std::vector<std::shared_ptr<IBatch>>> _batches;
	std::vector<std::shared_ptr<Camera>> _cameras;
	std::vector<IBatch*> _refBatch;
	std::shared_ptr<Shader> _blitShader;
	// frambuffer stuff
	GLuint _fb, _color, _depth;
	unsigned int _quadVAO, _quadVBO;
	glm::vec3 _clearColor;
    pybind11::function _startUpFunction;
    std::shared_ptr<CollisionEngine> _collisionEngine;
};

inline void Room::addShader(Shader * shader) {
    _shaders.push_back(shader);
    _batches.push_back({});
}

inline void Room::setStartUpFunction(pybind11::function f) {
    _startUpFunction = f;
}