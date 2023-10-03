#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 block_pos; // per instance

uniform mat4 MVP;

void main(){
    vec3 final_pos = position + block_pos;
    //gl_Position = proj * view * model * vec4(position.xyz, 1.0);
    gl_Position = MVP * vec4(final_pos, 1.0);
};
