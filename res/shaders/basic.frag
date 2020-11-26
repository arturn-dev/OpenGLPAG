#version 330 core

in vec4 col;
in vec2 tex;

out vec4 col_out;

uniform sampler2D texture_diffuse1;
uniform vec3 lightColor;

void main()
{
	col_out = vec4(lightColor, 1.0f) * (texture(texture_diffuse1, tex) + col);
}