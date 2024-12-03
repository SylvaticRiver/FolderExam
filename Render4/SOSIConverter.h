#pragma once

#include <iostream>
#include <queue>
#include <fstream>
#include "VAO.h"
#include "Object.h"

using namespace std;

struct Cell {
public:
	float minX, maxX, minZ, maxZ;
	vector<float> heights;
	vector<Cell*> neighbors;
};

class SOSIConverter
{
private:
	string filename;

public:
	vector<Vertex> vertexes;
	vector<GLuint> indices;
	vector<Triangle> triangles;

	SOSIConverter(string filename);
	void createAndWriteCustomFile(string name);
	float findAverageCellHeight(Cell* c, int iterations, vector<Cell*> cells);
	vector<GLuint> regularTriangulation(float minX, float maxX, float minZ, float maxZ, int divisions, vector<Vertex> verts);
	void renderHeight(string name, int triangulationDivision);
};
