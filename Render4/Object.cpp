#include "Object.h"

Object::Object()
{

}

Object::Object(Mesh& mesh, glm::vec3 startPos, glm::vec3 size, Rotator rotation, Shader& shaderProgram)
{
	this->mesh = mesh;
	this->currPos = startPos;
	this->size = size;
	this->rotation = rotation;
	this->velocity = glm::vec3(0, 0, 0);
	this->hasGravity = hasGravity;
	this->prevPos = this->currPos;

	this->mesh.VAO.Bind();
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::scale(glm::translate(objectModel, this->currPos), this->size);
	objectModel = glm::rotate(objectModel, glm::radians(this->rotation.amount), this->rotation.Axis);

	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
}

void Object::Render(Shader& shader, Camera& camera, bool dotRender)
{
	updateMovement();
	update(shader);
	this->mesh.Draw(shader, camera, dotRender);
}

void Object::setPos(glm::vec3 newPos)
{
	this->currPos = newPos;
}

void Object::move(glm::vec3 amount)
{
	this->currPos += amount;
}

void Object::rescale(glm::vec3 newSize)
{
	this->size = newSize;
}

void Object::rotate(float degree, glm::vec3 direction)
{
	this->rotation = Rotator{ direction, degree };
}

void Object::update(Shader& shaderProgram)
{
	this->mesh.VAO.Bind();
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::scale(glm::translate(objectModel, this->currPos), this->size);
	objectModel = glm::rotate(objectModel, glm::radians(this->rotation.amount), this->rotation.Axis);

	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
}

std::vector<Triangle> Object::getTriangles(vector<Vertex> vertices, vector<GLuint> indices) {

	std::vector<Triangle> tris;
	std::vector<Vertex> verts = vertices;
	std::vector<GLuint> ind = indices;

	for (int i = 0; i < ind.size() / 3; i++) {
		std::vector<Vertex> triverts;
		for (int j = 0; j < 3; j++) {
			triverts.push_back(verts[ind[(3 * i) + j]]);
		}

		//tatt fra forelesningsnotater 6.2.1
        //normal = u x v normalisert, der u = P1 - P0, v = P2 - P0
		Vertex P0 = triverts[0];
		Vertex P1 = triverts[1];
		Vertex P2 = triverts[2];

		glm::vec3 u = P1.pos - P0.pos;
		glm::vec3 v = P2.pos - P0.pos;

		glm::vec3 normal = glm::normalize(glm::cross(u, v));

		//choosing to use the average of the 3 vertices' friction to determine the friction of a triangle
		//an alternative could be to for example multiply the barycentric coordinates inside the triangle by the friction value of the corresponding vertex and adding it up,
		//but it's a lot of rework to do so I figured this would be good enough.
		float friction = (P0.friction + P1.friction + P2.friction) / 3;

		Triangle t{ triverts, normal, friction};
		tris.push_back(t);
	}
	return tris;
}

void Object::updateMovement()
{
	this->prevPos = this->currPos;
	move(this->velocity);
}

//tatt fra forelesningsnotater kap. 6.2.4 s. 91
//z = f(x, y), f(x, y) = u * f(Px, Py) + v * f(Qx, Qy) + w * f(Rx, Ry). Her erstatter jeg y med z og motsatt siden planet strekker seg i XZ planet med Y-verdien som oppover koordinat
float Object::getBarHeight(Triangle triangle, glm::vec3 bars) {
	return (bars[0] * triangle.verticies[0].pos[1]) + (bars[1] * triangle.verticies[1].pos[1]) + (bars[2] * triangle.verticies[2].pos[1]);
}

//taken from math 3 notes on canvas
//glm::vec3 Object::barycentricCoordinates(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
//{
//	glm::vec3 p12 = p2 - p1;
//	glm::vec3 p13 = p3 - p1;
//	glm::vec3 n = glm::cross(p12, p13);
//
//	float areal_123 = sqrt(pow(n.x, 2) + pow(n.y, 2) + pow(n.z, 2));
//	glm::vec3 baryc;
//
//	glm::vec3 p = p2 - this->currPos;
//	glm::vec3 q = p3 - this->currPos;
//	n = glm::cross(p, q);
//	baryc.x = n.y / areal_123;
//
//	p = p3 - this->currPos;
//	q = p1 - this->currPos;
//	n = glm::cross(p, q);
//	baryc.y = n.y / areal_123;
//
//	p = p1 - this->currPos;
//	q = p2 - this->currPos;
//	n = glm::cross(p, q);
//	baryc.z = n.y / areal_123;
//
//	return baryc;
//}

//I had to use Chat-GPT to help me find another method of calculating barycentric coordinates here. The previous method that I used (using the cross product divided by the area as seen above)
//led to some major issues. It would "share" the barycentric coordinates of triangles that connected into making a square together. Which effectively meant I'd only get the correct barycentric
//coordinates of every other triangle. It was suggested that it's due to the order of which I send vertices into the method, but after 3 days of debugging this issue I conceded
//to using the dot product and then determinant method which chat-GPT suggested as I am low on time
glm::vec3 Object::barycentricCoordinates(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	//projekserer utregningen til XZ-planet siden jeg har Y som høydeverdi
	glm::vec2 projP1(p1.x, p1.z);
	glm::vec2 projP2(p2.x, p2.z);
	glm::vec2 projP3(p3.x, p3.z);
	glm::vec2 projPos(this->currPos.x, this->currPos.z);

	glm::vec2 v0 = projP2 - projP1;
	glm::vec2 v1 = projP3 - projP1;
	glm::vec2 v2 = projPos - projP1;

	float d00 = glm::dot(v0, v0);
	float d01 = glm::dot(v0, v1);
	float d11 = glm::dot(v1, v1);
	float d20 = glm::dot(v2, v0);
	float d21 = glm::dot(v2, v1);

	float denom = d00 * d11 - d01 * d01;
	float v = (d11 * d20 - d01 * d21) / denom;
	float w = (d00 * d21 - d01 * d20) / denom;
	float u = 1.0f - v - w;

	return glm::vec3(u, v, w);
}

LightObject::LightObject(Mesh& mesh, glm::vec3 startPos, glm::vec3 size, Rotator rotation, glm::vec4 lightColor, float lightStrenght, Shader& lightShader, Shader& shaderProgram) {
	LightObject::mesh = mesh;
	LightObject::currPos = startPos;
	LightObject::size = size;
	LightObject::rotation = rotation;
	LightObject::lightcolor = lightColor;
	LightObject::lightStrenght = lightStrenght;

	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, LightObject::currPos);
	lightModel = glm::scale(glm::translate(lightModel, LightObject::currPos), LightObject::size);
	lightModel = glm::rotate(lightModel, glm::radians(LightObject::rotation.amount), LightObject::rotation.Axis);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), LightObject::lightcolor.x, LightObject::lightcolor.y, LightObject::lightcolor.z, LightObject::lightcolor.w);
	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), LightObject::lightcolor.x, LightObject::lightcolor.y, LightObject::lightcolor.z, LightObject::lightcolor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), LightObject::currPos.x, LightObject::currPos.y, LightObject::currPos.z);
	glUniform1f(glGetUniformLocation(shaderProgram.ID, "lightStrenght"), LightObject::lightStrenght);
}
