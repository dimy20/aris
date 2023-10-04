#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "game.h"
#include "input.h"

#define SCREEN_W 1024
#define SCREEN_H 768


#define LOG_ERR(s) fprintf(stderr, "Error: %s\n", s);

GLFWwindow* window;
bool running = true;
float delta_time;
float prev_time;

static void update_time();

static void glfw_error_callback(int error, const char* description){
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
};

static void GLAPIENTRY gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                              GLsizei length, const GLchar* message, const void* userParam){
    std::cout << "OpenGL debug message: " << message << std::endl;
};

static bool init_glfw_and_opengl(){
    if(!glfwInit()){
        LOG_ERR("Failed to initialize glfw");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    window = glfwCreateWindow(SCREEN_W, SCREEN_H, "Breakout", NULL, NULL);
    if(!window){
        LOG_ERR("Failed to create window");
        return false;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        LOG_ERR("Failed to initialize GLAD");
        return false;
    }

    glfwSetErrorCallback(glfw_error_callback);

    glViewport(0, 0, SCREEN_W, SCREEN_H);
    glDebugMessageCallback(gl_debug_callback, NULL);

    glfwSetKeyCallback(window, Input::key_callback);
    glfwSetCursorPosCallback(window, Input::cursor_position_callback);
    glfwSetFramebufferSizeCallback(window, Input::framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);

    return true;
};

void clear(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
};

int main(){
    if(!init_glfw_and_opengl()){
        exit(1);
    }
    Game game(SCREEN_W, SCREEN_H);

    if(!game.init()){
        exit(1);
    }

    prev_time = glfwGetTime();

    while(!glfwWindowShouldClose(window)){
        update_time();

        glfwPollEvents();

        game.process_input(delta_time);

        clear();

        if(!game.draw()){
            break;
        }

        glfwSwapBuffers(window);
    };

    glfwTerminate();

    return 0;
};

void update_time(){
    float curr_time = glfwGetTime();
    delta_time = curr_time - prev_time;
    prev_time = curr_time;
};
