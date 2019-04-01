#version 430
 
in vec2 UV;
 
out vec4 color;
 
uniform sampler2D myTexture;
 
void main()
{
	vec3 tmp = texture(myTexture, UV).rgb;

	if(tmp == vec3(1.0, 1.0, 1.0))
		discard;
		 
    color = vec4(tmp, 1.0);
}