#version 450

out vec4 FragColor;

uniform float time;

const float M_PI = 3.14159265359;
const float dayDuration = 24.0;

vec3 sunDirection = normalize(vec3(0.0, -5.0, 5.0));
vec3 sunColor = vec3(1.0, 1.0, 0.8);
float sunIntensity = 1.0;

void main()
{    
    float normalizedTime = fract(time / dayDuration);

    float dayNightCycle = sin(normalizedTime * 2.0 * M_PI) * 0.5 + 0.5;

    vec3 dayColor = vec3(0.4, 0.7, 1.0);
    vec3 nightColor = vec3(0.05, 0.05, 0.2);

    vec3 skyColor = mix(nightColor, dayColor, smoothstep(0.2, 0.8, dayNightCycle));

     FragColor = vec4(skyColor, 1.0);
}