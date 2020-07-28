#version 450 core

in vec3 fragNormal;
in vec3 fragPos;
in vec2 fragTexCoord;

out vec4 color;

uniform samplerCube skybox;
uniform vec3 camPos;

float	ambientStr		= 0.15f;
vec3	ambientColor	= vec3(1.0f, 1.0f, 1.0f);
vec3	lightColour		= vec3(1.0f, 1.0f, 1.0f);
vec3	lightPos		= vec3(0.0f, 30.0f, 100.0f);
int		rimExponent		= 1;
vec3	rimColour		= vec3(1.0f, 0.0f, 0.0f);

float	lightSpecStr	= 1.0f;
float	shininess		= 32.0f;

void main()
{
	vec3 norm = normalize(fragNormal);
	vec3 viewDir = normalize(fragPos - camPos);
	vec3 reflectDir = reflect(viewDir, norm);

	// Ambient
	vec3 ambient = ambientStr * ambientColor;

	// Light Direction
	vec3 lightDir = normalize(fragPos - lightPos);

	// Diffuse Colouring
	float diffuseStr = max(dot(norm, -lightDir), 0.0f);
	vec3 diffuse = diffuseStr * lightColour;

	vec3 halfwayVec = normalize(-lightDir + (-viewDir));
	float spec = pow(max(dot(norm, halfwayVec), 0.0f), shininess);

	vec3 specular = lightSpecStr * spec * lightColour;

	// Rim Lighting
	float rimFactor = 1.0 - dot(norm, (-viewDir));	// Calculate the rim factor
	rimFactor = smoothstep(0.0f, 1.0f, rimFactor);	// Constrain it to the exponent
	rimFactor = pow(rimFactor, rimExponent);		// Raise it to the rim exponent
	vec3 rim = rimFactor * rimColour * lightColour;	//

	//color = vec4( vec4(ambient + diffuse + specular + rim, 1.0f) * vec4(texture(skybox, reflectDir).rgb, 1.0f) );
	color = color = vec4( vec4(ambient + diffuse, 1.0f) * vec4(texture(skybox, reflectDir).rgb, 1.0f) );
}
