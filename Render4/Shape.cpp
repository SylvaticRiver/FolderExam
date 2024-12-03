#include "Shape.h"

Vertex cubeVerticies[] = {
Vertex{glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.5f, 0.5f)},
Vertex{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.5f, 0.5f)},
Vertex{glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.5f, 0.5f)},
Vertex{glm::vec3(1.0f, -1.0f,  1.0f), glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.5f, 0.5f)},
Vertex{glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.5f, 0.5f)},
Vertex{glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.5f, 0.5f)},
Vertex{glm::vec3(1.0f,  1.0f, -1.0f), glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.5f, 0.5f)},
Vertex{glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.5f, 0.5f)}
};

unsigned int cubeIndices[] = {
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

Vertex vertices[] =
{
	Vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
	Vertex{glm::vec3(0.5f, 0.0f,  0.5f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(0.0f, 0.8f,  0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)}
};

GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

Vertex octa[] = {
	Vertex{glm::vec3(0.0f, 0.0f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(1.0f, 0.0f,  0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(0.0f, 1.0f,  0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.0f,  0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(0.0f, 0.0f,  -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
};

GLuint octInd[] = {
	0, 1, 2,
	0, 1, 4,
	0, 3, 2,
	0, 3, 4,
	5, 1, 2,
	5, 1, 4,
	5, 3, 2,
	5, 3, 4
};

Shape::Shape(shapeType type) {
	if (type == CUBE) {
		std::vector <Vertex> verts(cubeVerticies, cubeVerticies + sizeof(cubeVerticies) / sizeof(Vertex));
		std::vector <GLuint> ind(cubeIndices, cubeIndices + sizeof(cubeIndices) / sizeof(GLuint));
		this->vert = verts;
		this->inds = ind;
	}
	else if (type == PYRAMID) {
		std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
		std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
		this->vert = verts;
		this->inds = ind;
	}
	else if (type == OCTAHEDRON) {
		std::vector <Vertex> verts(octa, octa + sizeof(octa) / sizeof(Vertex));
		std::vector <GLuint> ind(octInd, octInd + sizeof(octInd) / sizeof(GLuint));
		this->vert = verts;
		this->inds = ind;
	}
	else if (type == SPHERE) {
		std::vector <Vertex> verts(octa, octa + sizeof(octa) / sizeof(Vertex));
		std::vector <GLuint> ind(octInd, octInd + sizeof(octInd) / sizeof(GLuint));
		this->vert = verts;
		this->inds = ind;

		makeSphere(4);
	}
}

std::vector<Vertex>& Shape::getVerts()
{
	return this->vert;
}

std::vector<GLuint>& Shape::getInds()
{
	return this->inds;
}

//alle funksjoner under er fra pseudokode i forelesningsnotater kap 8.3, Listing 8.2 -> 8.4
void Shape::makeSphere(int subdivisions) {
	this->inds.clear();
	subDivide(octa[0].pos, octa[1].pos, octa[2].pos, subdivisions);
	subDivide(octa[0].pos, octa[1].pos, octa[4].pos, subdivisions);
	subDivide(octa[0].pos, octa[3].pos, octa[2].pos, subdivisions);
	subDivide(octa[0].pos, octa[3].pos, octa[4].pos, subdivisions);
	subDivide(octa[5].pos, octa[1].pos, octa[2].pos, subdivisions);
	subDivide(octa[5].pos, octa[1].pos, octa[4].pos, subdivisions);
	subDivide(octa[5].pos, octa[3].pos, octa[2].pos, subdivisions);
	subDivide(octa[5].pos, octa[3].pos, octa[4].pos, subdivisions);
}

void Shape::subDivide(glm::vec3 a, glm::vec3 b, glm::vec3 c, int n)
{
	if (n > 0) {
		glm::vec3 v1 = a + b;
		v1 = glm::normalize(v1);
		glm::vec3 v2 = a + c; 
		v2 = glm::normalize(v2);
		glm::vec3 v3 = c + b;
		v3 = glm::normalize(v3);

		subDivide(a, v1, v2, n - 1);
		subDivide(c, v2, v3, n - 1);
		subDivide(b, v3, v1, n - 1);
		subDivide(v3, v2, v1, n - 1);
	} else {
		createNewTriangle(a, b, c);
	}
}

void Shape::createNewTriangle(glm::vec3& a, glm::vec3& b, glm::vec3& c)
{
	Vertex v1{glm::vec3(a.x, a.y, a.z), glm::vec3(a.x, a.y, a.z), glm::vec3(a.x, a.y, a.z), glm::vec2(0.0f, 0.0f)};;
	this->vert.push_back(v1);
	this->inds.push_back(vert.size() - 1);
	Vertex v2{ glm::vec3(b.x, b.y, b.z), glm::vec3(b.x, b.y, b.z), glm::vec3(b.x, b.y, b.z), glm::vec2(0.0f, 0.0f) };
	this->vert.push_back(v2);
	this->inds.push_back(vert.size() - 1);
	Vertex v3{ glm::vec3(c.x, c.y, c.z), glm::vec3(c.x, c.y, c.z), glm::vec3(c.x, c.y, c.z), glm::vec2(0.0f, 0.0f) };
	this->vert.push_back(v3);
	this->inds.push_back(vert.size() - 1);
}