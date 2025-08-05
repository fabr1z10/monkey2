#include "agiroom.h"
#include "../indexedpng.h"
#include "../camera.h"
#include "prioritysheet.h"
#include "../linebatch.h"
#include "../game.h"
#include "../models/quad.h"

using namespace agi;

AGIRoom::AGIRoom(const std::string &pic, const std::string &spritesheet) : Room(), KeyboardListener(), _cursorChar("_") {

	auto assetPath = Game::instance().getWorkingDirectory() + "/assets/";
	// Create the main camera
	auto gameCam = std::make_shared<OrthoCamera>(160, 168,
												 Vec4(0, 24, 320, 168), 0.f, 1.f);
	gameCam->setPosition(Vec3(80, 84, 0.f), Vec3(0, 0, -1), Vec3(0, 1, 0));
	addCamera(gameCam);

	// UI camera
	auto uiCam = std::make_shared<OrthoCamera>(320, 200,
											   Vec4(0,0, 320, 200), 0.f, 1.f);
	uiCam->setPosition(Vec3(160, 100, 0), Vec3(0,0,-1), Vec3(0, 1, 0));
	addCamera(uiCam);

	// First thing we do is load the control image
	_controlImage = std::make_unique<IndexedPNG>(assetPath + "/" + pic + "c.png");

	//std::cout << " pixel @ 0,0 = " << (int)_priorityMap->getPixel(40, 22) << "\n";
	addBatch(std::make_shared<PicBatch>(0, pic));
	addBatch(std::make_shared<QuadBatchPalette>(256, 0, spritesheet));
	addBatch(std::make_shared<QuadBatchPalette>(256, 1, "sprites.yaml"));


	// Test image
//	auto model = std::make_shared<models::Quad>(0, 0, 0, 160, 168);
//	auto node = std::make_shared<Node>();
//	node->setModel(model);
//	getRoot()->add(node);

}

unsigned char AGIRoom::getPixel(int x, int y) {
	return _controlImage->getPixel(x, y);
}

int AGIRoom::keyCallback(GLFWwindow *, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_BACKSPACE) {
			if (_command.size() > 0) {
				_command.pop_back();
			} else {
				return 0;
			}
		} else {
			_command += char(key);
		}
	}
	auto s = ">" + _command + _cursorChar;
	_txtCommand->updateText(s);

}

void AGIRoom::setup() {
	auto s = ">" + _command + _cursorChar;
	auto text = std::make_shared<Text>(2, "sierra", s, 0, HAlign::LEFT, 0.f, Vec2());
	text->setPosition(Vec3(0, 24, 0));
	getRoot()->add(text);
	_txtCommand = text.get();
}