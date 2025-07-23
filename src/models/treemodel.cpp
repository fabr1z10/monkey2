#include "treemodel.h"


using namespace models;

void TreeModel::update() {


	std::list<TreeNode*> l{_root.get()};
	while (!l.empty()) {
		auto* current = l.front();
		l.pop_front();
		if (current->tileMap == nullptr) continue;
		// this will trigger a redraw
		current->tileMap->update();
		for (auto& c : current->children) {
			auto offset = current->tileMap->getOffset() + current->tileMap->getBonePosition(c->boneId);
			c->tileMap->setOffset(offset);
			l.push_back(c.get());
		}
	}
	for (const auto& n : _nodes) {
		n.second->tileMap->draw();
	}
}

void TreeModel::addSprite(const std::string &id, std::shared_ptr<models::TileMap> node, const std::string &parent, int boneId) {
	auto n = std::make_shared<TreeModel::TreeNode>();
	n->id = id;
	n->tileMap = node;
	n->boneId = boneId;

	_nodes[id] = n;
	node->setAutoDraw(false);
	if (!parent.empty()) {
		_nodes.at(parent)->children.push_back(n);
	} else {
		_root = n;
	}
}

void TreeModel::setNode(Node* node) {
	IModel::setNode(node);
	for (const auto& m : _nodes) {
		m.second->tileMap->setNode(node);
	}
}

void TreeModel::setVisible(bool value) {
	for (const auto& t: _nodes) {
		t.second->tileMap->setVisible(value);
	}
}

void TreeModel::setNodeAnimation(const std::string &node, const std::string &anim) {
	_nodes.at(node)->tileMap->setAnimation(anim);
}
void TreeModel::setAnimation(const std::string & id) {
	auto it = _animData.find(id);
	if (it != _animData.end()) {
		for (const auto& it2 : it->second) {
			_nodes.at(it2.first)->tileMap->setAnimation(it2.second);
		}
	}

}

void TreeModel::addAnimation(const std::string &id, const std::string &node, const std::string &anim) {
	_animData[id][node] = anim;
}

//void WalkableCharacter::setAnimation(const std::string& id) {
//	auto it = _animData.find(id);
//	if (it != _animData.end()) {
//		for (const auto& it2 : it->second) {
//			_anims.at(it2.first) = it2.second;
//			//_nodes.at(it2.first)->tileMap->setAnimation(it2.second + "-" + _direction);
//		}
//	}
//}
//
//void WalkableCharacter::setDirection(const std::string& d) {
//	_direction = d;
//
//}
//
//void WalkableCharacter::updateAnimations() {
//	for (const auto& a : _anims) {
//		_nodes.at(a.first)->tileMap->setAnimation()
//	}
//}
//void WalkableCharacter::addSprite(const std::string &id, std::shared_ptr<models::TileMap> node, const std::string &parent,
//								  int boneId) {
//
//	TreeModel::addSprite(id, node, parent, boneId);
//	_anims[id] = "";
//}