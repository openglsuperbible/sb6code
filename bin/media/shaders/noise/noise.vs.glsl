#version 420 core

out vec2 tc;

void main(void)
{
    const vec4 vertices[] = vec4[](vec4(-1.0, -1.0, 0.5, 1.0),
                                   vec4( 1.0, -1.0, 0.5, 1.0),
                                   vec4(-1.0,  1.0, 0.5, 1.0),
                                   vec4( 1.0,  1.0, 0.5, 1.0));

    tc = vertices[gl_VertexID].xy;
    gl_Position = vertices[gl_VertexID];
}
