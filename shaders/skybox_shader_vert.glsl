#version 450

const vec3 vertices[] = {

    // Back face
    vec3( 30.0, -20.0, -20.0), // Bottom left
    vec3(-30.0, -20.0, -20.0), // Bottom right
    vec3(-30.0,  20.0, -20.0), // Top right
    vec3( 30.0,  20.0, -20.0), // Top left
};

void main()
{
    gl_Position = vec4(vertices[gl_VertexID], 1.0);
}