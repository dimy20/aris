#include <cstring>
#include <glad/glad.h>
#include <assert.h>
#include <glm/glm.hpp>
#include <cstdio>
#include <vector>
#include "chunk.h"

static int indices[6 * 6] = {
    0, 1, 3, 3, 1, 2,
    1, 5, 2, 2, 5, 6,
    5, 4, 6, 6, 4, 7,
    4, 0, 7, 7, 0, 3,
    3, 2, 7, 7, 2, 6,
    4, 5, 0, 0, 5, 1
};

#define BLOCK_NUM_INDICES 36 // 36 indices per block
#define BLOCK_NUM_VERTICES 8 // 8 vertices per block

#define BLOCK_TYPE_EMPTY 0
#define BLOCK_TYPE_GRASS 1

typedef glm::tvec4<GLbyte> byte4;

uint8_t Chunk::get(int x, int y, int z){
    assert(x < CHUNK_SIZE && y < CHUNK_SIZE && z < CHUNK_SIZE);
    return mBlocks[x][y][z];
}

bool Chunk::init(){
    glGenBuffers(1, &mVbo);
    glGenBuffers(1, &mEbo);
    glGenVertexArrays(1, &mVao);
    bool ok = true;
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR){
        fprintf(stderr, "Gl Error: %d\n", err);
        ok = false;
    }
    return ok;
}

void Chunk::set(int x, int y, int z, uint8_t type){
    assert(x < CHUNK_SIZE && y < CHUNK_SIZE && z < CHUNK_SIZE);
    mBlocks[x][y][z] = type;
    mChanged = true;
}

Chunk::Chunk() : mElements(0), mChanged(false){
    memset(mBlocks, BLOCK_TYPE_GRASS, sizeof(mBlocks));
};

Chunk::~Chunk(){
    // change to opengl 4.5
    //glDeleteBuffers(1, &mEbo);
    //glDeleteVertexArrays(1, &mVao);
    //glDeleteBuffers(1, &mVbo);
}

bool Chunk::update(){
    mChanged = true;
    //std::vector<int> v;

    byte4 vertex_data[CHUNK_XYZ_SIZE * BLOCK_NUM_VERTICES];
    //uint32_t indices[CHUNK_XYZ_SIZE * BLOCK_NUM_INDICES];

    // 16 * 16 * 16 = (4096 blocks)
    int i = 0;
    //Build the vertex data for each little block in the chunk and send it to the gpu.
    for(int x = 0; x < CHUNK_SIZE; x++){
        for(int y = 0; y < CHUNK_SIZE; y++){
            for(int z = 0; z < CHUNK_SIZE; z++){
                uint8_t block_type = get(x, y, z);
                if(block_type == BLOCK_TYPE_EMPTY){
                    continue;
                }

                vertex_data[i++] = byte4(x, y, z, block_type);
                vertex_data[i++] = byte4(x + 1, y, z, block_type);
                vertex_data[i++] = byte4(x + 1, y + 1, z, block_type);
                vertex_data[i++] = byte4(x, y + 1, z, block_type);

                vertex_data[i++] = byte4(x, y, z + 1, block_type);
                vertex_data[i++] = byte4(x + 1, y, z + 1, block_type);
                vertex_data[i++] = byte4(x + 1, y + 1, z + 1, block_type);
                vertex_data[i++] = byte4(x, y + 1, z + 1, block_type);

            }
        }
    }
    mElements = i;
    assert(mElements == (CHUNK_XYZ_SIZE * BLOCK_NUM_VERTICES));
    glBindVertexArray(mVao);

    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(byte4) * mElements, vertex_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_BYTE, GL_FALSE, 4, (void *)0);
    glEnableVertexAttribArray(0);

    GLenum err;
    bool ok = true;
    while((err = glGetError()) != GL_NO_ERROR){
        fprintf(stderr, "Gl Error: %d\n", err);
        ok = false;
    }

    glBindVertexArray(0);
    return ok;
};

bool Chunk::render(Shader& shader, const glm::mat4& proj){
    bool ok = true;

    glBindVertexArray(mVao);
    shader.use();

    glm::mat4 view(1.0), model(1.0);
    view = glm::translate(view, glm::vec3(0.0, -0.0, -7.0));
    view = glm::rotate(view, glm::radians(20.0f), glm::vec3(1.0, 0.0, 0.0));
    glm::mat4 MVP = proj * view * model;

    shader.set_mat4("MVP", MVP);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR){
        fprintf(stderr, "Gl Error: %d\n", err);
        ok = false;
    }

    return ok;
};
