#include "triangle.h"
#include <cstdio>
#include <glad/glad.h>

glm::mat4 getViewMatrix(){
    glm::mat4 view(1.0);
    view = glm::translate(view, glm::vec3(0.0, -0, -2.0));
    return view;
}

Vertex triangle_data[3] ={
    Vertex(glm::vec3(-0.5, -0.5, 0.0)),
    Vertex(glm::vec3(0.0, 0.5, 0.0)),
    Vertex(glm::vec3(0.5, -0.5, 0.0)),
};

bool Triangle::init(){
    glGenBuffers(1, &mVbo);
    glGenVertexArrays(1, &mVao);

    glBindVertexArray(mVao);

    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_data), triangle_data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        fprintf(stderr, "Opengl Error: %d\n", error);
        return false;
    }

    return true;
};

bool Triangle::draw(Shader& shader, const glm::mat4& proj){
    glBindVertexArray(mVao);
    shader.use();

    glm::mat4 MVP = proj * getViewMatrix() * mTransform.getModelMatrix();
    shader.set_mat4("MVP", MVP);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        fprintf(stderr, "Opengl Error: %d\n", error);
        return false;
        // Handle or log the error
    }
    return true;

};

glm::mat4 Transform::getModelMatrix() const {
    glm::mat4 model(1.0);
    model = glm::translate(model, mPosition);
    model = glm::rotate(model, mRotation.mRads, mRotation.mAxis);
    model = glm::scale(model, mScale);
    return model;
};
