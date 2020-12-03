#version 330 core

out vec4 col_out;

uniform vec3 lightColor = vec3(1.0f);

void main()
{
	col_out = vec4(lightColor, 1.0f);
}