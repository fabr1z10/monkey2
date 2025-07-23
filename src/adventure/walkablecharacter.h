#pragma once

#include "../node.h"
#include "../models/treemodel.h"

namespace adventure {
	enum CharacterDirection {
		NORTH,
		SOUTH,
		EAST,
		WEST
	};

	class WalkableCharacter : public Node {
	public:
		WalkableCharacter(int batch, const std::string& legsSprite, const std::string& bodySprite, const std::string& headSprite);

		void setTalking(bool);

		CharacterDirection getDirection();

		void setDirection(CharacterDirection);

		void setAnimation(const std::string& anim) override;
	private:
		void updateModels();
		std::string _anim;
		models::TreeModel* _treeModel;
		bool _talking;
		CharacterDirection _direction;
		static std::unordered_map<CharacterDirection, std::pair<std::string, bool>> _dirInfo;
	};

	inline CharacterDirection WalkableCharacter::getDirection() {
		return _direction;
	}

}