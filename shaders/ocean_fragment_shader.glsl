#version 450

out vec4 FragColor;

in vec3 normal;
in vec3 worldPosition;

uniform vec3 cameraPosition;
uniform vec3 sunDirection;
uniform vec3 shallowColor;
uniform vec3 deepColor;

const float M_PI = 3.14159265359;

// Color constants for a much darker ocean
const vec3 skyColor = vec3(0.65, 0.80, 0.95);
const vec3 sunAmbient = vec3(0.02, 0.02, 0.05); // Even darker ambient light
const vec3 sunDiffuse = vec3(0.5, 0.5, 0.8); // Much less intense diffuse light
const vec3 sunColor = vec3(0.4, 0.4, 0.4); // Sun color

void main()
{
    vec3 N = normalize(normal);
    vec3 V = normalize(cameraPosition - worldPosition);
    vec3 S = normalize(sunDirection);

    // Ambient term
    vec3 ambient = sunAmbient;

    // Diffuse term
    float diff = max(dot(N, -S), 0.0);
	vec3 diffuse = sunDiffuse * diff;

    // Height-based color blending between shallow and deep water
    float relativeHeight = clamp(worldPosition.y, 0.0, 1.0);
    vec3 heightColor = mix(shallowColor, deepColor, relativeHeight);

    // Reflection term
    float refCoeff = pow(max(dot(N, V), 0.0), 1.0);
	vec3 reflectColor = (1 - refCoeff) * skyColor;

    // Reflected light from the sun
    vec3 reflectDir = reflect(S, N); 

    // Sun Color Effect
    float SdotReflectedCoeff = pow(max(dot(S, reflectDir), 0.0), 10);
    vec3 sunColorEffect = sunColor * SdotReflectedCoeff;

    // Fresnel term
    float fresnel = pow(1.0 - max(dot(N, V), 0.0), 3.0);
    fresnel = mix(0.1, 1.0, fresnel);
    reflectColor *= fresnel;

    // Surface more reflective at grazing angles
    float theta = dot(N, V);
    float reflectance = 0.5 + 0.5 * pow(1.0 - theta, 5.0);
    reflectColor = mix(reflectColor, sunColorEffect, reflectance);

    // Combine color components
    vec3 color = ambient + diffuse + heightColor + reflectColor + sunColorEffect;

    // Set the fragment color
    FragColor = vec4(color, 1.0);
}