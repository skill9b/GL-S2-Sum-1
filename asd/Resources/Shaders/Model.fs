#version 450 core
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoord;

out vec4 color;

uniform sampler2D texture_diffuse1;
float	ambientStr		= 0.05f;
vec3	ambientColor	= vec3(1.0f, 1.0f, 1.0f);
vec3	lightColour		= vec3(1.0f, 1.0f, 1.0f);
vec3	lightPos		= vec3(0.0f, 30.0f, 100.0f);
float	rimExponent		= 32.0f;
vec3	rimColour		= vec3(1.0f, 1.0f, 1.0f);
uniform vec3 camPos;
float	lightSpecStr	= 1.0f;
float	shininess		= 32.0f;

void main()
{
	// Ambient
	vec3 ambient = ambientStr * ambientColor;

	// Light Direction
	vec3 norm = normalize(fragNormal);
	vec3 lightDir = normalize(fragPos - lightPos);

	// Diffuse Colouring
	float diffuseStr = max(dot(norm, -lightDir), 0.0f);
	vec3 diffuse = diffuseStr * lightColour;

	// Specular Highlight
	vec3 negViewDir = normalize(camPos - fragPos);

	vec3 halfwayVec = normalize(-lightDir + negViewDir);
	float spec = pow(max(dot(norm, halfwayVec), 0.0f), shininess);

	vec3 specular = lightSpecStr * spec * lightColour * 2.0f;

	color = vec4(ambient + diffuse + specular, 1.0f) * vec4(texture(texture_diffuse1, fragTexCoord));
}

