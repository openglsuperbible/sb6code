#version 420 core

out vec4 color;

layout (binding = 0) uniform sampler2D tex_color;
layout (binding = 1) uniform sampler2D tex_normal;

in VS_OUT
{
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;

    vec3 ws_pos;
    vec4 color;
    vec2 texcoord;
} fs_in;

uniform vec3 light_pos = vec3(0.0, 3.0, 10.0);

void main(void)
{
    vec3 L = normalize(light_pos - fs_in.ws_pos);

    float x = dot(L, normalize(fs_in.tangent));
    float y = dot(L, normalize(fs_in.bitangent));
    float z = dot(L, normalize(fs_in.normal));

    vec3 V = normalize(vec3(x, y, z));
    vec3 bump = vec3(0.0, -1.0, 0.0); // texture(tex_normal, fs_in.texcoord).rgb;
    bump = normalize(bump * 2.0 - vec3(1.0));

    float diffuse = max( dot(L, bump), 0.0 );
    float specular = pow(dot(reflect(-L, bump), V), 6.0) * 0.3;

    // color = texture(tex_color, fs_in.texcoord);
    // color = vec4(x, y, z, 0.0);
    // color = vec4(bump, 1.0);
    // color = vec4(L, 1.0);
    // color = vec4( fs_in.ws_pos, 1.0 );
    color = vec4(abs(specular)) + abs(diffuse) * texture(tex_color, fs_in.texcoord);
}
