#include <locale>
#include <codecvt>
#include "util.h"

std::ostream& operator<<(std::ostream& os, const Vec2& vec) {
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const IVec2& vec) {
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}

// Overload the << operator for glm::vec2/3/4
std::ostream& operator<<(std::ostream& os, const glm::vec2& vec) {
	os << "(" << vec.x << ", " << vec.y << ")";
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec) {
	os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return os;
}

std::ostream& operator<<(std::ostream& os, const Vec3& vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec4& vec) {
	os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
	return os;
}

std::ostream& operator<<(std::ostream& os, const Vec4& vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
    return os;
}


std::u32string getString32(const std::string& str) {
    return std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>().from_bytes(str.c_str());
}

uint32_t packRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	return (static_cast<uint32_t>(r) << 24) |
		(static_cast<uint32_t>(g) << 16) |
		(static_cast<uint32_t>(b) << 8)  |
		static_cast<uint32_t>(a);
}

void unpackRGBA(uint32_t color, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a) {
	r = (color >> 24) & 0xFF;
	g = (color >> 16) & 0xFF;
	b = (color >> 8)  & 0xFF;
	a = color & 0xFF;
}