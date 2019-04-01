#version 430

in layout(location = 0) vec4 vertexPositionModel;
in layout(location = 1) vec2 vertexUV;

out vec2 UV;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform float time;

void main()
{
	vec4 pos = vertexPositionModel;
	pos.y = sin( pos.x + time )  *  cos( pos.z + time ) * .5f;
	gl_Position = projection * view * model * pos;
	vec2 tex = vertexUV;
	tex.x -= time * 0.005f;
	tex.y -= time * 0.005f;
	UV = tex;
}