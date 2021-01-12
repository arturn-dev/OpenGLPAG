#version 330 core

in vec3 norm;
in vec3 fragPos;

uniform vec3 viewPos;
uniform samplerCube skybox;

out vec4 col_out;

void main() {
	vec3 viewVec = normalize(fragPos - viewPos);
	vec3 reflectedVector = reflect(viewVec, normalize(norm));

	col_out = vec4(texture(skybox, reflectedVector).rgb, 1.0f);
}