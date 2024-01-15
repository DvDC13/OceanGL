#version 450

out vec4 FragColor;

in vec3 normal;
in vec3 worldPosition;

uniform vec3 cameraPosition;
uniform vec3 lightPos;

const float M_PI = 3.14159265359;

// Color constants
const vec3 skyColor = vec3(0.65, 0.80, 0.95);
const vec3 lightAmbient = vec3(0.05, 0.05, 0.1); // Darker ambient light
const vec3 lightDiffuse = vec3(0.8, 0.8, 1.0); // Less intense diffuse light
const vec3 lightSpecular = vec3(0.9, 0.9, 1.0); // Subtle specular highlight
const vec3 shallowColor = vec3(0.0, 0.22, 0.30); // Darker shallow water color
const vec3 deepColor = vec3(0.005, 0.015, 0.03); // Darker deep water color

void main()
{    
    vec3 L = normalize(lightPos - worldPosition);
    vec3 N = normalize(normal);
    vec3 V = normalize(cameraPosition - worldPosition);

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

    // Combine color components
    vec3 color = ambient + diffuse + heightColor + reflectColor;
	color += specular * specCoeff;

    // Set the fragment color
    FragColor = vec4(color, 1.0);
}