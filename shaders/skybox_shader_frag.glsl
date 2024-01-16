#version 450

out vec4 FragColor;

const float M_PI = 3.14159265359;

vec3 sunDirection = normalize(vec3(3.0, 2.0, -5.0));
vec3 sunColor = vec3(1.0, 1.0, 0.8);
float sunIntensity = 1.0;

in vec3 worldPosition;

uniform vec3 cameraPosition;

void main()
{
    vec3 V = normalize(cameraPosition - worldPosition);
    vec3 dayColor = vec3(0.4, 0.7, 1.0);
    float VdotSun = max(0.0, pow(dot(-sunDirection, V), 256));
    
    vec3 skyColor = vec3(0.0, 0.0, 0.0);
    skyColor += sunColor * VdotSun * sunIntensity;
    skyColor += dayColor * (1.0 - VdotSun);
    
    // = dayColor + sunColor * sunIntensity;
    FragColor = vec4(skyColor, 1.0);
}