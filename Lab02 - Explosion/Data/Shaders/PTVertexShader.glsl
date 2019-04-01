#version 430

in layout(location = 0) vec4 position;
in layout(location = 1) vec2 vertexUV;
 
out vec2 UV;
 
uniform mat4 modelToProjectionMatrix;
 
void main(){
 
    gl_Position =  modelToProjectionMatrix * position;
 
    UV = vertexUV;
}