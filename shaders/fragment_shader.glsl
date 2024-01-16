#version 450

out vec4 FragColor;

in vec3 normal;
in vec3 worldPosition;

uniform vec3 cameraPosition;
uniform vec3 lightPos;
uniform vec3 sunDirection;

const float M_PI = 3.14159265359;

// Color constants for a much darker ocean
const vec3 skyColor = vec3(0.65, 0.80, 0.95);
const vec3 lightAmbient = vec3(0.02, 0.02, 0.05); // Even darker ambient light
const vec3 lightDiffuse = vec3(0.5, 0.5, 0.8); // Much less intense diffuse light
const vec3 lightSpecular = vec3(0.7, 0.7, 0.9); // Reduced specular highlight
const vec3 shallowColor = vec3(0.0, 0.1, 0.2); // Darker shallow water color
const vec3 deepColor = vec3(0.0, 0.05, 0.1); // Very dark deep water color
const vec3 sunColor = vec3(0.4, 0.4, 0.4); // Sun color

void main()
{
    vec3 L = normalize(lightPos - worldPosition);
    vec3 N = normalize(normal);
    vec3 V = normalize(cameraPosition - worldPosition);
    vec3 S = normalize(sunDirection);

    // Ambient term
    vec3 ambient = lightAmbient;

    // Diffuse term
    float diff = max(dot(N, L), 0.0);
	vec3 diffuse = lightDiffuse * diff;

    // Height-based color blending between shallow and deep water
    float relativeHeight = clamp(worldPosition.y, 0.0, 1.0);
    vec3 heightColor = mix(shallowColor, deepColor, relativeHeight);

    // Reflection term
    float refCoeff = pow(max(dot(N, V), 0.0), 1.0);
	vec3 reflectColor = (1 - refCoeff) * skyColor;

    // Specular term
    vec3 reflectDir = reflect(-L, N); 
	float specCoeff = pow(max(dot(V, reflectDir), 0.0), 128);
    vec3 specular = lightSpecular * specCoeff;

    // Sun Color
    float SdotReflectedCoeff = pow(max(dot(S, reflectDir), 0.0), 10);
    vec3 sunColor = sunColor * SdotReflectedCoeff;

    // Combine color components
    vec3 color = ambient + diffuse + heightColor + reflectColor + sunColor;
	color += specular * specCoeff;

    // Set the fragment color
    FragColor = vec4(color, 1.0);
}