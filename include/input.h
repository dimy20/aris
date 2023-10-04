#pragma once

#include <GLFW/glfw3.h>
namespace Input{
    enum Key{
        A = GLFW_KEY_A,
        W = GLFW_KEY_W,
        S = GLFW_KEY_S,
        D = GLFW_KEY_D,
        SHIFT = GLFW_KEY_LEFT_SHIFT,
        MOUSE_0 = GLFW_MOUSE_BUTTON_1,
        MOUSE_1 =     GLFW_MOUSE_BUTTON_2,
        MOUSE_2 =     GLFW_MOUSE_BUTTON_3,
        MOUSE_3 =     GLFW_MOUSE_BUTTON_4,
        MOUSE_4 =     GLFW_MOUSE_BUTTON_5,
        MOUSE_5 =     GLFW_MOUSE_BUTTON_6,
        MOUSE_6 =     GLFW_MOUSE_BUTTON_7,
        MOUSE_7 =     GLFW_MOUSE_BUTTON_8,
    };

    void init();
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void cursor_position_callback(GLFWwindow* window, double x, double y);
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    bool is_key_pressed(int key);
    bool is_mouse_button_pressed(int button);
    double get_mouse_x();
    double get_mouse_y();
};
