#version 330 core

in vec3 tex;

uniform samplerCube skybox;

out vec4 col_out;

void main()
{
	col_out = texture(skybox, tex);
}