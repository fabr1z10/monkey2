#include "vec.h"
#include "error.h"

Vec2::Vec2(const std::vector<float>& data) {
    M_Assert(data.size() >=2, "Vec2 requires at least 2 components");
    x = data[0];
    y = data[1];
}

Vec3::Vec3(const std::vector<float>& data) {
    M_Assert(data.size() >=2, "Vec3 requires at least 2 components");
    x = data[0];
    y = data[1];
    z = data.size() > 2 ? data[2] : 0.f;
}

Vec4::Vec4(const std::vector<float>& data) {
    M_Assert(data.size() >= 4, "Vec4 requires at least 4 components");
    x = data[0];
    y = data[1];
    z = data[2];
    w = data[3];

}



Color::Color(const std::string& str) {

    unsigned int red = std::stoul(str.substr(1, 2), nullptr, 16);
    unsigned int green = std::stoul(str.substr(3, 2), nullptr, 16);
    unsigned int blue = std::stoul(str.substr(5, 2), nullptr, 16);
    r = red / 255.f;
    g = green / 255.f;
    b = blue / 255.f;
    a = 1.f;
}

