#version 430

in layout(location=0) vec4 vertexPositionModel;
in layout(location=1) vec3 vertexColor;
in layout(location=2) vec3 normalModel;

uniform mat4 modelToWorldMatrix;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 normalWorld;
out vec3 vertexPositionWorld;

void main()
{
	gl_Position = projection * view * model * vertexPositionModel;
	normalWorld = vec3(model * vec4(normalModel, 0));
	vertexPositionWorld = vec3(model * vertexPositionModel);
}