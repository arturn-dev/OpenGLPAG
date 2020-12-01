#version 330 core

in vec4 col;
in vec2 tex;
in vec3 norm;
in vec3 fragPos;

out vec4 col_out;

struct LightColors
{
	vec3 diffuse;
	vec3 specular;
};

struct DirLight
{
	vec3 direction;
	LightColors lightColors;
};

struct PointLight
{
	vec3 position;
	LightColors lightColors;
};

uniform sampler2D texture_diffuse1;
uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLight;

vec3 normN = normalize(norm);
vec3 viewVec = normalize(viewPos - fragPos);

const vec3 ambientColor = vec3(1.0f, 1.0f, 1.0f);
const float ambientStrength = 0.1f;
const float specularStrength = 0.5f;
const float pointLightConstant = 1.0f;
const float pointLightLinear = 0.09f;
const float pointLightQuadratic = 0.032f;

///////////////////////////////////////////
// Directional light calculation functions.
///////////////////////////////////////////

vec3 dirLight_calcDiffLight(DirLight _dirLight)
{
	vec3 lightVec = normalize(-_dirLight.direction);
	vec3 diffuseLight = max(dot(normN, lightVec), 0.0f) * _dirLight.lightColors.diffuse;

	return diffuseLight;
}

vec3 dirLight_calcSpecLight(DirLight _dirLight)
{
	vec3 lightVec = normalize(-_dirLight.direction);
	vec3 reflectVec = reflect(-lightVec, normN);
	vec3 specularLight = pow(max(dot(viewVec, reflectVec), 0.0f), 128) * _dirLight.lightColors.specular * specularStrength;

	return specularLight;
}

vec3 dirLightCalc(DirLight _dirLight)
{
	vec3 outputLight = dirLight_calcDiffLight(_dirLight) + dirLight_calcSpecLight(_dirLight);

	return outputLight;
};

/////////////////////////////////////
// Point light calculation functions.
/////////////////////////////////////

vec3 pointLight_calcDiffLight(PointLight _pointLight, float attenuation)
{
	vec3 lightVec = normalize(_pointLight.position - fragPos);
	vec3 diffuseLight = max(dot(normN, lightVec), 0.0f) * _pointLight.lightColors.diffuse;

	return diffuseLight;
}

vec3 pointLight_calcSpecLight(PointLight _pointLight, float attenuation)
{
	vec3 lightVec = normalize(_pointLight.position - fragPos);
	vec3 reflectVec = reflect(-lightVec, normN);
	vec3 specularLight = pow(max(dot(viewVec, reflectVec), 0.0f), 128) * _pointLight.lightColors.specular * specularStrength;

	return specularLight;
}

vec3 pointLightCalc(PointLight _pointLight)
{
	float d = distance(_pointLight.position, fragPos);
	float attenuation = 1.0f / (pointLightConstant + pointLightLinear * d + pointLightQuadratic * d * d);

	vec3 outputLight = pointLight_calcDiffLight(_pointLight, attenuation) + pointLight_calcSpecLight(_pointLight, attenuation);

	return outputLight;
}

//////////////
// Main method
//////////////
void main()
{
	// Ambient light	
	vec3 ambientLight = ambientColor * ambientStrength;

	// Lights computation
	vec3 outputLight = dirLightCalc(dirLight) + pointLightCalc(pointLight);

	// Combine
	col_out = vec4(ambientLight + outputLight, 1.0f) * (texture(texture_diffuse1, tex) + col);
}