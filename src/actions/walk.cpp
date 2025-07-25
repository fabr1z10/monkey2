#include "walk.h"

using namespace actions;

WalkTo::WalkTo(Node *node, adventure::WalkArea * walkArea, Vec2 position, float speed) : Action(),
    _node(dynamic_cast<adventure::WalkableCharacter*>(node)), _walkArea(walkArea), _targetPosition(position.toGlm()), _speed(speed) {
}

void WalkTo::start() {
    Action::start();
    glm::vec2 pos = _node->getWorldPosition().toGlm();
    if (glm::length(pos - _targetPosition) < 0.01) {
        _completed = true;
        return;
    }
        auto path = _walkArea->dijkstraShortestPath(pos, _targetPosition);

    if (path.size() < 2) {
        _completed = true;
        return;
    }
//    for (size_t i = 0; i < path.size(); i++) {
//        std::cout << path[i] << (i == path.size() - 1 ? "\n" : "->");
//    }
    _segmentId = 0;
    //std::string dir;
    for (size_t i = 1; i < path.size(); ++i) {
        glm::vec2 increment = path[i] - path[i-1];
        //dir = (increment.y >= fabs(increment.x) ? adventure::CharacterDirection::NORTH :
		//		(increment.y <= -fabs(increment.x) ? "-s" : "-e"));
        WalkSegment segment;
		segment.d = fabs(increment.y) > fabs(increment.x) ?
				(increment.y > 0 ? adventure::CharacterDirection::NORTH : adventure::CharacterDirection::SOUTH) :
				(increment.x > 0 ? adventure::CharacterDirection::EAST : adventure::CharacterDirection::WEST);
        segment.direction = glm::normalize(increment);
        segment.length = glm::length(increment);
        //segment.flip = (path[i].x < path[i-1].x);
        segment.anim = "walk";
        segment.targetPosition = path[i];
        _segments.push_back(segment);
    }
    _lastDirection = _segments.back().d;
    initialize();

}

void WalkTo::initialize() {
    _distanceTraversed = 0.f;
    const auto& s = _segments[_segmentId];
	_node->setDirection(s.d);
	_node->setAnimation(s.anim);
    //_node->flipHorizontal(s.flip);
    //_node->setAnimation(s.anim);
}

void WalkTo::run(double dt) {
    const auto& seg = _segments[_segmentId];
    float ds = (float)dt * _speed;
    auto delta = ds * seg.direction;
    _node->move(delta);
    _distanceTraversed += ds;
    if (_distanceTraversed >= seg.length) {
        _node->setPosition(Vec3(seg.targetPosition.x, seg.targetPosition.y, 0.f));
        _segmentId++;
        if (_segmentId >= _segments.size()) {
            _node->setAnimation("idle");
            _completed = true;
        } else {
            initialize();
        }
    }
}
