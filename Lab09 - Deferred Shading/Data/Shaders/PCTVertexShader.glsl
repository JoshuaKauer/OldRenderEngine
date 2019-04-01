#version 430

in layout(location = 0) vec4 vertexPositionModel;
in layout(location = 1) vec3 vertexColor;
in layout(location = 2) vec2 vertexUV;

out vec3 vertexToFragmentColor;
out vec2 UV;

uniform mat4 modelToProjectionMatrix;

void main()
{
	gl_Position = modelToProjectionMatrix * vertexPositionModel;
	vertexToFragmentColor = vertexColor;
	UV = vertexUV;
}