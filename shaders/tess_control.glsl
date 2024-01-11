#version 450

layout (vertices = 4) out;

void main()
{
    gl_TessLevelInner[0] = 4096.0;
    gl_TessLevelInner[1] = 4096.0;

    gl_TessLevelOuter[0] = 4096.0;
    gl_TessLevelOuter[1] = 4096.0;
    gl_TessLevelOuter[2] = 4096.0;
    gl_TessLevelOuter[3] = 4096.0;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}