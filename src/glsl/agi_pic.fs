R"(
#version 430 core

uniform usampler2D texture_pdiffuse1;
uniform sampler2D texture_palette;  // where palette is stored as RGBA8
uniform sampler2D uPriorityTex;

in vec2 TexCoords;
in flat int TexIndex;
//in vec4 Color;

out vec4 FragColor;

void main()
{
    //FragColor = vec4(1,1,1,1);
	//return;
	uint index = texture(texture_pdiffuse1, TexCoords).r;
	vec4 texColor = texelFetch(texture_palette, ivec2(int(index), TexIndex), 0);
	//if (texColor.a < 0.5) {
    //	discard;
    //}
    FragColor = texColor;
    //return;
    float gray = texture(uPriorityTex, TexCoords).r;    // get z
    //uint priority = texture(uPriorityTex, TexCoords).r; // assuming grayscale or red channel
    //float z = priority; //1.0 - priority; // invert so higher priority = nearer
    // 0 = near, 1 = far-- > priority = 4 --> 1 , 15 --> 0
    gl_FragDepth = gray; //1.0 - (priority - 3.0)/13.0; // write to depth buffer manually
    //FragColor =  vec4(float(index)/255.0, 0, 0, 1); //vec4(1,1,1,1);//texColor;
}

)"