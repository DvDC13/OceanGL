#version 450

const vec3 vertices[] = {

    // Back face
    vec3( 200000.0, -20000.0, -20000.0), // Bottom left
    vec3(-20000.0, -20000.0, -20000.0), // Bottom right
    vec3(-20000.0,  20000.0, -20000.0), // Top right
    vec3( 20000.0,  20000.0, -20000.0), // Top left
    
    // // Front face
    // vec3(-2000.0, -2000.0,  2000.0), // Bottom left
    // vec3( 2000.0, -2000.0,  2000.0), // Bottom right
    // vec3( 2000.0,  2000.0,  2000.0), // Top right
    // vec3(-2000.0,  2000.0,  2000.0), // Top left

    // // Right face
    // vec3( 2000.0, -2000.0,  2000.0), // Bottom left
    // vec3( 2000.0, -2000.0, -2000.0), // Bottom right
    // vec3( 2000.0,  2000.0, -2000.0), // Top right
    // vec3( 2000.0,  2000.0,  2000.0), // Top left

    // // Left face
    // vec3(-2000.0, -2000.0, -2000.0), // Bottom left
    // vec3(-2000.0, -2000.0,  2000.0), // Bottom right
    // vec3(-2000.0,  2000.0,  2000.0), // Top right
    // vec3(-2000.0,  2000.0, -2000.0), // Top left

    // // Top face
    // vec3(-2000.0,  2000.0, -2000.0), // Bottom left
    // vec3( 2000.0,  2000.0, -2000.0), // Bottom right
    // vec3( 2000.0,  2000.0,  2000.0), // Top right
    // vec3(-2000.0,  2000.0,  2000.0), // Top left

    // // Bottom face
    // vec3(-2000.0, -2000.0, -2000.0), // Bottom left
    // vec3( 2000.0, -2000.0, -2000.0), // Bottom right
    // vec3( 2000.0, -2000.0,  2000.0), // Top right
    // vec3(-2000.0, -2000.0,  2000.0)  // Top left
};

void main()
{
    gl_Position = vec4(vertices[gl_VertexID], 1.0);
}