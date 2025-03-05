#include "intersection.h"

Intersection::Intersection() {
	_map[{ShapeType::POINT, ShapeType::POLYGON}] = PointVsShape;

}

bool Intersection::test(const Shape& s1, const glm::mat4 & t1, const Shape& s2, const glm::mat4 & t2) {

	auto it = _map.find(std::make_pair(s1.getType(), s2.getType()));
	if (it == _map.end()) {
		auto it2 = _map.find(std::make_pair(s2.getType(), s1.getType()));
		if (it2 == _map.end()) {
			return false;
		}
		return it2->second(s2, t2, s1, t1);
	} else {
		return it->second(s1, t1, s2, t2);
	}
}

bool PointVsShape(const Shape & point, const glm::mat4 & pointTransform, const Shape& shape, const glm::mat4 & shapeTransform) {

	// point pos is now in world coords. need to convert it to shape coords
	glm::vec3 pointPos = pointTransform[3];
	glm::vec3 pp = glm::inverse(shapeTransform) * glm::vec4(pointPos, 1.f);
	return shape.isInside(pp);
}