#include "agiroom.h"
#include "../indexedpng.h"
#include "../camera.h"
#include "prioritysheet.h"
#include "../linebatch.h"
#include "../game.h"
#include "../models/quad.h"

using namespace agi;

AGIRoom::AGIRoom(const std::string &pic, const std::string &spritesheet) : Room() {

	auto assetPath = Game::instance().getWorkingDirectory() + "/assets/";
	// Create the camera
	auto gameCam = std::make_shared<OrthoCamera>(160, 168,
												 Vec4(0, 0, 160, 168), 0.f, 1.f);
	gameCam->setPosition(Vec3(80, 84, 0.f), Vec3(0, 0, -1), Vec3(0, 1, 0));
	addCamera(gameCam);

	// First thing we do is load the control image
	_controlImage = std::make_unique<IndexedPNG>(assetPath + "/" + pic + "p.png");

	//std::cout << " pixel @ 0,0 = " << (int)_priorityMap->getPixel(40, 22) << "\n";
	addBatch(std::make_shared<PicBatch>(0, pic));
	addBatch(std::make_shared<QuadBatchPalette>(256, 0, spritesheet));

	// Test image
//	auto model = std::make_shared<models::Quad>(0, 0, 0, 160, 168);
//	auto node = std::make_shared<Node>();
//	node->setModel(model);
//	getRoot()->add(node);

}