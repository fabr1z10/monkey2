#include "model.h"

IModel::IModel() : _node(nullptr), _update(true) {}


std::string IModel::getAnimation() {
	return "";
}

void IModel::update() {
	if (_update) {
		updateImpl();
	}
}