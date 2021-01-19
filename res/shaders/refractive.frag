#version 330 core

in vec3 norm;
in vec3 fragPos;

uniform vec3 viewPos;
uniform samplerCube skybox;

out vec4 col_out;

void main() {
	float ratio = 1.0 / 1.2;
	vec3 viewVec = normalize(fragPos - viewPos);
	vec3 refractedVector = refract(viewVec, normalize(norm), ratio);

	col_out = vec4(texture(skybox, refractedVector).rgb, 1.0f);
}