R"(
#version 330 core

in vec3 Normal;   // Interpolated normal from the vertex shader
in vec4 Color;    // Interpolated vertex color

uniform vec3 uLightDir;    // Direction of the light (normalized)
uniform vec3 uLightColor;  // Color of the light
uniform float uAmbient;

out vec4 FragColor;

void main() {
    // Normalize the normal
    vec3 normal = normalize(Normal);

    // Calculate diffuse lighting
    float diff = max(dot(-normalize(uLightDir), normal), 0.0);

    // Calculate ambient lighting
    vec3 ambient = vec3(uAmbient) * vec3(Color);

    // Calculate diffuse lighting
    vec3 diffuse = diff * uLightColor * vec3(Color);

    // Combine ambient and diffuse lighting
    vec3 finalColor = ambient + diffuse;

    // Output the final color, preserving the alpha from the input color
    FragColor = vec4(finalColor, Color.a);
}

)"