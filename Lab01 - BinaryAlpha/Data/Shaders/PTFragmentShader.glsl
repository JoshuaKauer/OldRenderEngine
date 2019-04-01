#version 430
 
in vec2 UV;
 
out vec3 color;
 
uniform sampler2D myTexture;
 
void main()
{
	vec3 tmp = texture(myTexture, UV).rgb;

	if(tmp == vec3(1.0, 1.0, 1.0))
		discard;
		 
    color = tmp;
}