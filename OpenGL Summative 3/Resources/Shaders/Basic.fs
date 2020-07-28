#version 450 core

in vec3 fragColor;
in vec2 fragTexCoord;

out vec4 color;

uniform float currentTime;
uniform sampler2D tex;
uniform sampler2D tex2;

void main()
{
	
    color = mix(texture(tex, fragTexCoord), texture(tex2, fragTexCoord), 0.5f * sin(currentTime) + 0.5f);
}
