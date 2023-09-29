#include <glm/ext/matrix_projection.hpp>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <iostream>

#include "game.h"
#include "triangle.h"

Triangle t;

Game::Game(uint32_t width, uint32_t height)
    : m_width(width),
    m_height(height)
{
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    mProj = glm::perspective(45.0f, aspect, 0.1f, 100.0f);
};

bool Game::init(){
    if(!m_screen_shader.load("shaders/vertex/screen.glsl", "shaders/frag/screen.glsl")){
        return false;
    }
    if(!t.init()) return false;

    auto& r = t.mTransform.mRotation;
    r.mRads = glm::radians(-85.0f);
    r.mAxis = glm::vec3(1.0, 0.0, 0.0);

    return true;
};

bool Game::draw(){
    if(!t.draw(m_screen_shader, mProj)) return false;
    return true;
};

Game::~Game() {};
