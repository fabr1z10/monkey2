#pragma once

#include <iostream>
#include "vec.h"


// Overload the << operator for glm::vec2/3/4
std::ostream& operator<<(std::ostream& os, const glm::vec2& vec) ;
std::ostream& operator<<(std::ostream& os, const Vec2& vec) ;
std::ostream& operator<<(std::ostream& os, const IVec2& vec) ;

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec) ;
std::ostream& operator<<(std::ostream& os, const Vec3& vec) ;


std::ostream& operator<<(std::ostream& os, const glm::vec4& vec) ;
std::ostream& operator<<(std::ostream& os, const Vec4& vec) ;
std::u32string getString32(const std::string&);

uint32_t packRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

void unpackRGBA(uint32_t color, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a);