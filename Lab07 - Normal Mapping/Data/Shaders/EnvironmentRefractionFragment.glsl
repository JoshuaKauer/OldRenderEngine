#version 430 core

in vec3 Normal;
in vec3 Position;
out vec4 color;

uniform vec3 cameraPos;
uniform samplerCube environment;

//Reflect
//void main()
//{             
//    vec3 I = normalize(Position - cameraPos);
//    vec3 R = reflect(I, normalize(Normal));
//    color = texture(environment, R);
//}

//Refract
void main()
{             
    float ratio = 1.00 / 1.01;
    vec3 I = normalize(Position - cameraPos);
    vec3 R = refract(I, normalize(Normal), ratio);
    color = texture(environment, R);
} 