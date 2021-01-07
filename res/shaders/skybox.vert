#version 330 core

layout (location = 1) in vec3 pos_in;

uniform mat4 view = mat4(1.0f);
uniform mat4 proj = mat4(1.0f);

out vec3 tex;

void main()
{
	gl_Position = proj * view * vec4(pos_in, 1.0f);
	
	tex = pos_in;
}