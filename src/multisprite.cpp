#include "multisprite.h"
#include "error.h"
#include "renderer.h"

void MultiSprite::addNode(std::shared_ptr<Node> node, int parent, int slot, int z) {
	int newId = _subNodes.size();
	if (parent == -1 && !_subNodes.empty()) {
		GLIB_FAIL("Attempting to have multiple roots in multisprite.")
	}
	auto* sr = dynamic_cast<SpriteRenderer*>(node->getRenderer());
	M_Assert(sr != nullptr, "Can only add sprites to multisprite.");
	if (parent == -1) {
		Node::add(node);
	}
	else {
		_subNodes[parent]->add(node);
	}
	_subNodes.push_back(node);
	_renderers.push_back(sr);
	if (parent != -1) {
		_tree[parent].push_back({ newId, slot, z*0.01f });
	}
	node->setActive(false);
	_tree[newId] = {};
}

void MultiSprite::setAnimation(const std::string& anim) {
	
	auto it = _anims.find(anim);
	M_Assert(it != _anims.end(), ("[Multisprite] Unknown animation: " + anim).c_str());
	const auto& v = it->second;
	for (size_t i = 0; i < v.size(); ++i) {
		if (!v[i].empty()) {
			_subNodes[i]->setAnimation(v[i]);
		}
	}
}

void MultiSprite::addAnimation(const std::string& anim, const std::vector<std::string>& subAnims) {
	_anims[anim] = subAnims;
}

void MultiSprite::render() {
	// first, need to update all frames. If a node 
	// updates frmae, then for each
	
	// walk through tree
	std::list<int> l = {0};

	while (!l.empty()) {
		auto current = l.front();
		l.pop_front();
		auto& node = _subNodes[current];
		if (node->getRenderer()->updateFrame()) {
			// ok, if we get here, we need to move child
			node->getRenderer()->updateGeometry();
			for (const auto& c : _tree[current]) {
				glm::vec2 slot = _renderers[current]->getSlot(c.slot);
				// check slot to which child is attached
				
				_subNodes[c.id]->setPosition(glm::vec3(slot, c.z));
				std::cout << "ciao " << _renderers[current]->getFrame() << slot.x << ", " << slot.y << "\n";
			}
		}
		for (const auto& c : _tree[current]) {
			l.push_back(c.id);
		}
		
	}

}