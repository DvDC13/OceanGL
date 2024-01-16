#version 450

layout(quads, equal_spacing, ccw) in;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;
uniform int numberOfWaves;

uniform float amplitude;
uniform float frequency;
uniform float amplitude_attenuation;
uniform float frequency_amplification;

uniform vec3 cameraPosition;

out vec3 normal;
out vec3 worldPosition;

vec2 get_wave_height(vec2 position, vec2 direction, float frequency, float amplitude)
{
    position -= vec2(1.0, 1.0);
    float proj = dot(position, direction);
    float height = amplitude * (exp(sin(proj * frequency + time) - 1.0) - (1 + exp(-2))/2);
    float derivative = height * cos(proj * frequency + time);
    return vec2(height, -derivative);
}

float get_height_from_position(vec2 position)
{
    float height = 0.0;

    float random = 1.0;

    float amplitude = amplitude;
    float frequency = frequency;

    for(int i = 0; i < numberOfWaves; i++)
    {
        vec2 direction = normalize(vec2(sin(random), cos(random)));
        amplitude *= amplitude_attenuation;
        frequency *= frequency_amplification;
        vec2 wave_parameters = get_wave_height(position, direction, frequency, amplitude);

        height += wave_parameters.x;
        position += direction * wave_parameters.y;

        random += 1229.0;
    }

    return height;
}

vec3 get_normal(vec3 position, float epsilon)
{
    float h = get_height_from_position(position.xz);
    float h_dx = get_height_from_position(position.xz - vec2(epsilon, 0.0));
    float h_dz = get_height_from_position(position.xz + vec2(0.0, epsilon));

    vec3 normal = cross(vec3(epsilon, h - h_dx, 0.0), vec3(0.0, h - h_dz, -epsilon));

    return normalize(normal);
}

void main()
{
    vec4 p1 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
    vec4 p2 = mix(gl_in[3].gl_Position, gl_in[2].gl_Position, gl_TessCoord.x);
    vec4 p = mix(p1, p2, gl_TessCoord.y);
    
    float height = get_height_from_position(p.xz);

	p += vec4 (0.0, height, 0.0, 0.0);

    normal = get_normal(p.xyz, 0.1);

    gl_Position = projection * view * model * p;
    
    worldPosition = p.xyz;
}