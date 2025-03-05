#pragma once

#include <glm/glm.hpp>
struct Bounds {

	Bounds() : xm(0.f), xM(0.f), ym(0.f), yM(0.f) {}

	Bounds transform(const glm::mat4& t);

	float xm;
	float xM;
	float ym;
	float yM;

	bool testAABB(const Bounds& other) {
		auto notCollide = other.xm > xM || xm > other.xM || other.ym > yM || ym > other.yM;
		return !notCollide;
	}
};