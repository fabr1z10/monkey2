#pragma once

#include "glm/glm.hpp"
#include <cmath>
#include <string>
#include <vector>

class Vec2 {
public:
    Vec2() : x(0.f), y(0.f) {}

    Vec2(float x, float y) : x(x), y(y) {}

    Vec2(const std::vector<float>& data);

    // Add two Vec2s
    Vec2 operator+(const Vec2& other) const {
        return Vec2(x + other.x, y + other.y);
    }

    // Multiply Vec2 by scalar
    Vec2 operator*(float scalar) const {
        return Vec2(x * scalar, y * scalar);
    }

    // Return the length of the vector
    float length() const {
        return std::sqrt(x * x + y * y);
    }
    bool isZero() {
        return (x == 0.f && y == 0.f);
    }
    glm::vec2 toGlm() const {
        return glm::vec2(x, y);
    }
    float x;
    float y;
};

class IVec2 {
public:
    IVec2() : x(0), y(0) {}

    IVec2(int x, int y) : x(x), y(y) {}

    // Add two Vec2s
    IVec2 operator+(const IVec2& other) const {
        return IVec2(x + other.x, y + other.y);
    }

    // Multiply Vec2 by scalar
    IVec2 operator*(int scalar) const {
        return IVec2(x * scalar, y * scalar);
    }

    // Return the length of the vector
    float length() const {
        return std::sqrt(x * x + y * y);
    }

    bool isZero() {
        return (x == 0 && y == 0);
    }
    glm::ivec2 toGlm() const {
        return glm::ivec2(x, y);
    }
    int x;
    int y;
};


class Vec3 {
public:
    Vec3() : x(0.f), y(0.f), z(0.f) {}

    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vec3(const std::vector<float>& data);

    Vec3(glm::vec3 v) : x(v.x), y(v.y), z(v.z) {}

    Vec3 operator+(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    Vec3 operator*(float scalar) const {
        return Vec3(x * scalar, y * scalar, z*scalar);
    }

    // Return the length of the vector
    float length() const {
        return std::sqrt(x * x + y * y + z*z);
    }

    bool isZero() {
        return (x == 0.f && y == 0.f && z == 0.f);
    }

    glm::vec3 toGlm() const {
        return glm::vec3(x, y, z);
    }
    float x;
    float y;
    float z;
};

class IVec3 {
public:
	IVec3() : x(0), y(0), z(0) {}

	IVec3(int x, int y, int z) : x(x), y(y), z(z) {}

	IVec3(const std::vector<int>& data);

	IVec3(glm::ivec3 v) : x(v.x), y(v.y), z(v.z) {}

	IVec3 operator+(const IVec3& other) const {
		return IVec3(x + other.x, y + other.y, z + other.z);
	}

	IVec3 operator*(float scalar) const {
		return IVec3(x * scalar, y * scalar, z*scalar);
	}

	// Return the length of the vector
	float length() const {
		return std::sqrt(x * x + y * y + z*z);
	}

	bool isZero() {
		return (x == 0 && y == 0 && z == 0);
	}

	glm::ivec3 toGlm() const {
		return glm::ivec3(x, y, z);
	}
	int x;
	int y;
	int z;
};


class Vec4 {
public:
    Vec4() : x(0.f), y(0.f), z(0.f), w(0.f) {}

    Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    Vec4(const std::vector<float>& data);

    Vec4 operator+(const Vec4& other) const {
        return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    Vec4 operator*(float scalar) const {
        return Vec4(x * scalar, y * scalar, z*scalar, w*scalar);
    }

    // Return the length of the vector
    float length() const {
        return std::sqrt(x * x + y * y + z*z + w*w);
    }

    bool isZero() {
        return (x == 0.f && y == 0.f && z == 0.f && w == 0.f);
    }

    glm::vec4 toGlm() const {
        return glm::vec4(x, y, z, w);
    }

    float x;
    float y;
    float z;
    float w;
};

class IVec4 {
public:
	IVec4() : x(0), y(0), z(0), w(0) {}

	IVec4(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}

	IVec4(const std::vector<int>& data);

	IVec4 operator+(const IVec4& other) const {
		return IVec4(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	IVec4 operator*(float scalar) const {
		return IVec4(x * scalar, y * scalar, z*scalar, w*scalar);
	}

	// Return the length of the vector
	float length() const {
		return std::sqrt(x * x + y * y + z*z + w*w);
	}

	bool isZero() {
		return (x == 0.f && y == 0.f && z == 0.f && w == 0.f);
	}

	glm::ivec4 toGlm() const {
		return glm::ivec4(x, y, z, w);
	}

	int x;
	int y;
	int z;
	int w;
};



class Color {
public:
	Color() : r(0.f), g(0.f), b(0.f), a(0.f) {}

    Color(float red, float green, float blue, float alpha) : r(red), g(green), b(blue), a(alpha) {}

    Color(const std::string& hex);

    glm::vec4 toGlm() const {
        return glm::vec4(r,g,b,a);
    }
    float r;
    float g;
    float b;
    float a;


};
