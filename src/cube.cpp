#include "vertex.h"
#include <glad/glad.h>
#include <stdio.h>
#include "shader.h"

uint32_t vbo, vao, ebo;

// A cube.
Vertex cube_data[] = {
    glm::vec3(-1, -1, -1),
    glm::vec3(1, -1, -1),
    glm::vec3(1, 1, -1),
    glm::vec3(-1, 1, -1),
    glm::vec3(-1, -1, 1),
    glm::vec3(1, -1, 1),
    glm::vec3(1, 1, 1),
    glm::vec3(-1, 1, 1)
};

int indices[6 * 6] = {
    0, 1, 3, 3, 1, 2,
    1, 5, 2, 2, 5, 6,
    5, 4, 6, 6, 4, 7,
    4, 0, 7, 7, 0, 3,
    3, 2, 7, 7, 2, 6,
    4, 5, 0, 0, 5, 1
};

bool prepare_cube(){
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    //Vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_data), cube_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
    glEnableVertexAttribArray(0);

    //indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
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

bool draw_cube(Shader& shader, const glm::mat4& proj){
    glBindVertexArray(vao);
    shader.use();

    glm::mat4 view(1.0), model(1.0);
    view = glm::translate(view, glm::vec3(0.0, -0.0, -7.0));
    view = glm::rotate(view, glm::radians(20.0f), glm::vec3(1.0, 0.0, 0.0));

    shader.set_mat4("MVP", proj * view * model);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    GLenum err;
    bool ok = true;
    while((err = glGetError()) != GL_NO_ERROR){
        fprintf(stderr, "Opengl Error: %d\n", err);
        ok = false;
    }

    glBindVertexArray(0);
    return ok;
};
