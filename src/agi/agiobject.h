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
	};


}