#pragma once

#include <cstdint>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "chunk.h"

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

struct Game{
    Game(uint32_t width, uint32_t height);
    ~Game();

    bool init();
    void process_input();
    void update();
    bool draw();

    GameState m_state;
    uint32_t m_width;
    uint32_t m_height;

    Shader mScreenShader;
    glm::mat4 mProj;
    Chunk m_chunk;
};
