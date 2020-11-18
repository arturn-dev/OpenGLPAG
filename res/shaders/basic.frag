#version 330 core

in vec4 col;
in vec2 tex;

out vec4 col_out;

uniform sampler2D texture_diffuse1;
uniform vec4 userColor;

void main()
{
	col_out = texture(texture_diffuse1, tex) + col;
}