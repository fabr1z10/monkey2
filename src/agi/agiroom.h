#pragma once

#include "../room.h"
#include "../indexedpng.h"
#include "../keylistener.h"
#include "../text.h"

// agi - adventure game interpreter
namespace agi {

	class AGIRoom : public Room, public KeyboardListener {
	public:
		AGIRoom(const std::string& pic, const std::string& view);
		unsigned char getPixel(int x, int y);
		int keyCallback(GLFWwindow*, int key, int scancode, int action, int mods) override;
		void setup() override;
	private:
		std::unique_ptr<IndexedPNG> _controlImage;
		std::string _cursorChar;
		std::string _command;
		Text* _txtCommand;
	};


}