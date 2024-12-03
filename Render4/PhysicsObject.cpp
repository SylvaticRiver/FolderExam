#include "PhysicsObject.h"

PhysicsObject::PhysicsObject()
{

}

PhysicsObject::PhysicsObject(Mesh& mesh, glm::vec3 startPos, glm::vec3 size, Rotator rotation, Shader& shaderProgram, bool hasGravity, float mass)
{
	this->mesh = mesh;
	this->currPos = startPos;
	this->size = size;
	this->rotation = rotation;
	this->velocity = glm::vec3(0, 0, 0);
	this->hasGravity = hasGravity;
	this->mass = mass;

	this->mesh.VAO.Bind();
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::scale(glm::translate(objectModel, this->currPos), this->size);
	objectModel = glm::rotate(objectModel, glm::radians(this->rotation.amount), this->rotation.Axis);

	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
}

void PhysicsObject::updateMovement()
{
	if (hasGravity && this->velocity.y > -1.0) {
		this->velocity += glm::vec3(0, -0.05, 0);
	}
	Object::updateMovement();
}

bool PhysicsObject::isColliding(Object object)
{
	for (Triangle t : object.getTriangles(object.mesh.vertices, object.mesh.indices)) {
		glm::vec3 normal = t.normal;
		glm::vec3 TriPos = glm::vec3((t.verticies[0].pos + t.verticies[1].pos).x / 2, (t.verticies[0].pos + t.verticies[1].pos).y / 2, (t.verticies[0].pos + t.verticies[1].pos).z / 2);

		glm::vec3 y = glm::vec3(this->currPos.x - ((TriPos.x * object.size.x) + object.currPos.x), this->currPos.y - ((TriPos.y * object.size.y) + object.currPos.y), this->currPos.z - ((TriPos.z * object.size.z) + object.currPos.z));
		float r = (glm::normalize(this->mesh.vertices[0].pos + this->mesh.vertices[1].pos) * this->size).x + 0.1;
		float f = glm::l1Norm((y * normal));
		if (f <= r) {
			this->lastCollided = t;
			return true;
		}
	}
	return false;
}

void PhysicsObject::onCollisionWall(Object object)
{	
	Triangle tri = this->lastCollided;
		//Vertex v1 = tri.verticies[0];
		//Vertex v2 = tri.verticies[1];
		//Vertex v3 = tri.verticies[2];
		//glm::vec3 TriPos = glm::vec3((v1.pos + v2.pos).x / 2, (v1.pos + v2.pos).y / 2, (v1.pos + v2.pos).z / 2);
		//TriPos = glm::vec3((TriPos + v3.pos).x / 2, (TriPos + v3.pos).y / 2, (TriPos + v3.pos).z / 2);

		//glm::vec3 normal = glm::cross(v2.pos - v1.pos, v3.pos - v1.pos);

		glm::vec3 normal = tri.normal;
		glm::vec3 TriPos = glm::vec3((tri.verticies[0].pos + tri.verticies[1].pos).x / 2, (tri.verticies[0].pos + tri.verticies[1].pos).y / 2, (tri.verticies[0].pos + tri.verticies[1].pos).z / 2);
		TriPos = glm::vec3((TriPos + tri.verticies[2].pos).x / 2, (TriPos + tri.verticies[2].pos).y / 2, (TriPos + tri.verticies[2].pos).z / 2);

		float r = (glm::normalize(this->mesh.vertices[0].pos + this->mesh.vertices[1].pos) * this->size).x;

		glm::vec3 y = (glm::vec3(this->currPos.x - ((TriPos.x * object.size.x) + object.currPos.x), this->currPos.y - ((TriPos.y * object.size.y) + object.currPos.y), this->currPos.z - ((TriPos.z * object.size.z) + object.currPos.z)) * normal) * normal;
		glm::vec3 a = (this->velocity * normal) * normal;
		glm::vec3 x = this->velocity - (glm::vec3(0.125 * a.x, 0.125 * a.y, 0.125 * a.z));

		//av en eller annen grunn teleporterer dette ballene out of bounds, saa jeg bruker heller bare den forrige posisjonsdataen til ballen
		//this->move((((r - y) / r) * this->velocity) + ((y / r) * x));
		this->currPos = this->prevPos;
		this->velocity = x * glm::vec3(0.95);

	//this->velocity *= -1;
}

//Provde metoden fra forelesninger/Vism notater, men uansett hvor mye jeg provde fikk jeg det ikke til, brukte 2 hele dager paa dette for jeg ga opp og heller bare brukte squareDistance mellom kulene
//for kollisjonsdetektering

//bool PhysicsObject::isCollidingBall(Object* object)
//{
//	static const float EPSILON{ 0.000001 };
//	glm::vec3 A = this->currPos - object->currPos;
//	glm::vec3 B = this->velocity - object->velocity;
//
//	auto AB = A * B;
//	auto AA = A * A;
//	auto BB = B * B;
//
//	auto d = 0.1f + 0.1f;
//	auto rot = (AB * AB) - (BB * (AA - d * d));
//	float t = -1.0f;
//	float l = glm::length(rot);
//	if (l >= 0.0f) {
//		t = -glm::length(AB) - std::sqrt(l);
//	}
//	if (glm::length(BB) > EPSILON) {
//		t = t / glm::length(BB);
//	}
//
//	return 0 < t < 1;
//}

bool PhysicsObject::isCollidingBall(Object* object)
{
	float d = 1.0f + 1.0f;
	float dd = glm::distance2(this->currPos, object->currPos);
	return dd < d * d;
}

void PhysicsObject::rollOnsurface(Triangle triangle)
{
	//bruker oppfunnet deltaTid istedet for å måle den siden jeg tror ikke det har for mye å si
	float TICKDELTA = 0.0001;
	float frictionConstant = triangle.friction != 0 ? triangle.friction : 0.44f;
	//float TICKDELTA = 0.0000000001;

	glm::vec3 P0 = triangle.verticies[0].pos;
	glm::vec3 P1 = triangle.verticies[1].pos;
	glm::vec3 P2 = triangle.verticies[2].pos;

	glm::vec3 normal = glm::normalize(glm::cross(P1 - P0, P2 - P0));
	//glm::vec3 normal = glm::cross(P1 - P0, P2 - P0);

	//formel 9.14 i forelesningsnotater: a = g[nx * nz, ny * nz, (nz^2) - 1]
	//her erstatter jeg normal.z med normal.y i akselerasjonsformelen siden oppover-retningen min er y-retning
	glm::vec3 a = glm::vec3((normal.x * normal.y) * 9.81, ((normal.y *normal.y) - 1) * 9.81, ((normal.z * normal.y)) * 9.81);

	//kap. 9.2.5 i forelesningsnotater, og
	//formel 9.12 i forelesningsnotater: N = G * n * cos(a) = mg[nx, ny, nz] * ny = mg[nx*ny, ny*ny, nz*ny]
	glm::vec3 frictionForce = -glm::vec3((normal.x * normal.y) * 9.81, ((normal.y * normal.y) - 1) * 9.81, (normal.z * normal.y) * 9.81);

	//V_k+1. formel 9.16 i forelesningsnotater: V_k+1 = V_k + a * delta T
	//legger til friksjonen som i kap 9.2.5 i forelesningsnotater: R = µN
	glm::vec3 newVel = this->velocity + glm::vec3(a.x * TICKDELTA, 0, a.z * TICKDELTA) + (frictionForce * frictionConstant * 0.0001f);

	if (sqrt(newVel.x * newVel.x + newVel.y * newVel.y + newVel.z * newVel.z) >= 0.95f) {
		this->velocity = newVel * 0.9999f;

		glm::vec3 V_k = glm::vec3(this->velocity.x * TICKDELTA, this->velocity.y * TICKDELTA, this->velocity.z * TICKDELTA);

		//formel 9.17 i forelesningsnotater: P_k+1 = P_k + V_k * delta T + 1/2 a (delta T)^2
		glm::vec3 h = glm::vec3(0.5 * a.x * (TICKDELTA * TICKDELTA), 0.5 * a.y * (TICKDELTA * TICKDELTA), 0.5 * a.z * (TICKDELTA * TICKDELTA));

		this->move(V_k + h);

		//formel 9.11 i forelesningsnotater: r = n x v
		glm::vec3 rot = glm::cross(normal, this->velocity);

		this->rotate(glm::length(this->velocity) / TICKDELTA, rot);
	}
	else {
		this->velocity *= 0;
	}
}

//kollisjonsrespons som i pseudokode kap. 9.7.6, Listing 9.1
void PhysicsObject::onBallCollision(PhysicsObject* object) {

	glm::vec3 v0 = this->velocity * (this->mass - object->mass) + object->velocity * object->mass * 2.0f;
	v0 = v0 / (object->mass + this->mass);
	glm::vec3 v1 = this->velocity * (object->mass - this->mass) + this->velocity * this->mass * 2.0f;
	v1 = v1 / (this->mass + object->mass);
	cout << "hit" << endl;
	this->velocity = v0;
	this->currPos = this->prevPos;
	object->velocity = v1;
	object->currPos = object->prevPos;
}

