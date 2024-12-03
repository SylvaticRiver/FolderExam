#include "SOSIConverter.h"
#include <algorithm>

SOSIConverter::SOSIConverter(string filename)
{
	this->filename = filename;
}

void SOSIConverter::createAndWriteCustomFile(string name)
{
	ofstream out;
	ifstream in;

	in.open(this->filename);
	string s;
	in >> s;

	int x, y, z;
	int amount = 0;
	out.open(name);

	out << "    " << endl;
	out << "x" << " " << "y" << " " << "z" << endl;
	while (in.peek() != EOF) {
		while (s != ".PUNKT" && s != ".KURVE" && in.peek() != EOF) {
			in.ignore(1000, '\n');
			in >> s;
		}

		if (s == ".PUNKT") {
			for (int i = 0; i < 3; i++) {
				in.ignore(1000, '\n');
			}

			in.ignore(10, ' ');
			in >> y;

			for (int i = 0; i < 5; i++) {
				in.ignore(1000, '\n');
			}

			in >> x >> z;
			in >> s;
			amount++;

			out << x << " " << y << " " << z << endl;
		}

		else if (s == ".KURVE") {
			string temp;
			for (int i = 0; i < 3; i++) {
				in.ignore(1000, '\n');
			}

			in.ignore(10, ' ');
			in >> y;

			for (int i = 0; i < 5; i++) {
				in.ignore(1000, '\n');
			}

			while (in.peek() != '.' && temp != ".KURVE" && in.peek() != EOF) {
				if (!isdigit(in.peek()) && in.peek() != '.') {
					in.ignore(10000, '\n');
				}
				else if (in.peek() != '.') {
					in >> x >> z;
					out << x << " " << y << " " << z << endl;
					amount++;
				}
				if (in.peek() == '.') {
					in >> temp;
				}
			}
			s = temp;
		}
	}

	out.seekp(0);

	out << amount;

	in.close();
	out.close();
}

float SOSIConverter::findAverageCellHeight(Cell* c, int iterations, vector<Cell*> cells) {
	float avgHeight = 0;
	if (!c->heights.empty()) {
		for (float height : c->heights) {
			avgHeight += height;
		}
		avgHeight /= c->heights.size();
	}
	else if (iterations <= 4) {
		if (!c->neighbors.empty()) {
			int nulledNeighbors = 0;
			for (Cell* neighbor : c->neighbors) {
				float f = findAverageCellHeight(neighbor, iterations + 1, cells);
				if (f != 0) {
					avgHeight += f;
				}
				else {
					nulledNeighbors++;
				}
			}
			avgHeight /= max((int)(c->neighbors.size() - nulledNeighbors), 3);
		}
	}

	return avgHeight;
}

//implementering av algoritme for regulær triangulering kap. 11.4 i forelesningsnotater.
vector<GLuint> SOSIConverter::regularTriangulation(float minX, float maxX, float minZ, float maxZ, int divisions, vector<Vertex> verts) {
	vector<GLuint> indices;
	vector<Vertex> newVerts;
	vector<Cell*> cells;
	vector<Triangle> tris;

	float deltaX = (maxX - minX) / divisions;
	float deltaZ = (maxZ - minZ) / divisions;

	for (int i = 0; i < divisions; i++) {
		for (int j = 0; j < divisions; j++) {
			Cell* c = new Cell{ minX + (deltaX * j), minX + (deltaX * (j + 1)), minZ + (deltaZ * i), minZ + (deltaZ * (i + 1))};
			if (j != 0) {
				c->neighbors.push_back(cells[j - 1]);
				cells[j - 1]->neighbors.push_back(c);
			}
			if (i != 0) {
				c->neighbors.push_back(cells[(divisions * (i - 1)) + j]);
				cells[(divisions * (i - 1)) + j]->neighbors.push_back(c);
			}
			cells.push_back(c);
		}
	}

	for (Vertex v : verts) {
		for (Cell* c : cells) {
			if ((c->maxX >= v.pos.x && v.pos.x > c->minX) && (c->maxZ >= v.pos.z && c->minZ < v.pos.z)) {
				c->heights.push_back(v.pos.y);
			}
		}
	}

	for (int i = 0; i < cells.size(); i++) {
		float friction = 0.0f;
		Cell* c = cells[i];
		float avgX = c->minX + (deltaX / 2);
		float avgZ = c->minZ + (deltaZ / 2);
		float height = findAverageCellHeight(c, 0, vector<Cell*>());

		if (i > 100 && i < 150) {
			friction = 0.8f;
		}

		Vertex v{ glm::vec3(avgX, height, avgZ), glm::vec3(1, 0, 0), glm::vec3(0, 0, 0) , glm::vec2(0.5, 0.5), friction };
		newVerts.push_back(v);
	}

	for (int i = 0; i < (newVerts.size() / divisions) - 1; i++) {
		for (int j= 0; j < (newVerts.size() / divisions) - 1; j++) {
			indices.push_back((divisions * i) + j);
			indices.push_back((divisions * i) + j + 1);
			indices.push_back((divisions * (i + 1)) + j);

			indices.push_back((divisions * (i + 1)) + j);
			indices.push_back((divisions * (i + 1)) + j + 1);
			indices.push_back((divisions * i) + j + 1);
		}
	}

	this->triangles = Object::getTriangles(newVerts, indices);

	this->vertexes = newVerts;
	return indices;
}

void SOSIConverter::renderHeight(string name, int triangulationDivision)
{
	ifstream in;
	int amount;

	float lowestValueX = 0;
	float lowestValueZ = 0;

	float highestValueX = 0;
	float highestValueZ = 0;

	vector<Vertex> verts;
	vector<GLuint> inds;

	in.open(name);

	double x, y, z;
	in.ignore(1000, '\n');
	in.ignore(1000, '\n');

	while (in.peek() != EOF) {
		in >> x >> y >> z;
		if (lowestValueX > x || lowestValueX == 0) {
			lowestValueX = x;
		}
		if (lowestValueZ > z || lowestValueZ == 0) {
			lowestValueZ = z;
		}
		if (highestValueX < x || highestValueX == 0) {
			highestValueX = x;
		}
		if (highestValueZ < z || highestValueZ == 0) {
			highestValueZ = z;
		}
	}

	in.close();

	in.open(name);

	in >> amount;

	in.ignore(1000, '\n');
	in.ignore(1000, '\n');

	for (int i = 0; i < amount; i++) {
		in >> x >> y >> z;
		x = (x - lowestValueX) / 10000;
		y = y / 5;
		z = (z - lowestValueZ) / 10000;

		Vertex v{ glm::vec3(x, y, z), glm::vec3(1, 0, 0), glm::vec3(0, 0, 0) , glm::vec2(0.5, 0.5) };
		verts.push_back(v);
	}

	inds = regularTriangulation(0, (highestValueX - lowestValueX) / 10000, 0, (highestValueZ - lowestValueZ) / 10000, triangulationDivision, verts);

	cout << this->triangles.size() << endl;
	//tatt fra forelesningsnotater 6.2.1
	//normal = u x v normalisert, der u = P1 - P0, v = P2 - P0
	for (int i = 0; i < this->triangles.size(); i++) {
		Triangle triangle = this->triangles[i];
		Vertex P0 = triangle.verticies[0];
		Vertex P1 = triangle.verticies[1];
		Vertex P2 = triangle.verticies[2];

		glm::vec3 u = P2.pos - P0.pos;
		glm::vec3 v = P1.pos - P0.pos;

		glm::vec3 normal = glm::normalize(glm::cross(u, v));

		this->triangles[i].normal = normal;
	}

	//formel fra kap. 6.2.1 i forelesningsnotater s. 88
	//partiellderiverer: partiellderivert x = -y, partiellderivert y = -x.
	//normal = u x v normalisert, der u = [1, 0, partiellderivert x], v = [0, 1, partiellderivert y]
	for (int i = 0; i < this->vertexes.size(); i++) {
		Vertex vert = this->vertexes[i];
		float partDer_X = -vert.pos.y;
		float partDer_Z = -vert.pos.x;

		glm::vec3 u(1, 0, partDer_X);
		glm::vec3 v(0, 1, partDer_Z);

		glm::vec3 normal = glm::cross(u, v);
		normal = glm::normalize(normal);

		this->vertexes[i].normal = normal;
	}

	//this->vertexes = verts;
	this->indices = inds;

}
