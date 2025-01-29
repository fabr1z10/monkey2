R"(
#version 430 core

uniform sampler2DArray textureArray;

in vec2 TexCoords;
in flat int TexIndex;

out vec4 FragColor;

void main()
{
    //FragColor = vec4(TexCoords, 0, 1); //1,TexIndex,TexIndex,1);// texture(textureArray, vec3(TexCoords, 0));
    FragColor = texture(textureArray, vec3(TexCoords, 0));
}

)"