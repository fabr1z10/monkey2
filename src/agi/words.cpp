#include "words.h"
#include <iostream>
#include <fstream>

using namespace agi;

Words::Words(const std::string &wordFile) {
	std::ifstream file("assets/" + wordFile);
	if (!file) {
		GLIB_FAIL("Cannot find words file");
	}
	std::string line;
	while (std::getline(file, line)) {
		if (line.empty()) continue;
		size_t colonPos = line.find(':');
		if (colonPos == std::string::npos) {
			GLIB_FAIL("Invalid row, missing colon.");
		}
		auto groupId = std::stoi(line.substr(0, colonPos));
		size_t openBrace = line.find('{', colonPos);
		size_t closeBrace = line.find('}', openBrace);
		if (openBrace == std::string::npos || closeBrace == std::string::npos) {
			GLIB_FAIL("Invalid row, missing open or close brace.");
		}

		std::string contents = line.substr(openBrace + 1, closeBrace - openBrace - 1);

		// Now extract strings inside quotes
		size_t pos = 0;
		while ((pos = contents.find('\'', pos)) != std::string::npos) {
			size_t endQuote = contents.find('\'', pos + 1);
			if (endQuote == std::string::npos) break;
			std::string word = contents.substr(pos + 1, endQuote - pos - 1);
			_stringToGroupId[word] = groupId;
			pos = endQuote + 1;
		}
	}
}

int Words::getGroupId(const std::string &s) {
	auto it = _stringToGroupId.find(s);
	if (it == _stringToGroupId.end()) {
		return 1;
	}
	return it->second;
}