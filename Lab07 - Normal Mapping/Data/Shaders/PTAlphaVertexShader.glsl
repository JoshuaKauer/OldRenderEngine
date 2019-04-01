#version 430

in layout(location = 0) vec4 position;
in layout(location = 1) vec2 vertexUV;
 
out vec2 UV;
 
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
 
void main(){
 
    gl_Position =  projection * view * model * position;
 
    UV = vertexUV;
}