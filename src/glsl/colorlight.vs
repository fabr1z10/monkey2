R"(
#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec3 vNormal;

uniform mat4 pv_mat;

out vec4 Color;
out vec3 Normal;

void main() {
    gl_Position = pv_mat * vec4(vPosition, 1);
    Color = vColor;
    Normal = vNormal;
}

)"