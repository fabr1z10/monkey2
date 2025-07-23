R"(
#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec4 vColor;
layout (location = 2) in int vHide;

uniform mat4 pv_mat;

out vec4 Color;

void main() {
    if (vHide == 1) {
        gl_Position = vec4(2.0, 2.0, 2.0, 1.0); // outside clip space
    } else {
        gl_Position = pv_mat * vec4(vPosition, 1);
        Color = vColor;
    }
}

)"