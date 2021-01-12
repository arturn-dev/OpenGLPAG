#version 330 core

layout (location = 1) in vec3 pos_in;
layout (location = 2) in vec3 norm_in;

uniform mat4 model = mat4(1.0f);
uniform mat4 view = mat4(1.0f);
uniform mat4 proj = mat4(1.0f);
uniform mat4 normModel = mat4(1.0f);

out vec3 norm;
out vec3 fragPos;

void main()
{
	gl_Position = proj * view * model * vec4(pos_in, 1.0f);
	fragPos = vec3(model * vec4(pos_in, 1.0f));

	norm = mat3(normModel) * norm_in;
}