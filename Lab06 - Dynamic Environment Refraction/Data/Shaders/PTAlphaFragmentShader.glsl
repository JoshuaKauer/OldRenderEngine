#version 430
 
in vec2 UV;
 
out vec4 color;
 
uniform sampler2D myTexture;
 
void main()
{
    color = vec4(texture(myTexture, UV).rgb, 0.5);
}