#pragma once
#include "tilemodel.h"


namespace models {

	class TreeModel : public IModel {

	public:

		struct TreeNode {
			std::string id;
			std::shared_ptr<models::TileMap> tileMap = nullptr;
			int boneId;
			std::vector<std::shared_ptr<TreeNode>> children;
		};

		void update() override;

		void draw() override {}

		TreeModel() = default;

		virtual void
		addSprite(const std::string &id, std::shared_ptr<models::TileMap>, const std::string &parent, int boneId);

		void setNode(Node *) override;

		void setVisible(bool) override;

		std::string getAnimation() override;

		void setAnimation(const std::string &) override;

		void setNodeAnimation(const std::string &node, const std::string &anim);

		void addAnimation(const std::string &id, const std::string &node, const std::string &anim);

	protected:
		std::string _animation;

		std::shared_ptr<TreeNode> _root;

		std::unordered_map<std::string, std::shared_ptr<TreeNode>> _nodes;

		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> _animData;
	};

	inline std::string TreeModel::getAnimation() {
		return _animation;
	}
}

//	class WalkableCharacter : public TreeModel {
//	public:
//		WalkableCharacter() : TreeModel();
//
//		void addSprite(const std::string& id, std::shared_ptr<models::TileMap>, const std::string &parent, int boneId) override;
//
//		void updateAnimations();
//
//		void setAnimation(const std::string&) override;
//
//		void setDirection(const std::string&) override;
//
//		std::string _legsAnim;
//		std::string _bodyAnim;
//		std::string _headAnim;
//		std::string _direction;
//		std::unordered_map<std::string, std::string> _anims;
//	};
//}