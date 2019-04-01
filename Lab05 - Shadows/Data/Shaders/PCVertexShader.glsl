#version 430

in layout(location = 0) vec4 vertexPositionModel;
in layout(location = 1) vec3 vertexColor;
in layout(location = 2) vec3 vertexNormal;

out vec3 vertexToFragmentColor;

uniform mat4 modelToProjectionMatrix;

void main()
{
	gl_Position = modelToProjectionMatrix * vertexPositionModel;
	vertexToFragmentColor = vertexColor;
}