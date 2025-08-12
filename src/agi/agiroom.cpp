#include "agiroom.h"
#include "../indexedpng.h"
#include "../camera.h"
#include "prioritysheet.h"
#include "../linebatch.h"
#include "../game.h"
#include "../models/quad.h"
#include "../models/linemodel.h"
#include <filesystem> // C++17
#include <cctype>
#include <fstream>

namespace fs = std::filesystem;
using namespace agi;

extern GLFWwindow * window;

std::shared_ptr<AGIContext> AGIRoom::_agi;


AGIContext::AGIContext() {
	auto dirPath = Game::instance().getWorkingDirectory() + "/assets/messages";

	for (const auto &entry : fs::directory_iterator(dirPath)) {
		if (!entry.is_regular_file()) continue;

		// filename as int (room number)
		std::string filename = entry.path().filename().string();
		// skip files whose names are not numeric
		if (!std::all_of(filename.begin(), filename.end(), ::isdigit))
			continue;
		int roomNumber = std::stoi(filename);

		std::ifstream file(entry.path());
		if (!file) {
			std::cerr << "Cannot open file: " << entry.path() << "\n";
			continue;
		}

		std::string line;
		while (std::getline(file, line)) {
			std::istringstream iss(line);
			int number;
			char colon;
			std::string text;

			if (iss >> number >> colon && colon == ':' && std::getline(iss >> std::ws, text)) {
				_messages[roomNumber][number] = text;
			}
		}
	}

	// Example: print everything
	for (auto &[room, innerMap] : _messages) {
		std::cout << "Room " << room << ":\n";
		for (auto &[num, str] : innerMap) {
			std::cout << "  " << num << ": " << str << "\n";
		}
	}
	//exit(1);
}

bool AGIContext::isSet(int id) const {
	return _flags[id];
}

void AGIContext::setFlag(int id) {
	_flags[id]= true;
}

void AGIContext::resetFlag(int id) {
	_flags[id]= false;
}

void AGIContext::toggleFlag(int id) {
	_flags[id] = !_flags[id];
}

int AGIContext::getVar(int id) const {
	return _variables[id];
}

void AGIContext::setVar(int id, int value) {
	_variables[id] = value;
}

std::string AGIContext::getMessage(int room, int msgId) {
	return _messages.at(room).at(msgId);
}

void AGIRoom::pause(bool value) {
	if (!value && _message != nullptr) {
		_message->remove();
		_message = nullptr;
		if (_objShow != nullptr) {
			_objShow->remove();
			_objShow = nullptr;
		}
	}
	_paused = value;
	_mainNode->setActive(!value);
}

void AGIRoom::quit() {
	_quitRequested = true;
	print(_quitMessage);
}


void AGIRoom::addSaid(const std::vector<int> &ids, pybind11::function f) {
	Trie* current = _parserRoot.get();
	for (const auto& gid : ids) {
		// check if we already have it
		auto iter = current->groupIds.find(gid);
		if (iter != nullptr) {
			current = iter->second.get();
		} else {
			// create a new one
			auto tr = std::make_shared<Trie>();
			current->groupIds[gid] = tr;
			current = tr.get();
		}
	}
	current->script = f;

}

AGIRoom::AGIRoom(int id, const std::string &pic, const std::string &spritesheet) : Room(), KeyboardListener(), _cursorChar("_"),
	_paused(false), _message(nullptr), _objShow(nullptr), _quitRequested(false), _textMaxLength(30) {

	if (!_agi) {
		_agi = std::make_shared<AGIContext>();
	}
	_agi->setVar(0, id);

	std::cout << "Room created\n";

	_egaPal[4] = Color("#aa0000");

	_wordsTok = std::make_shared<Words>("words");

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
	addBatch(std::make_shared<TriangleBatch>(256, 1));
	addBatch(std::make_shared<LineBatch>(256, 1));


	// Test image
//	auto model = std::make_shared<models::Quad>(0, 0, 0, 160, 168);
//	auto node = std::make_shared<Node>();
//	node->setModel(model);
//	getRoot()->add(node);
	auto main = std::make_shared<Node>();
	auto ui = std::make_shared<Node>();
	getRoot()->add(main);
	getRoot()->add(ui);
	_mainNode = main.get();
	_uiNode = ui.get();
	_parserRoot = std::make_shared<Trie>();

	_quitMessage = Game::instance().getValue<std::string>("quitMessage");
}


void AGIRoom::add(std::shared_ptr<Node> node) {
	_mainNode->add(node);
}
unsigned char AGIRoom::getPixel(int x, int y) {
	return _controlImage->getPixel(x, y);
}

//void AGIRoom::processInput(const std::string &command) {
//	std::cout << "-- processing: " << command << std::endl;
//}

int AGIRoom::keyCallback(GLFWwindow *, int key, int scancode, int action, int mods) {
	if (!_paused) {
		bool shift = (mods & GLFW_MOD_SHIFT) || (mods & GLFW_MOD_CAPS_LOCK);
		//std::cout << key << "..." << mods << "\n";

		if (action == GLFW_PRESS) {
			if (key == GLFW_KEY_F1) {
				pause(!_paused);
				return 0;
			}
			if (key == GLFW_KEY_ENTER) {
				std::string c(_command);
				_command.clear();
				_txtCommand->updateText(">" + _cursorChar);
				parseCommand(c);
				return 0;
			}
			if (key == GLFW_KEY_BACKSPACE) {
				if (_command.size() > 0) {
					_command.pop_back();
				} else {
					return 0;
				}
			} else {
				if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
					if (!shift) {
						key += 0x20;
					}
					_command += char(key);
				} else if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9 || key == GLFW_KEY_SPACE) {
					_command += char(key);
				}

			}
		}
		auto s = ">" + _command + _cursorChar;
		_txtCommand->updateText(s);
	} else {
		if (action == GLFW_PRESS && (key == GLFW_KEY_ENTER || key == GLFW_KEY_ESCAPE)) {
			if (_quitRequested && key == GLFW_KEY_ENTER) {
				//Game::instance().shutdown();
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				Game::instance().closeRoom();
			}
			_quitRequested = false;
			pause(false);
		}
	}

}

void AGIRoom::setup() {
	auto s = ">" + _command + _cursorChar;
	auto text = std::make_shared<Text>(2, "sierra", s, 0, HAlign::LEFT, 0.f, Vec2());
	text->setPosition(Vec3(0, 24, 0));
	getRoot()->add(text);
	_txtCommand = text.get();
}

void AGIRoom::parseCommand(const std::string& command) {
	// as a first step, I split the string into words separated by space
	auto words = splitBySpace(command);
	std::vector<int> gIds;
	for (const auto& w : words) {
		gIds.push_back(_wordsTok->getGroupId(w));
	}
	std::cout << "command entered: [";
	for (size_t i = 0; i < gIds.size(); ++i) {
		std::cout << gIds[i] << (i == gIds.size()-1 ? "]" : ", ");
	}
	std::cout << "\n";
	// now map each word into the corresponding group id
	int i = 0;
	std::string errWord;
	Trie* t = _parserRoot.get();
	while (i < gIds.size()) {
		auto it = t->groupIds.find(gIds[i]);
		if (it == t->groupIds.end()) {
			// not found, try anyword
			if (t->groupIds.count(1) == 0) {
				t = nullptr;
				break;
			}
		}
		// increase i
		i++;
		t = it->second.get();
	}
	if (t == nullptr) {
		auto culpritWord = words[i];
		std::cout << "Don't know " + culpritWord << "\n";
		print("I don't understand \"" + culpritWord + "\".");
	} else {
		if (t->script) {
			t->script(this);
		}
	}
}


std::vector<std::string> AGIRoom::splitBySpace(const std::string &str) {
	std::istringstream iss(str);
	std::vector<std::string> tokens;
	std::string word;
	while (iss >> word) { // automatically skips extra spaces
		tokens.push_back(word);
	}
	return tokens;
}

void AGIRoom::print(int room, int msgId) {
	print(_agi->getMessage(room, msgId));
}

void AGIRoom::showObject(int objId) {
	auto* batch = dynamic_cast<QuadBatchPalette*>(getBatch(1));
	auto model = batch->getSpriteSheet()->makeSprite(1, std::to_string(objId));
	auto node = std::make_shared<Node>();
	node->setModel(model);
	node->setPosition(Vec3(80, 0, 0));
	_mainNode->add(node);
	_objShow = node.get();
	print(65536, objId);

}

void AGIRoom::print(const std::string & pippo) {
	auto wrappedText = wrapText(pippo, _textMaxLength);
	auto text = std::make_shared<Text>(2, "sierra", wrappedText, 1,
									   HAlign::LEFT, 0.f, Vec2(-0.5f, -0.5f));
	text->setPosition(Vec3(160, 100, 0));
	auto textSize = text->getSize();
	std::cout << "size = " << text->getSize().x << ", " << text->getSize().y << "\n";

	int padx = 10;
	int pady = 5;

	float rect_x = - textSize.x*0.5f - padx;
	float rect_x1 = textSize.x*0.5f + padx;

	float rect_y0 = - textSize.y*0.5f - pady;
	float rect_y1 = textSize.y*0.5f + pady;


	auto whiteSquare = std::make_shared<Node>();
	whiteSquare->setModel(std::make_shared<models::ColorQuadModel>(3, textSize.x + 2*padx,textSize.y+2*pady, Color(1.f, 1.f, 1.f, 1.f)));
	whiteSquare->setPosition(Vec3(- textSize.x*0.5f - padx , - textSize.y*0.5f-pady, -0.01f));
	text->add(whiteSquare);
	//getRoot()->add(whiteSquare);

//	auto topHorLine = std::make_shared<Node>();
//	std::vector<float> p = {0, 0, textSize.x + 2.f * padx - 4,0};
//	topHorLine->setModel(std::make_shared<models::LineModel>(4, p, _egaPal[4]));
//	topHorLine->setPosition(Vec3(-textSize.x*0.5f - padx + 2, textSize.y * 0.5f + pady -2, -0.f));
//	text->add(topHorLine);

	text->add(line(rect_x + 2, rect_y1 - 2, 0.f, 0, 0, textSize.x + 2.f*padx - 4, 0, 4));
	text->add(line(rect_x+2, rect_y0 +3, 0.f, 0, 0,textSize.x + 2.f*padx - 4, 0, 4));
	text->add(line(rect_x+2, rect_y0 +2, 0.f, 0, 0,0, textSize.y + 2.f * pady - 4, 4));
	text->add(line(rect_x+3, rect_y0 +2, 0.f, 0, 0,0, textSize.y + 2.f * pady - 4, 4));
	text->add(line(rect_x1 -2, rect_y0 +2, 0.f, 0, 0,0, textSize.y + 2.f * pady - 4, 4));
	text->add(line(rect_x1 -3, rect_y0 +2, 0.f, 0, 0,0, textSize.y + 2.f * pady - 4, 4));

	getRoot()->add(text);
	_message = text.get();
	pause(true);
}

std::shared_ptr<Node> AGIRoom::line(float x, float y, float z, float x0, float y0, float x1, float y1, int color) {
	auto line = std::make_shared<Node>();
	std::vector<float> p = {x0, y0, x1, y1};
	line->setModel(std::make_shared<models::LineModel>(4, p, _egaPal[color]));
	line->setPosition(Vec3(x, y, z));
	return line;
}

std::string AGIRoom::wrapText(const std::string &text, size_t maxLen) {
	std::string result;
	size_t start = 0;

	while (start < text.size()) {
		size_t end = start + maxLen;
		if (end >= text.size()) {
			result += text.substr(start);
			break;
		}

		size_t spacePos = text.rfind(' ', end);
		if (spacePos == std::string::npos || spacePos < start) {
			spacePos = end;
		}

		result += text.substr(start, spacePos - start);
		result += '\n';

		start = (text[spacePos] == ' ') ? spacePos + 1 : spacePos;
	}

	return result;
}


bool AGIRoom::isSet(int id) const {
	return _agi->isSet(id);
}

void AGIRoom::setFlag(int id) {
	_agi->setFlag(id);
}

void AGIRoom::resetFlag(int id) {
	_agi->resetFlag(id);
}

void AGIRoom::toggleFlag(int id) {
	_agi->toggleFlag(id);
}

int AGIRoom::getVar(int id) const {
	return _agi->getVar(id);
}

void AGIRoom::setVar(int id, int value) {
	_agi->setVar(id, value);
}