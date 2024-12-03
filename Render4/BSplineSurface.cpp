#include "BSplineSurface.h"

//følger pseudo-kode fra forelesningsnotater kap. 12.3, listing 12.2 -> listing 12.5

BSplineSurface::BSplineSurface()
{
	string name = "stest";
	n_u = 4;
	n_v = 3;
	d_u = 2;
	d_v = 2;

	mu.push_back(0);
	mu.push_back(0);
	mu.push_back(0);
	mu.push_back(1);
	mu.push_back(2);
	mu.push_back(2);
	mu.push_back(2);

	mv.push_back(0);
	mv.push_back(0);
	mv.push_back(0);
	mv.push_back(1);
	mv.push_back(1);
	mv.push_back(1);

	for (int i = 0; i < n_u; i++) {
		mc.push_back(glm::vec3(i, 0, 0));
	}
	mc.push_back(glm::vec3(0, 0, 1));
	mc.push_back(glm::vec3(1, 2, 1));
	mc.push_back(glm::vec3(2, 2, 1));
	mc.push_back(glm::vec3(3, 0, 1));
	for (int i = 0; i < n_u; i++) {
		mc.push_back(glm::vec3(i, 0, 2));
	}
	makeBiQuadraticSurface();
	
}

void BSplineSurface::makeBiQuadraticSurface() {
	float h = 0.1f;

	int nu = (mu[n_u] - mu[d_u]) / h;
	int nv = (mv[n_v] - mv[d_v]) / h;

	for (int i = 0; i < nu; i++) {
		for (int j = 0; j < nv; j++) {

			float u = j * h;
			float v = i * h;

			int newU = findKnotInterval(mu, d_u, n_u, u);
			int newV = findKnotInterval(mv, d_v, n_v, v);

			pair<glm::vec3, glm::vec3> koeff = B2(u, v, newU, newV);

			glm::vec3 p0 = evaluateBiquadratic(newU, newV, koeff.first, koeff.second);

			verts.push_back(Vertex{p0, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec2(0.5f, 0.5f)});
		}
	}

	for (int i = 0; i < nu - 1; i++) {
		for (int j = 0; j < nv - 1; j++) {
			ind.push_back((nu * i) + j);
			ind.push_back((nu * i) + j + 1);
			ind.push_back((nu * (i + 1)) + j);

			ind.push_back((nu * (i + 1)) + j);
			ind.push_back((nu * (i + 1)) + j + 1);
			ind.push_back((nu * i) + j + 1);
		}
	}
}

int BSplineSurface::findKnotInterval(vector<float> vec, int d, int amount, int t)
{
	int i = amount - 1;
	while (t < vec[i] && i > d) {
		i--;
	}
	return i;
}

glm::vec3 BSplineSurface::evaluateBiquadratic(int u, int v, glm::vec3 bu, glm::vec3 bv) {
	glm::vec3 r{ 0.0f, 0.0f, 0.0f };

	float du = u - d_u;
	float dv = v - d_v;

	float w[3][3];

	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 3; i++) {
			float fu = du + i;
			float fv = dv + j;

			r += bu[i] * bv[j] * mc[fu + fv * n_u];
		}
	}
	return r;
}

pair<glm::vec3, glm::vec3> BSplineSurface::B2(float tu, float tv, int u, int v) {

	glm::vec3 bv;
	glm::vec3 bu;

	float bu_0; 
	float bu_2; 
	float bv_0; 
	float bv_2;

	while (tu > 1) {
		tu--;
	} 

	bu_0 = powf(1 - tu, 2);
	bu_2 = powf(tu, 2);

	while (tv > 1) {
		tv--;
	}

	bv_0 = powf(1 - tv, 2);
	bv_2 = powf(tv, 2);

	bu = glm::vec3(bu_0, 1 - bu_0 - bu_2, bu_2);
	bv = glm::vec3(bv_0, 1 - bv_0 - bv_2, bv_2);

	return pair<glm::vec3, glm::vec3>(bu, bv);
}