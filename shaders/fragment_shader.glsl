#version 450

out vec4 FragColor;

in vec3 normal;
in vec3 worldPosition;

uniform vec3 cameraPosition;

const float M_PI = 3.14159265359;

uniform vec3 lightPos;

void main()
{    
    vec3 L = normalize(lightPos - worldPosition);
    vec3 N = normalize(normal);
    vec3 V = normalize(cameraPosition - worldPosition);

    vec3 ambientFactor = vec3(0.0);
    vec3 diffuseFactor = vec3(1.0);

    vec3 skyColor = vec3(0.65, 0.80, 0.95);

    vec3 lightAmbient = vec3(1.0, 1.0, 1.0);
    vec3 ambient = ambientFactor * lightAmbient;

    vec3 shallowColor = vec3(0.0, 0.64, 0.68);
	vec3 deepColor = vec3(0.02, 0.05, 0.1);

    float relativeHeight = worldPosition.y;
    vec3 heightColor = mix(shallowColor, deepColor, relativeHeight);

    float sprayThresholdUpper = 1.0;
	float sprayThresholdLower = 0.9;
	float sprayRatio = 0;
    if (relativeHeight > sprayThresholdLower) sprayRatio = (relativeHeight - sprayThresholdLower) / (sprayThresholdUpper - sprayThresholdLower);
	vec3 sprayBaseColor = vec3(1.0);
	vec3 sprayColor = sprayRatio * sprayBaseColor;

    vec3 lightDiffuse = vec3(1.0, 1.0, 1.0);
    float diff = max(dot(N, L), 0.0);
	vec3 diffuse = diffuseFactor * lightDiffuse * diff;

    float refCoeff = pow(max(dot(N, V), 0.0), 1.0);
	vec3 reflectColor = (1 - refCoeff) * skyColor;

    vec3 lightSpecular = vec3(1.0, 0.9, 0.7);
    vec3 reflectDir = reflect(-L, N); 
	float specCoeff = pow(max(dot(V, reflectDir), 0.0), 128);
    vec3 specular = lightSpecular * specCoeff;

    vec3 combinedColor = ambient + diffuse + heightColor + reflectColor;   

    specCoeff = clamp(specCoeff, 0, 1);
	combinedColor *= (1 - specCoeff);
	combinedColor += specular;
	vec3 color = combinedColor;

    FragColor.rgba = vec4(color, 1.0);
}