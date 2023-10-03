#include <glm/glm.hpp>
#include <glad/glad.h>
#include <iostream>
#include <vector>

struct Square{
    uint32_t mVao, mVbo, mEbo;
    uint32_t m_instance_vbo;
    bool changed = true;
};

Square s;

#include "game.h"
#include "triangle.h"
#include "chunk.h"

Vertex data [] = {
    Vertex(glm::vec3(-1, -1, 0)),
    Vertex(glm::vec3(-1, 1, 0)),
    Vertex(glm::vec3(-0.8, -1, 0)),
    Vertex(glm::vec3(-0.8, 1, 0)),
};

static uint32_t indices [] = {
    0, 1, 2,
    2, 1, 3
};

static glm::mat4 getViewMatrix(){
    glm::mat4 view(1.0);
    view = glm::translate(view, glm::vec3(0.0, -0, -2.0));
    return view;
}

bool square_prepare_instance_vbo(size_t n){
    std::vector<float> offsets;
    float off = -1.0;
    for(size_t i = 0; i < n; i++){
        off += 0.6;
        offsets.push_back(off);
    };
    glGenBuffers(1, &s.m_instance_vbo);

    glBindVertexArray(s.mVao);

    glBindBuffer(GL_ARRAY_BUFFER, s.m_instance_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * n, offsets.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);

    glBindVertexArray(0);

    GLenum err;
    bool ok = true;
    while((err = glGetError()) != GL_NO_ERROR){
        fprintf(stderr, "Error: %d\n", err);
        ok = false;
    }
    return ok;
};

bool square_draw_many_v2(Shader& shader, const glm::mat4& proj, size_t n){
    if(s.changed){
        if(!square_prepare_instance_vbo(n)){
            return false;
        }
        s.changed = false;
    }

    glBindVertexArray(s.mVao);
    shader.use();
    glm::mat4 mvp = proj * getViewMatrix() * glm::mat4(1.0);
    shader.set_mat4("MVP", mvp);

    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, n);
    glBindVertexArray(0);

    GLenum err;
    bool ok = true;
    while((err = glGetError()) != GL_NO_ERROR){
        fprintf(stderr, "Error: %d\n", err);
        ok = false;
    }
    return ok;
};

bool square_draw_many(Shader& shader, const glm::mat4& proj, size_t n){
    shader.use();
    glBindVertexArray(s.mVao);
    float offset = -0.3;
    for(int i = 0; i < n; i++){
        glm::mat4 model(1.0);
        model = glm::translate(model, glm::vec3(offset, 0, 0));
        glm::mat4 mvp = proj * getViewMatrix() * model;

        shader.set_mat4("MVP", mvp);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        offset += 0.6;
    };
    glBindVertexArray(0);

    GLenum err;
    bool ok = true;
    while((err = glGetError()) != GL_NO_ERROR){
        fprintf(stderr, "Error: %d\n", err);
        ok = false;
    }
    return ok;

};

bool square_draw(Shader& shader, const glm::mat4& proj){
    shader.use();
    glBindVertexArray(s.mVao);

    glm::mat4 mvp = proj * getViewMatrix() * glm::mat4(1.0);
    shader.set_mat4("MVP", mvp);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    return true;
};

bool square_init(){
    glGenBuffers(1, &s.mVbo);
    glGenBuffers(1, &s.mEbo);
    glGenVertexArrays(1, &s.mVao);

    glBindVertexArray(s.mVao);

    glBindBuffer(GL_ARRAY_BUFFER, s.mVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s.mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    return true;
};


Game::Game(uint32_t width, uint32_t height)
    : m_width(width),
    m_height(height)
{
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    mProj = glm::perspective(45.0f, aspect, 0.1f, 100.0f);
};

bool Game::init(){
    if(!mScreenShader.load("shaders/vertex/screen.glsl", "shaders/frag/screen.glsl")){
        return false;
    }
    return true;
};

bool Game::draw(){
    if(!m_chunk.prepare_blocks()){
        fprintf(stderr, "Failed to prepare blocks!\n");
        return false;
    }
    if(!m_chunk.draw(mScreenShader, mProj)){
        return false;
    }
    return true;
};

Game::~Game() {};
