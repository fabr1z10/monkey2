#include "functions.h"

Game& game() {
	auto& g = Game::instance();
	return g;
}
