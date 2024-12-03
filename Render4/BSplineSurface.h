#pragma once

#include "glm/vec3.hpp"
#include <iostream>
#include <vector>
#include "VBO.h"

using namespace std;

class BSplineSurface
{
private:
	int n_u = 4;
	int n_v = 3;
	int d_u = 2;
	int d_v = 2;

	vector<float> mu;
	vector<float> mv;
	vector<glm::vec3> mc;

	glm::vec3 c{4, 3, 0};

	pair<glm::vec3, glm::vec3> B2(float tu, float tv, int u, int v);
	glm::vec3 evaluateBiquadratic(int u, int v, glm::vec3 bu, glm::vec3 bv);
	void makeBiQuadraticSurface();
	int findKnotInterval(vector<float> vec, int d, int amount, int t);

public:

	vector<Vertex> verts;
	vector<GLuint> ind;

	BSplineSurface();
};

