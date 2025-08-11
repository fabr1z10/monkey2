#pragma once

#include "../room.h"
#include "../indexedpng.h"
#include "../keylistener.h"
#include "../text.h"
#include "words.h"

// agi - adventure game interpreter
namespace agi {

	class Trie {
	public:
		std::unordered_map<int, std::shared_ptr<Trie>> groupIds;
		pybind11::function script;
	};


	class AGIRoom : public Room, public KeyboardListener {
	public:
		AGIRoom(const std::string& pic, const std::string& view);
		unsigned char getPixel(int x, int y);
		int keyCallback(GLFWwindow*, int key, int scancode, int action, int mods) override;
		void setup() override;
		void parseCommand(const std::string&);
		void print(const std::string&);
		void add(std::shared_ptr<Node>);
		void pause(bool);
	private:
		std::vector<std::string> splitBySpace(const std::string &str);
		std::string wrapText(const std::string &text, size_t maxLen);
		std::shared_ptr<Node> line(float x, float y, float z, float x0, float y0, float x1, float y1, int color);
		//void processInput(const std::string& command);
		std::unique_ptr<IndexedPNG> _controlImage;
		std::string _cursorChar;
		std::string _command;
		Text* _txtCommand;
		Node* _mainNode;
		Node* _uiNode;
		std::shared_ptr<Trie> _parserRoot;
		std::shared_ptr<Words> _wordsTok;
		bool _paused;
		Color _egaPal[16];
		Node* _message;
	};


}