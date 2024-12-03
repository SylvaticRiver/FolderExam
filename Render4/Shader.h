#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <iostream>
#include "glad/glad.h"
#include <string>
#include <fstream>
#include <sstream>
#include <cerrno>

using namespace std;

string readShaderFile(const char* filename);

class Shader {
public:
	unsigned int ID;
	Shader(const char* vertexFileName, const char* fragmentFileName);

	void Activate();
	void Delete();
};

#endif