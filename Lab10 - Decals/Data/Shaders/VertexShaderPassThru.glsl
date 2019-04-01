#version 430

in layout(location = 0) vec4 vertexPositionModel;
in layout(location = 1) vec3 vertexColor;
in layout(location = 2) vec2 vertexUV;

out vec3 vertexToFragmentColor;
out vec2 UV;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 positionOffset;

void main()
{
	gl_Position = projection * view * model * (vertexPositionModel + vec4(positionOffset, 1));
	vertexToFragmentColor = vertexColor;
	UV = vertexUV;
}