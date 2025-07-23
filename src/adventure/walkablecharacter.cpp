#include "walkablecharacter.h"
#include "../functions.h"

using namespace adventure;

WalkableCharacter::WalkableCharacter(int batch, const std::string &legsSprite, const std::string &bodySprite,
									 const std::string &headSprite) : Node(), _talking(false), _anim("idle"), _direction(CharacterDirection::EAST) {
	auto model = std::make_shared<models::TreeModel>();
	model->addSprite("legs", std::dynamic_pointer_cast<models::TileMap>(::getModel(0, legsSprite)), "", 0);
	model->addSprite("body", std::dynamic_pointer_cast<models::TileMap>(::getModel(0, bodySprite)), "legs", 0);
	model->addSprite("head", std::dynamic_pointer_cast<models::TileMap>(::getModel(0, headSprite)), "body", 0);
	_treeModel = model.get();
	setModel(model);
	updateModels();
}

std::unordered_map<CharacterDirection, std::pair<std::string, bool>> WalkableCharacter::_dirInfo = {
	{CharacterDirection::EAST, {"-e", false}},
	{CharacterDirection::WEST, {"-e", true}},
	{CharacterDirection::NORTH, {"-n", false}},
	{CharacterDirection::SOUTH, {"-s", false}}
};

void WalkableCharacter::setDirection(adventure::CharacterDirection d) {
	_direction = d;
	flipHorizontal(d == adventure::CharacterDirection::WEST);
	if (!_anim.empty()) {
		updateModels();
	}
}

void WalkableCharacter::setTalking(bool value) {
	_talking = value;
	updateModels();
}


void WalkableCharacter::setAnimation(const std::string &anim) {
	if (_anim == anim) return;
	_anim = anim;
	//const auto& d = _dirInfo[_direction];

	if (anim == "idle" || anim == "walk") {
		updateModels();
	}
}

void WalkableCharacter::updateModels() {
	const auto& d = _dirInfo[_direction];
	_treeModel->setNodeAnimation("legs", _anim + d.first);
	_treeModel->setNodeAnimation("body", _anim + d.first);
	_treeModel->setNodeAnimation("head", (_talking ? "talk" : "idle") + d.first);
}