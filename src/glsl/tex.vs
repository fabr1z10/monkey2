R"(
#version 430 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoords;
layout (location = 2) in int vTexIndex;

uniform mat4 pv_mat;

out vec2 TexCoords;
out flat int TexIndex;

void main() {
    // calculate position
    gl_Position = pv_mat * vec4(vPosition, 1);

    TexCoords = vTexCoords;

    TexIndex = vTexIndex;
}

)"