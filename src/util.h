#pragma once

#include <iostream>
#include "glm/glm.hpp"


// Overload the << operator for glm::vec2/3/4
std::ostream& operator<<(std::ostream& os, const glm::vec2& vec) ;

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec) ;


std::ostream& operator<<(std::ostream& os, const glm::vec4& vec) ;
