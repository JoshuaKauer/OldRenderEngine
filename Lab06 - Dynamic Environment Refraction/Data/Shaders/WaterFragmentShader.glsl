#version 430

in vec2 UV;

out vec4 daColor;

uniform sampler2D myTexture;

void main()
{
	daColor = texture( myTexture, UV );
	daColor.a = 0.7;
}