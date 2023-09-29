#pragma once
#include "vertex.h"
#include "shader.h"


struct Rotation{
    Rotation(const glm::vec3& axis, float rads):
        mAxis(axis), mRads(rads) {};

    static Rotation defaultRotation();

    glm::vec3 mAxis;
    float mRads;
};

// The model transform
struct Transform{
    Transform() : 
        mPosition(glm::vec3(0.0)), 
        mScale(glm::vec3(1.0)), 
        mRotation(glm::vec3(0.0, 0.0, 1.0), 0.0){};

    Transform(const glm::vec3& pos, const glm::vec3& scale, const Rotation& rot) :
        mPosition(pos), mScale(scale), mRotation(rot) {};

    glm::mat4 getModelMatrix() const;

    glm::vec3 mPosition;
    glm::vec3 mScale;
    Rotation mRotation;
};

struct Triangle{
    Triangle() {};
    bool init();
    bool draw(Shader& shader, const glm::mat4& proj);

    uint32_t mVao;
    uint32_t mVbo;
    Transform mTransform;
};
