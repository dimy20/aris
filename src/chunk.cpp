#include <cglm/mat4.h>
#include <cstring>
#include <glad/glad.h>
#include <assert.h>
#include <glm/glm.hpp>
#include <cstdio>
#include <vector>
#include "chunk.h"
#include <cglm/cglm.h>

#define OPENGL_ERR_CHECK(ok) do{                 \
    GLenum err;                                  \
    while((err = glGetError()) != GL_NO_ERROR){  \
        fprintf(stderr, "Error: %d\n", err);     \
        ok = false;                              \
    }                                            \
}while(0);

typedef glm::tvec3<uint8_t> blockPos;

static int indices[6 * 6] = {
    0, 1, 3, 3, 1, 2,
    1, 5, 2, 2, 5, 6,
    5, 4, 6, 6, 4, 7,
    4, 0, 7, 7, 0, 3,
    3, 2, 7, 7, 2, 6,
    4, 5, 0, 0, 5, 1
};

static glm::vec3 block_data[] = {
    glm::vec3(-1, -1, -1),
    glm::vec3(1, -1, -1),
    glm::vec3(1, 1, -1),
    glm::vec3(-1, 1, -1),
    glm::vec3(-1, -1, 1),
    glm::vec3(1, -1, 1), glm::vec3(1, 1, 1),
    glm::vec3(-1, 1, 1)
};

void Chunk::init(){
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glGenBuffers(1, &m_instance_vbo);
};

bool Chunk::prepare_blocks(){
    glBindVertexArray(m_vao);

    m_instance_positions.clear();

    for(size_t x = 0; x < CHUNK_X_AXIS; x++){
        for(size_t y = 0; y < CHUNK_Y_AXIS; y++){
            for(size_t z = 0; z < CHUNK_Z_AXIS; z++){
                Block& b = m_blocks[x][y][z];
                if(b.type == BlockType::EMPTY){
                    continue;
                }
                m_instance_positions.push_back(glm::vec3(x, y, z));
            };
        }
    }
    size_t n = m_instance_positions.size();
    glBindBuffer(GL_ARRAY_BUFFER, m_instance_vbo);

    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(glm::vec3) * n,
                 &m_instance_positions[0],
                 GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);

    glBindVertexArray(0);

    bool ok = true;
    OPENGL_ERR_CHECK(ok);
    return ok;
};

bool Chunk::load_block_vertex_data(){
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glGenVertexArrays(1, &m_vao);

    glBindVertexArray(m_vao);

    //Vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(block_data), block_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
    glEnableVertexAttribArray(0);

    //indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    GLenum err;
    bool ok = true;
    while((err = glGetError()) != GL_NO_ERROR){
        fprintf(stderr, "Opengl Error: %d\n", err);
        ok = false;
    }

    glBindVertexArray(0);
    return ok;
};

Chunk::Chunk(){
    init();

    if(!load_block_vertex_data()){
        exit(1);
    }
}

Chunk::~Chunk(){
    //glDeleteBuffers(1, &m_vbo);
    //glDeleteBuffers(1, &m_instance_vbo);
    //glDeleteBuffers(1, &m_ebo);
    //glDeleteVertexArrays(1, &m_vao);
}

bool Chunk::draw(Shader& shader, const glm::mat4& vp){
    glBindVertexArray(m_vao);
    shader.use();

    glm::mat4 model(1.0);
    shader.set_mat4("MVP", vp * model);

    size_t num_instances = m_instance_positions.size();

    glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, num_instances);

    //change color to draw lines
    shader.set_bool("drawing_lines", true);
    glLineWidth(2.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, num_instances);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    shader.set_bool("drawing_lines", false);

    GLenum err;
    bool ok = true;
    while((err = glGetError()) != GL_NO_ERROR){
        fprintf(stderr, "Opengl Error: %d\n", err);
        ok = false;
    }
    glBindVertexArray(0);
    return ok;
};
