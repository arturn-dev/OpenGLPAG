#version 330 core

layout (location = 1) in vec3 pos_in;
layout (location = 2) in vec4 col_in;
layout (location = 3) in vec2 tex_in;
layout (location = 4) in vec3 norm_in;

uniform mat4 model = mat4(1.0f);
uniform mat4 view = mat4(1.0f);
uniform mat4 proj = mat4(1.0f);

out vec4 col;
out vec2 tex;

void main()
{
	vec3 norm = norm_in + vec3(0.1f);
	gl_Position = proj * view * model * vec4(pos_in, 1.0f);
	col = col_in;
	tex = tex_in;
}