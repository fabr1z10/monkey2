#pragma once

#include "node.h"
#include "sprite.h"

class MultiSprite : public Node {

public:

	void setAnimation(const std::string&) override;
	
	std::string getAnimation() const override;

	void addAnimation(const std::string&, const std::vector<std::string>&);

	void addNode(std::shared_ptr<Node>, int parent=-1, int slot=0, int z=0);

	void render() override;

private:

	struct NodeRelation {
		int id;
		int slot;
		float z;
	};

	// 0 is always root
	std::unordered_map<int, std::vector<NodeRelation> > _tree;
	std::vector< SpriteRenderer*> _renderers;
	std::unordered_map<std::string, std::vector<std::string> > _anims;
	std::vector<std::shared_ptr<Node>> _subNodes;
	std::string _animation;
};