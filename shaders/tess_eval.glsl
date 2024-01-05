#version 450

layout(quads, equal_spacing, ccw) in;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;


void main()
{
    vec4 p1 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
    vec4 p2 = mix(gl_in[3].gl_Position, gl_in[2].gl_Position, gl_TessCoord.x);
    vec4 p = mix(p1, p2, gl_TessCoord.y);
	p += vec4 (0, sin((p.z + time) * 3.0) / 10, 0.0, 0.0);

	vec3 v1 = vec3(1.0, 0.0, 0.0);
	vec3 v2 = vec3(0.0, 3.0/10.0 * cos((p.z + time) * 3.0), 1.0);

	vec3 normal = -normalize(cross(v1,v2));

    gl_Position = projection * view * model * p;
}