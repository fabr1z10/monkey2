#pragma once


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "room.h"
#include "keylistener.h"
#include <pybind11/pybind11.h>

class Game {
public:
	Game();
	~Game();
	static Game &instance() {
		static Game instance; // Guaranteed to be destroyed.
		// Instantiated on first use.
		return instance;
	}

	[[nodiscard]] glm::ivec2 getDeviceSize() const;

	[[nodiscard]] glm::ivec2 getWindowSize() const;

	[[nodiscard]] double getDeviceAspectRatio() const;

	void start();

	void run();

	static void WindowResizeCallback(GLFWwindow* win, int width, int height);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_pos_callback(GLFWwindow*, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow*, int, int, int);

	void registerToKeyboardEvent(KeyboardListener*);
    void unregisterToKeyboardEvent(KeyboardListener*);

	glm::vec4 getWindowViewport() const;

	Shader* getShader(int shaderId);

	Room* getRoom();
private:
	void loadRoom();
	void initGL();
	pybind11::module_ _settings;
	pybind11::module_ _source;
	glm::vec4 _windowViewport;

	glm::ivec2 _windowSize;
	glm::ivec2 _deviceSize;
	std::string _title;
	double _deviceAspectRatio;
	bool _enableMouse;
	bool _shutdown;
	double _frameTime;
	double _timeLastUpdate;
	std::shared_ptr<Room> _room;
	bool _run;
	std::unique_ptr<ShaderStore> _shaderStore;
    std::unordered_set<KeyboardListener*> _keyboardListeners;

};

inline glm::ivec2 Game::getDeviceSize() const {
	return _deviceSize;
}

inline glm::ivec2 Game::getWindowSize() const {
	return _windowSize;
}

inline double Game::getDeviceAspectRatio() const {
	return _deviceAspectRatio;
}

inline glm::vec4 Game::getWindowViewport() const {
	return _windowViewport;
}