#pragma once

#include <string>
#include <vector>
#include "glm/glm.hpp"

class PolyApproximation {
public:
	static std::vector<glm::vec2> approximatePoly(
		const std::string& path, glm::ivec4 coords, float epsilon);

private:
	static std::vector<glm::vec2> findContourPoints(unsigned char* alpha, int width, int height);
	static std::vector<glm::vec2> douglasPeucker(const std::vector<glm::vec2>& points, float);
	static float pointToLineDistance(const glm::vec2& p, const glm::vec2& a, const glm::vec2& b);

};