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


	class AGIContext {
	public:
		AGIContext();
		bool isSet(int) const;
		void setFlag(int);
		void resetFlag(int);
		void toggleFlag(int);
		int getVar(int) const;
		void setVar(int, int);
		std::string getMessage(int room, int msgId);
	private:
		std::unordered_map<int, std::unordered_map<int, std::string>> _messages;
		bool _flags[256]{};
		int _variables[256]{};
	};

	class AGIRoom : public Room, public KeyboardListener {
	public:
		AGIRoom(int id, const std::string& pic, const std::string& view);
		unsigned char getPixel(int x, int y);
		int keyCallback(GLFWwindow*, int key, int scancode, int action, int mods) override;
		void setup() override;
		void parseCommand(const std::string&);
		void print(const std::string&);
		void print(int, int);
		void showObject(int);
		void add(std::shared_ptr<Node>);
		void pause(bool);
		void addSaid(const std::vector<int>& ids, pybind11::function f);
		void quit();

		// forward to context
		bool isSet(int) const;
		void setFlag(int);
		void resetFlag(int);
		void toggleFlag(int);
		int getVar(int) const;
		void setVar(int, int);
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
		bool _quitRequested;
		Color _egaPal[16];
		Node* _message;
		Node* _objShow;
		std::string _quitMessage;
		static std::shared_ptr<AGIContext> _agi;
		int _textMaxLength;
	};


}