#pragma once

#include "../room.h"
#include "../indexedpng.h"
#include "../keylistener.h"
#include "../text.h"

// agi - adventure game interpreter
namespace agi {

	class Words {
	public:
		Words(const std::string& wordFile);
		int getGroupId(const std::string& s);
	private:
		std::unordered_map<std::string, int> _stringToGroupId;
	};


}