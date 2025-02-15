#include <locale>
#include <codecvt>
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

std::u32string getString32(const std::string& str) {
    return std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>().from_bytes(str.c_str());
}
