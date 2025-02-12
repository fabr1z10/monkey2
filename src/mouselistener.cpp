#include "mouselistener.h"
#include "game.h"

MouseListener::MouseListener() {
    Game::instance().registerToMouseEvent(this);

}

MouseListener::~MouseListener() {
    Game::instance().unregisterToMouseEvent(this);

}