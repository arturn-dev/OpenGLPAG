#version 330 core

layout (location = 1) in vec3 pos_in;
layout (location = 2) in vec4 col_in;

uniform mat4 proj = mat4(1.0f);
uniform mat4 view = mat4(1.0f);
uniform mat4 model = mat4(1.0f);

out vec4 col;

void main()
{
	gl_Position = vec4(mat3(view) * mat3(model) * pos_in, 1.0f);

	col = col_in;
}