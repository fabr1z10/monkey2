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

	virtual ~Room() = default;

	void initialize();

	void update(double dt);

	void addBatch(std::shared_ptr<IBatch>);

	void draw();

	void addCamera(std::shared_ptr<Camera>);

	std::shared_ptr<Node> getRoot();

	IBatch* getBatch(int);

	virtual void setup() {}

	void start();

	void end();

	Camera* getCamera(int camId);

	int getCameraCount() const;

	void setClearColor(Vec3);

    void addShader(Shader*);

    void setStartUpFunction(pybind11::function);

    void setCollisionEngine(std::shared_ptr<ICollisionEngine>);

    ICollisionEngine* getCollisionEngine();

    void setHotSpotManager(Node*);

    Node* getHotSpotManager();
private:
	std::shared_ptr<Node> _root;
	std::vector<Shader*> _shaders;
	std::vector<std::vector<std::shared_ptr<IBatch>>> _batches;
	std::vector<std::shared_ptr<Camera>> _cameras;
    std::unordered_map<int, IBatch*> _refBatch;
	std::shared_ptr<Shader> _blitShader;
	// frambuffer stuff
	GLuint _fb, _color, _depth;
	unsigned int _quadVAO, _quadVBO;
	glm::vec3 _clearColor;
    pybind11::function _startUpFunction;
    std::shared_ptr<ICollisionEngine> _collisionEngine;
    Node* _hotSpotManager;

};

inline void Room::addShader(Shader * shader) {
    _shaders.push_back(shader);
    _batches.push_back({});
}

inline void Room::setStartUpFunction(pybind11::function f) {
    _startUpFunction = f;
}

inline int Room::getCameraCount() const {
	return _cameras.size();
}
