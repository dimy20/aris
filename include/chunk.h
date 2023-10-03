#pragma once

#include <cstdint>
#include "shader.h"
#include <vector>

#define CHUNK_SIZE 2 // Chunk size across all three dimensions (x, y, z)
#define CHUNK_X_AXIS CHUNK_SIZE
#define CHUNK_Y_AXIS CHUNK_SIZE
#define CHUNK_Z_AXIS CHUNK_SIZE
#define NUM_BLOCKS CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE
#define BLOCK_NUM_INDICES 36 // 36 indices per block
#define BLOCK_NUM_VERTICES 8 // 8 vertices per block

enum BlockType{
    EMPTY = 0,
    GRASS = 1
};

struct Block{
    Block() : type(BlockType::GRASS) {};
    BlockType type;
};

struct Chunk{
    Chunk();
    ~Chunk();

    bool draw(Shader& shader, const glm::mat4& proj);
    bool prepare_blocks();
    private:
        bool load_block_vertex_data();
        void init();

    public:
        uint32_t m_vao, m_vbo, m_ebo;
        uint32_t m_instance_vbo;
        Block m_blocks[CHUNK_X_AXIS][CHUNK_Y_AXIS][CHUNK_Z_AXIS];
        std::vector<glm::vec3> m_instance_positions; // block position for each block instance
};
