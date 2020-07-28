#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 fragTexCoord;
out vec3 fragPos;
out vec3 fragNormal;

uniform mat4 PVM;
uniform mat3 normcalc;

void main()
{
	gl_Position = PVM * vec4(position, 1.0);
	fragPos = vec3(PVM * vec4(position, 1.0));
	fragNormal = normal;
	fragTexCoord = texCoords;
}

