#version 430

in layout(location = 0) vec4 vertexPositionModel;
in layout(location = 1) vec2 vertexUV;

out vec2 UV;

uniform mat4 modelToProjectionMatrix;
uniform float time;

void main()
{
	vec4 pos = vertexPositionModel;
	pos.y = sin( pos.x + time )  *  cos( pos.z + time );// * .5f;
	gl_Position = modelToProjectionMatrix * pos;
	vec2 tex = vertexUV;
	tex.x += time * 0.01f;
	tex.y -= time * 0.01f;
	UV = tex;
}