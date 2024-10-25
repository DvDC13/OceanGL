#version 450

layout (vertices = 4) out;

void main()
{
    gl_TessLevelInner[0] = 8192.0;
    gl_TessLevelInner[1] = 8192.0;

    gl_TessLevelOuter[0] = 8192.0;
    gl_TessLevelOuter[1] = 8192.0;
    gl_TessLevelOuter[2] = 8192.0;
    gl_TessLevelOuter[3] = 8192.0;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}