#include "functions.h"

Game& game() {
	auto& g = Game::instance();
	return g;
}

glm::vec4 fromHex(const std::string& str) {
    unsigned int r = std::stoul(str.substr(1, 2), nullptr, 16);
    unsigned int g = std::stoul(str.substr(3, 2), nullptr, 16);
    unsigned int b = std::stoul(str.substr(5, 2), nullptr, 16);
    return glm::vec4(r / 255.f, g / 255.f, b / 255.f, 1.f);

}