#version 430

in layout(location = 0) vec4 vertexPositionModel;
in layout(location = 1) vec3 vertexColor;
in layout(location = 2) vec3 vertexNormal;

out vec3 vertexToFragmentColor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	gl_Position = projection * view * model * vertexPositionModel;
	vertexToFragmentColor = vertexColor;
}