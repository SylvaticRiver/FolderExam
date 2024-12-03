#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include "glad/glad.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV;
	float friction;
};

class VBO {
public:
	unsigned int ID;
	VBO(std::vector<Vertex>& verticies);

	void Bind();
	void Unbind();
	void Delete();

};

#endif

