R"(
#version 430 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoords;
layout (location = 2) in int vTexIndex;
layout (location = 3) in vec4 vColor;

uniform mat4 pv_mat;

out vec2 TexCoords;
out flat int TexIndex;
out vec4 Color;

void main() {
    // calculate position
    gl_Position = pv_mat * vec4(vPosition, 1);

    TexCoords = vTexCoords;
    Color = vColor;

    TexIndex = vTexIndex;
}

)"