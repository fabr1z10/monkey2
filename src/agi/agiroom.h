#pragma once

#include "../room.h"
#include "../indexedpng.h"

// agi - adventure game interpreter
namespace agi {

	class AGIRoom : public Room {
	public:
		AGIRoom(const std::string& pic, const std::string& view);

	private:
		std::unique_ptr<IndexedPNG> _controlImage;
	};


}