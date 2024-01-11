#version 450

out vec4 FragColor;

in vec3 normal;
in vec3 worldPosition;

const float PI = 3.14159265359;

vec3 lightPos = vec3(5.0, 5.0, 5.0);
vec3 lightColor = vec3(1.0, 1.0, 1.0);
float intensity = 1.0;

void main()
{
    vec3 sea_color = vec3(0.2, 0.2, 0.9);
    
    vec3 L = normalize(lightPos - worldPosition);
    vec3 N = normalize(normal);

    float lambertTerm = dot(N, L);

    FragColor = vec4(sea_color * (lambertTerm * lightColor * intensity), 1.0);
}