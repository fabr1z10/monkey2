#include "keylistener.h"
#include "game.h"

KeyboardListener::KeyboardListener() {
    Game::instance().registerToKeyboardEvent(this);

}

KeyboardListener::~KeyboardListener() {
    Game::instance().unregisterToKeyboardEvent(this);
}