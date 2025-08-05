#pragma once

#include "../node.h"


// agi - adventure game interpreter
namespace agi {

	class AGIRoom;

	class AGIObject : public Node {
	public:
		AGIObject(const std::string& viewId, int x, int y, float speed);

		void computePriority();

		void customUpdate(double) override;
	private:
		float _x;
		float _y;
		float _speed;
		int _priority;
		float _horizontalMove;
		float _verticalMove;
		int _refresh;
		int _ticks;
		double _accumulatedTime;
		agi::AGIRoom* _room;

		// Each object on screen has a direction it is moving in. This is a numerical value from 0 to 8:
		// When an object is moving normally (and has a direction greater than 0), it will continue moving
		// until it is told to stop, move in another direction, or runs into an obstacle (such as a control line,
		// a block, or another object).
		int _dir;
		const static std::unordered_map<int, int> _keyDir;
		bool _hasUpDownAnim;
		bool _hasLeftAnim;
	};


}