#pragma once

#define CHUNK_SIZE 1
#define CHUNK_XYZ_SIZE CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE
#include <cstdint>
#include "shader.h"

struct Chunk{
    Chunk();
    ~Chunk();
    bool init();
    uint8_t get(int x, int y, int z);
    void set(int x, int y, int z, uint8_t type);
    bool update();
    bool render(Shader& shader, const glm::mat4& proj);

    uint8_t mBlocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
    uint32_t mVao, mEbo, mVbo;
    int mElements;
    bool mChanged;
};
