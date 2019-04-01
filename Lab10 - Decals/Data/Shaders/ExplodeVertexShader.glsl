#version 430 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoords;

out VS_OUT {
    vec2 texCoords;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * position; 
    vs_out.texCoords = texCoords;
}