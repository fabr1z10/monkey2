#pragma once


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "room.h"
#include "keylistener.h"
#include "mouselistener.h"
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

    [[nodiscard]] IVec2 getDeviceSize() const;

    [[nodiscard]] IVec2 getWindowSize() const;

	[[nodiscard]] double getDeviceAspectRatio() const;

	void start();

	void run();

	void closeRoom();

	static void WindowResizeCallback(GLFWwindow* win, int width, int height);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_pos_callback(GLFWwindow*, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow*, int, int, int);

	void registerToKeyboardEvent(KeyboardListener*);
    void unregisterToKeyboardEvent(KeyboardListener*);
    void registerToMouseEvent(MouseListener*);
    void unregisterToMouseEvent(MouseListener*);

	glm::vec4 getWindowViewport() const;

	Shader* getShader(int shaderId);
    bool hasShader(int);

	Room* getRoom();

    // transforms screen coordinate into device coordinates
    glm::vec2 getDeviceCoordinates(glm::vec2);

    void makeCurrent(std::shared_ptr<Room>);

    bool started() const;

    std::string getWorkingDirectory() const;
private:
    std::string getPythonScriptDirectory();
	void loadRoom();
	void initGL();
	pybind11::module_ _settings;
	pybind11::module_ _source;
	glm::vec4 _windowViewport;

    IVec2 _windowSize;
    IVec2 _deviceSize;
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
    std::unordered_set<MouseListener*> _mouseListeners;
    float _screenHeight;
    std::string _cwd;
};

inline IVec2 Game::getDeviceSize() const {
	return _deviceSize;
}

inline IVec2 Game::getWindowSize() const {
	return _windowSize;
}

inline double Game::getDeviceAspectRatio() const {
	return _deviceAspectRatio;
}

inline glm::vec4 Game::getWindowViewport() const {
	return _windowViewport;
}

inline bool Game::hasShader(int id) {
    return _shaderStore->hasShader(id);
}
inline void Game::makeCurrent(std::shared_ptr<Room> room) {
    _room = room;
}

inline bool Game::started() const {
    return _run;
}

inline std::string Game::getWorkingDirectory() const {
    return _cwd;
}
