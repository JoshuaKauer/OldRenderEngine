#version 430

in vec3 vertexToFragmentColor;
in vec2 UV;

out vec3 daColor;

uniform sampler2D myTexture;

void main()
{
	daColor = vertexToFragmentColor * texture( myTexture, UV ).rgb;
}