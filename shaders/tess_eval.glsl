#version 450

layout(quads, equal_spacing, ccw) in;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;
uniform int numberOfWaves;

out vec3 normal;
out vec3 worldPosition;

float get_wave_height(vec2 position, vec2 direction, float frequency, float amplitude)
{
    position += vec2(1.0, 1.0);
    float proj = dot(position, direction);
    return amplitude * exp(sin(proj * frequency + time) - 1.0);
}

float get_height_from_position(vec2 position)
{
    float height = 0;
    float amplitude = 0.05;
    float frequency = 3.0;
    float amplitude_attenuation = 0.85;
    float frequency_amplification = 1.15;

    float random = 1.0;

    for(int i = 0; i < numberOfWaves; i++)
    {
        amplitude *= amplitude_attenuation;
        frequency *= frequency_amplification;
        height += get_wave_height(position, normalize(vec2(sin(random), cos(random))), frequency, amplitude);

        random += 1229.0;
    }

    return height;
}

vec3 get_normal(vec3 position, float epsilon)
{
    float h = get_height_from_position(position.xz);
    float h_dx = get_height_from_position(position.xz - vec2(epsilon, 0.0));
    float h_dz = get_height_from_position(position.xz + vec2(epsilon, 0.0));

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

    normal = get_normal(p.xyz, 0.01);

    gl_Position = projection * view * model * p;
    
    worldPosition = p.xyz;
}