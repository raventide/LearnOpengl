#version 430 core

out vec4 fragColor;

in vec4 TriColor;
in vec2 TexCoord;

uniform sampler2D tex;

void main()
{
	fragColor = texture(tex,TexCoord);
}
