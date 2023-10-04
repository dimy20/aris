#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "input.h"
#include <cstring>
#include <stdio.h>

#define MAX_KEYS 1024
static bool keyboard[MAX_KEYS];
static double mouse_x;
static double mouse_y;

void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < MAX_KEYS){
        if (action == GLFW_PRESS){
            keyboard[key] = true;
        }else if (action == GLFW_RELEASE){
            keyboard[key] = false;
        }
    }
}

void Input::framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void Input::init(){
    memset(keyboard, 0, sizeof(bool) * MAX_KEYS);
    mouse_x = 0.0;
    mouse_y = 0.0;
};

bool Input::is_key_pressed(int key){ 
    if(key < 0 || key >= MAX_KEYS) return false;
    return keyboard[key];
}

void Input::cursor_position_callback(GLFWwindow* window, double x, double y) {
    mouse_x = x;
    mouse_y = y;
};

double Input::get_mouse_x(){ return mouse_x; }
double Input::get_mouse_y() { return mouse_y; };
