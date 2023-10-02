#version 410 core

layout (location = 0) in vec4 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform mat4 MVP;

void main(){
    //gl_Position = proj * view * model * vec4(position.xyz, 1.0);
    gl_Position = MVP * vec4(position.xyz, 1.0);
};
