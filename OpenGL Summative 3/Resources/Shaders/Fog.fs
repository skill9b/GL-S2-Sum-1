#version 450 core

in vec3 fragColor;
in vec2 fragTexCoord;
in vec4 mWorldPos;
out vec4 color;

uniform float currentTime;
uniform sampler2D tex;
uniform sampler2D tex2;

uniform vec3 camPos;
float d = distance(mWorldPos.xyz, camPos);
float lerp = (d - 5.0f) / 50.f;
vec4 vFogColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);

void main()
{
    lerp = clamp(lerp, 0.0, 1.0);
    color = mix(texture(tex, fragTexCoord), vFogColor, lerp);
}
