#include "game.h"
#include "error.h"
#include "pyhelper.h"
#include "util.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;
namespace py = pybind11;

GLFWwindow* window;

Game::Game() : _run(false) {
	_frameTime = 1.0 / 60.0;
	_shaderStore = std::make_unique<ShaderStore>();

}

Game::~Game() {
	_settings.release();
	_source.release();
}

void Game::start() {
	_settings = py::module_::import("settings");
	_source = py::module_::import("src");

	_deviceSize = py_get<glm::ivec2>(_settings, "device_size");
	_windowSize = py_get<glm::ivec2>(_settings, "window_size", _deviceSize);
	_enableMouse = py_get<bool>(_settings, "enable_mouse", false);
	_deviceAspectRatio = static_cast<double>(_deviceSize[0]) / _deviceSize[1];
    //fs::path fl = _mainModule.attr("__file__").cast<std::string>();

    _cwd = getPythonScriptDirectory();
    // auto pippo = py::module_::import("__main__").attr("__file__").cast<std::string>();
    std::cout << " -- cwd: " << _cwd << "\n";
	std::cout << " -- device size: " << _deviceSize << std::endl;
    // If provided, call init function. Here you can initialize assets, or any
    // thing you need for the game.
    if (pybind11::hasattr(_source, "init")) {
        _source.attr("init")();
    }
	initGL();
}

void Game::initGL() {
	// Initialise GLFW
	if(!glfwInit())
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		exit(1);
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( _windowSize[0], _windowSize[1], _title.c_str(), NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		exit(1);
	}
	glfwMakeContextCurrent(window);
	// note: we are setting a callback for the frame buffer resize event,
	// so the dimensions we will get will be in pixels and NOT screen coordinates!
	glfwSetFramebufferSizeCallback(window, Game::WindowResizeCallback);

    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* vendor = glGetString(GL_VENDOR);

    std::cout << "OpenGL Version: " << version << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "Vendor: " << vendor << std::endl;
    GLint maxTextureArrayLayers;
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxTextureArrayLayers);
    std::cout << "Max texture array layers: " << maxTextureArrayLayers << std::endl;
	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		exit(1);
	}
    // Get extensions
//    GLint numExtensions = 0;
//    glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
//    std::cout << "#ext: " << numExtensions << "\n";
//    for (int i = 0; i < numExtensions; i++) {
//        std::cout << glGetStringi(GL_EXTENSIONS, i) << std::endl;  // Correct way for OpenGL 3.0+
//    }
	// Ensure we can capture the escape key being pressed below
	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	Game::WindowResizeCallback(window, _windowSize[0], _windowSize[1]);
	glfwSetKeyCallback(window, key_callback );
	if (_enableMouse) {
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetCursorPosCallback(window, cursor_pos_callback);

	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_POINT_SMOOTH);
	// setupFramebufferRendering();
//    if (useFrameBuffer) {
	//_engineDraw = std::make_unique<FrameBufferEngineDraw>();
//    } else {
//        _engineDraw = std::make_unique<BasicEngineDraw>();
//    }
	// Dark blue background
	//loadShaders();
}

void Game::loadRoom() {
	if (!pybind11::hasattr(_source, "create_room")) {
		GLIB_FAIL("no function create_room found!");
	}
	auto roomBuilder = _source.attr("create_room");

	_room = roomBuilder().cast<std::shared_ptr<Room>>();
	//_engineDraw->init(_room.get());

}

void Game::run() {

	std::cout << " -- engine starts running..\n";
	_shutdown = false;
	// main loop
	while (!_shutdown) {
//		m_scheduledForRemoval.clear();
		loadRoom();
//
		_run = true;
		_room->start();

		do {
			double currentTime = glfwGetTime();
			/// note: if I run the update only every frame time CPU goes to 100%. If I run it on
			/// every iter, it doesn't. Tried move the glfwSwapBuffers call (and successive) out of the loop
			/// and that seems to work.
			if (true || currentTime - _timeLastUpdate >= _frameTime) {
				double dt = currentTime - _timeLastUpdate;
				_timeLastUpdate = currentTime;

				// remove all entities scheduled for removal
//				if (!m_scheduledForRemoval.empty()) {
//					for (auto & g : m_scheduledForRemoval) {
//						g->getParent()->removeChild(g->getId());
//						m_allNodes.erase(g->getId());
//					}
//					m_scheduledForRemoval.clear();
//				}



				// restore if you want framebuffer rendering
				//glBindFramebuffer(GL_FRAMEBUFFER, _fb);
				//glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

				// update the room
				_room->update(dt);

				// draw the room
				_room->draw();
				//_engineDraw->draw(_room.get());

				glfwSwapBuffers(window);
				glfwPollEvents();
			}
			//m_shutdown = !(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
			//             glfwWindowShouldClose(window) == 0);
			_shutdown = !(glfwWindowShouldClose(window) == 0);

		} // Check if the ESC key was pressed or the window was closed
		while (_run && !_shutdown);
		_room->end();
		IBatch::resetId();
		//_room->end();
		//m_allNodes.clear();
		//_allPyObjects.clear();
		//if (_room) {
	//		_room->cleanUp();
		//}

	}
	_settings.release();
	_source.release();
	//_mainModule.release();
	//m_settings.attr("on_close")();

	//_batches.clear();
	//_engineDraw->shutdown();
	//m_game.release();
	//m_settings.release();
	//_factory.release();
	//_main.release();
	glfwTerminate();
}

void Game::WindowResizeCallback(GLFWwindow* win, int width, int height) {
	if (height == 0) {
		height = 1;
	}
    auto& game = Game::instance();
    game._screenHeight = height;
	float winAspectRatio = static_cast<float>(width) / height;
	game._windowSize = glm::ivec2(width, height);
	auto deviceSize = game.getDeviceSize();
	auto dar = game.getDeviceAspectRatio();
	int vx, vy, vw, vh;
	int sx = width / deviceSize.x;
	int sy = height / deviceSize.y;
	//engine.m_pixelScaleFactor = std::min(sx, sy);
	if (winAspectRatio > dar) {
		// vertical bands
		vw = (int) (height * dar);
		vh = (int) (vw / dar);
		vx = (int) ((width - vw) / 2);
		vy = (int) ((height - vh) / 2);
	} else {
		// horizontal bands
		vw = width;
		vh = (int) (width / dar);
		vx = 0;
		vy = (int) ((height - vh) / 2);
	}
//    vw = deviceSize.x * engine.m_pixelScaleFactor;
//    vh = deviceSize.y * engine.m_pixelScaleFactor;
//    vx = (int) ((width - vw) / 2.);
//    vy = (int) ((height - vh) / 2.);
//    glPointSize(engine.m_pixelScaleFactor);
//    glLineWidth(engine.m_pixelScaleFactor);
	game._windowViewport = glm::vec4(vx, vy, vw, vh);
	//std::cout << "cazzocane: " << vw << ", " << vh << "\n";
	//engine.setActualDeviceViewport(glm::vec4(0, 0, deviceSize.x, deviceSize.y));//glm::vec4(vx, vy, static_cast<float>(vw) / deviceSize[0], static_cast<float>(vh) / deviceSize[1]));

}

void Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	for (auto& s : Game::instance()._keyboardListeners) {
		auto retval = s->keyCallback(window, key, scancode, action, mods);
		if (retval == 1) {
			break;
		}
	}
}

void Game::mouse_button_callback(GLFWwindow* win, int button, int action, int mods) {
    for (auto &listener : Game::instance()._mouseListeners) {
        if (listener->mouseButtonCallback(win, button, action, mods) == 0) break;
    }
}

void Game::cursor_pos_callback(GLFWwindow * win, double xpos, double ypos) {
    for (auto &listener : Game::instance()._mouseListeners) {
        listener->cursorPosCallback(win, xpos, ypos);
    }
}

Shader * Game::getShader(int shaderId) {
	return _shaderStore->getShader(shaderId);
}

Room* Game::getRoom() {
	return _room.get();
}

void Game::registerToKeyboardEvent(KeyboardListener * listener) {
    _keyboardListeners.insert(listener);
}

void Game::unregisterToKeyboardEvent(KeyboardListener * listener) {
    _keyboardListeners.erase(listener);
}


void Game::registerToMouseEvent(MouseListener* listener) {
    _mouseListeners.insert(listener);
}

void Game::unregisterToMouseEvent(MouseListener * listener) {
    _mouseListeners.erase(listener);

}


glm::vec2 Game::getDeviceCoordinates(glm::vec2 s) {
    float devx = (s.x - _windowViewport.x) * _deviceSize.x / _windowViewport[2];
    float devy = (_screenHeight - s.y - _windowViewport.y) * _deviceSize.y / _windowViewport[3];
    return {devx, devy};
}

std::string Game::getPythonScriptDirectory() {
    auto mainModule = py::module_::import("__main__");
    fs::path fl = mainModule.attr("__file__").cast<std::string>();
    auto cwd = fl.parent_path();
    return cwd;
}

void Game::closeRoom() {
	_run = false;
}