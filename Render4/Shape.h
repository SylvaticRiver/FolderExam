#pragma once

#include "VBO.h"
#include "Mesh.h"

class Shape
{
public:
	enum shapeType {
		CUBE,
		PYRAMID,
		OCTAHEDRON,
		SPHERE
	};

	std::vector<Vertex> vert;
	std::vector<GLuint> inds;

	Shape(shapeType type);
	std::vector<Vertex>& getVerts();
	std::vector<GLuint>& getInds();
	void subDivide(glm::vec3 a, glm::vec3 b, glm::vec3 c, int n);
	void createNewTriangle(glm::vec3& a, glm::vec3& b, glm::vec3& c);
	void makeSphere(int subdivisions);
};

