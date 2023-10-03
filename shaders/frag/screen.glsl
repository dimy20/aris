#version 450 core

out vec4 frag_color;
uniform bool drawing_lines;

void main(){
    frag_color = drawing_lines ? vec4(1.0, 0, 0, 1.0) : vec4(0.6, 0.8, 0.5, 1.0);
};
