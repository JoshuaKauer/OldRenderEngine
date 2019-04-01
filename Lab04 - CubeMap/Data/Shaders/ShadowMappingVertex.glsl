#version 430

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;

out VS_OUT {
    vec3 FragPos;
	vec2 TexCoords;
    vec3 Normal;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * position;
    vs_out.FragPos = vec3(model * position);
    vs_out.TexCoords = texCoords;
    vs_out.Normal = transpose(inverse(mat3(model))) * normal;
}