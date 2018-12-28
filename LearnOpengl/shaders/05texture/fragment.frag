#version 430 core

in vec4 outColor;
out vec4 fragColor;

in vec2 TexCoord;

uniform sampler2D tex;

void main()
{
	fragColor = texture(tex, TexCoord) * outColor;
}