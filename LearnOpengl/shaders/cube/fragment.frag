#version 430 core

out vec4 fragColor;

in vec2 TexCoord;
in vec4 TriColor;

uniform sampler2D tex;

void main()
{
	fragColor = TriColor;
}
