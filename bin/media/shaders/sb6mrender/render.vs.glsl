#version 420 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec2 texcoord;

out VS_OUT
{
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;

    vec3 ws_pos;
    vec4 color;
    vec2 texcoord;
} vs_out;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void)
{
    gl_Position = proj_matrix * mv_matrix * position;
    vs_out.normal = mat3(mv_matrix) * normal;
    vs_out.tangent = mat3(mv_matrix) * tangent;
    vs_out.bitangent = mat3(mv_matrix) * bitangent;

    vs_out.ws_pos = (mv_matrix * position).xyz;
    vs_out.color = position * 2.0 + vec4(0.5, 0.5, 0.5, 0.0);
    vs_out.texcoord = texcoord;
}
