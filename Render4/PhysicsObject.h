#pragma once
#include "Object.h"

class PhysicsObject :
    public Object
{
public:

    bool hasGravity;
    float mass;
    Triangle lastCollided;

    PhysicsObject();
    PhysicsObject(Mesh& mesh, glm::vec3 startPos, glm::vec3 size, Rotator rotation, Shader& shaderProgram, bool hasGravity, float mass);

    void updateMovement() override;
    bool isColliding(Object object);
    void onCollisionWall(Object object);
    bool isCollidingBall(Object* object);
    void rollOnsurface(Triangle triangle);
    void onBallCollision(PhysicsObject* object);
};

