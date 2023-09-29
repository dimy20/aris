#include <glm/glm.hpp>
#pragma once

struct Vertex{
    Vertex(const glm::vec3& pos) : mPosition(pos) {};
    glm::vec3 mPosition;
};
