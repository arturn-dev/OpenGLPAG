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

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float cutOffDeg;
	LightColors lightColors;
};

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 viewPos;
uniform DirLight dirLight;
#define MAX_POINT_LIGHTS_COUNT 10
uniform PointLight pointLights[MAX_POINT_LIGHTS_COUNT];
#define MAX_SPOT_LIGHTS_COUNT 10
uniform SpotLight spotLights[MAX_SPOT_LIGHTS_COUNT];

vec3 normN = normalize(norm);
vec3 viewVec = normalize(viewPos - fragPos);
vec3 diffuseColor = texture(texture_diffuse1, tex).rgb + col.rgb;
vec3 specularColor = texture(texture_specular1, tex).rgb + col.rgb;

const vec3 ambientColor = vec3(1.0f, 1.0f, 1.0f);
const float ambientStrength = 0.5f;
const float specularStrength = 1.0f;
const float shininness = 32.0f;
const float pointLightConstant = 1.0f;
const float pointLightLinear = 0.09f;
const float pointLightQuadratic = 0.032f;
const float spotLightInnerAngleDegDiff = 5.0f;

vec3 calcHalfwayVec(vec3 lightPos)
{
	vec3 lightVec = normalize(lightPos - fragPos);
	vec3 halfwayVec = normalize(lightVec + viewVec);

	return halfwayVec;
}

///////////////////////////////////////////
// Directional light calculation functions.
///////////////////////////////////////////

vec3 dirLight_calcDiffLight(DirLight _dirLight)
{
	vec3 lightVec = normalize(-_dirLight.direction);
	vec3 diffuseLight = max(dot(normN, lightVec), 0.0f) * _dirLight.lightColors.diffuse;

	return diffuseLight * diffuseColor;
}

vec3 dirLight_calcSpecLight(DirLight _dirLight)
{
	vec3 lightVec = normalize(-_dirLight.direction);
	vec3 reflectVec = reflect(-lightVec, normN);
	vec3 specularLight = pow(max(dot(viewVec, reflectVec), 0.0f), shininness) * _dirLight.lightColors.specular * specularStrength;

	return specularLight * specularColor;
}

vec3 dirLightCalc(DirLight _dirLight)
{
	vec3 outputLight = dirLight_calcDiffLight(_dirLight) + dirLight_calcSpecLight(_dirLight);

	return outputLight;
};

/////////////////////////////////////
// Point light calculation functions.
/////////////////////////////////////

vec3 pointLight_calcDiffLight(PointLight _pointLight)
{
	vec3 lightVec = normalize(_pointLight.position - fragPos);
	vec3 diffuseLight = max(dot(normN, lightVec), 0.0f) * _pointLight.lightColors.diffuse;

	return diffuseLight * diffuseColor;
}

vec3 pointLight_calcSpecLight(PointLight _pointLight)
{
	vec3 halfwayVec = calcHalfwayVec(_pointLight.position);
	vec3 specularLight = pow(max(dot(normN, halfwayVec), 0.0f), shininness) * _pointLight.lightColors.specular * specularStrength;

	return specularLight * specularColor;
}

vec3 pointLightCalc(PointLight _pointLight)
{
	float d = distance(_pointLight.position, fragPos);
	float attenuation = 1.0f / (pointLightConstant + pointLightLinear * d + pointLightQuadratic * d * d);

	vec3 outputLight = pointLight_calcDiffLight(_pointLight) + pointLight_calcSpecLight(_pointLight);

	return outputLight * attenuation;
}

/////////////////////////////////////
// Spot light calculation functions.
/////////////////////////////////////

vec3 spotLight_calcDiffLight(SpotLight _spotLight)
{
	vec3 lightVec = normalize(_spotLight.position - fragPos);
	vec3 diffuseLight = max(dot(normN, lightVec), 0.0f) * _spotLight.lightColors.diffuse;

	return diffuseLight * diffuseColor;
}

vec3 spotLight_calcSpecLight(SpotLight _spotLight)
{
	vec3 halfwayVec = calcHalfwayVec(_spotLight.position);
	vec3 specularLight = pow(max(dot(normN, halfwayVec), 0.0f), shininness) * _spotLight.lightColors.specular * specularStrength;

	return specularLight * specularColor;
}

vec3 spotLightCalc(SpotLight _spotLight)
{
	vec3 outputLight = vec3(0.0f);
	vec3 lightVec = normalize(_spotLight.position - fragPos);
	float lightVecCos = dot(lightVec, normalize(-_spotLight.direction));
	float cutOffCos = cos(radians(_spotLight.cutOffDeg));
	if (lightVecCos > cutOffCos)
	{
		outputLight += spotLight_calcDiffLight(_spotLight) + spotLight_calcSpecLight(_spotLight);

		// Calculate intensity
		float i = clamp((lightVecCos - cutOffCos) / (cos(radians(_spotLight.cutOffDeg - spotLightInnerAngleDegDiff)) - cutOffCos), 0.0f, 1.0f);

		// Calculate attenuation
		float d = distance(_spotLight.position, fragPos);
		float attenuation = 1.0f / (pointLightConstant + pointLightLinear * d + pointLightQuadratic * d * d);

		outputLight *= i * attenuation;
	}

	return outputLight;
}

//////////////
// Main method
//////////////
void main()
{
	// Ambient light	
	vec3 ambientLight = diffuseColor * ambientColor * ambientStrength;

	// Lights computation
	vec3 outputLight = dirLightCalc(dirLight);
	
	int i;
	for(i = 0; i < MAX_POINT_LIGHTS_COUNT; i++)
	{
		outputLight +=  pointLightCalc(pointLights[i]);
	}

	for(i = 0; i < MAX_SPOT_LIGHTS_COUNT; i++)
	{
		outputLight += spotLightCalc(spotLights[i]);
	}

	// Combine
	col_out = vec4(ambientLight + outputLight, 1.0f);
}