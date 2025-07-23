R"(
#version 430 core

uniform usampler2D texture_pdiffuse1;
uniform sampler2D texture_palette;  // where palette is stored as RGBA8

in vec2 TexCoords;
in flat int TexIndex;
//in vec4 Color;

out vec4 FragColor;

void main()
{
	uint index = texture(texture_pdiffuse1, TexCoords).r;
	vec4 texColor = texelFetch(texture_palette, ivec2(int(index), TexIndex), 0);
	if (texColor.a < 0.5) {
    	discard;
    }
    FragColor = texColor;
    //FragColor =  vec4(float(index)/255.0, 0, 0, 1); //vec4(1,1,1,1);//texColor;
}

)"