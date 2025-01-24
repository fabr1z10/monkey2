
#include "util.h"

// Overload the << operator for glm::vec2/3/4
std::ostream& operator<<(std::ostream& os, const glm::vec2& vec) {
	os << "(" << vec.x << ", " << vec.y << ")";
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec) {
	os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec4& vec) {
	os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
	return os;
}