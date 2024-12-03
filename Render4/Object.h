#ifndef OBJECT_CLASS_H
#define OBJECT_CLASS_H

#include<string>
#include <iostream>
#include <vector>
#include "glad/glad.h"

#include"Camera.h"
#include "Mesh.h"

struct Rotator {
	glm::vec3 Axis;
	float amount;
};

class Object
{
public:
	Mesh mesh;
	glm::vec3 currPos;
	glm::vec3 prevPos;
	glm::vec3 size;
	Rotator rotation;

	glm::vec3 velocity;
	bool hasGravity;

	Object();
	Object(Mesh& mesh, glm::vec3 startPos, glm::vec3 size, Rotator rotation, Shader& shaderProgram);

	void Render(Shader& shader, Camera& camera, bool dotRender);
	void setPos(glm::vec3 newPos);
	void move(glm::vec3 amount);
	void rescale(glm::vec3 newSize);
	void rotate(float degree, glm::vec3 direction);
	void update(Shader& shaderProgram);
	float getBarHeight(Triangle triangle, glm::vec3 bars);
	glm::vec3 barycentricCoordinates(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	static std::vector<Triangle> getTriangles(vector<Vertex> vertices, vector<GLuint> indices);
	virtual void updateMovement();
};

class LightObject : public Object {
public:
	glm::vec4 lightcolor;
	float lightStrenght;
	LightObject(Mesh& mesh, glm::vec3 startPos, glm::vec3 size, Rotator rotation, glm::vec4 lightColor, float lightStrenght, Shader& lightShader, Shader& shaderProgram);

};

#endif
