#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures)
{
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;

	VAO.Bind();
	VBO VBO(vertices);
	EBO EBO(indices);
	VAO.LinkVBO(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkVBO(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.LinkVBO(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	VAO.LinkVBO(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));
	VAO.LinkVBO(VBO, 4, 1, GL_FLOAT, sizeof(Vertex), (void*)(11 * sizeof(float)));
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void Mesh::Draw(Shader& shader, Camera& camera, bool dotRender)
{
	shader.Activate();
	VAO.Bind();

	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].type;
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}
		textures[i].texUnit(shader, (type + num).c_str(), i);
		textures[i].Bind();
	}

	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	camera.Matrix(shader, "camMatrix");

	if (dotRender) {
		glPointSize(min(1.0f * (glm::distance(this->pos, camera.position) * 3), 5.0f));
		glDrawArrays(GL_POINTS, 0, this->vertices.size());
	}
	else {
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
}
