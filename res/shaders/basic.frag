#version 330 core

in vec4 col;
in vec2 tex;
in vec3 norm;
in vec3 fragPos;

out vec4 col_out;

uniform sampler2D texture_diffuse1;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
	// Ambient light
	vec3 ambientColor = vec3(1.0f, 1.0f, 1.0f);
	float ambientStrength = 0.1f;
	vec3 ambientLight = ambientColor * ambientStrength;

	vec3 normN = normalize(norm);

	// Diffuse light
	vec3 lightVec = normalize(lightPos - fragPos);
	vec3 diffuseLight = max(dot(normN, lightVec), 0.0f) * lightColor;

	// Specular light
	float specularStrength = 0.5f;
	vec3 viewVec = normalize(viewPos - fragPos);
	vec3 reflectVec = reflect(-lightVec, normN);
	vec3 specularLight = pow(max(dot(viewVec, reflectVec), 0.0f), 128) * lightColor * specularStrength;

	// Combine
	col_out = vec4(ambientLight + diffuseLight + specularLight, 1.0f) * (texture(texture_diffuse1, tex) + col);
}