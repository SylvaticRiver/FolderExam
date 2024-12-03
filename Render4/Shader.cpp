#include "Shader.h"

string readShaderFile(const char* filename)
{
	string shaderCode;
	ifstream shaderFile;
	stringstream shaderStream;
	shaderFile.open(filename);
	if (!shaderFile);
	shaderStream << shaderFile.rdbuf();

	shaderFile.close();

	shaderCode = shaderStream.str();
	return shaderCode;
}

Shader::Shader(const char* vertexFileName, const char* fragmentFileName) {

	string vertexCode = readShaderFile(vertexFileName);
	string fragmentCode = readShaderFile(fragmentFileName);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	int vertexSuccess;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexSuccess);

	if (!vertexSuccess) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "vertex shader compile error : " << infoLog << endl;
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	int fragmentShaderSuccess;
	char infoLogFrag[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderSuccess);

	if (!fragmentShaderSuccess) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLogFrag);
		cout << "Fragment shader compile error : " << infoLogFrag << endl;
	}
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	int linkSuccess;
	char infoLogProgram[512];
	glGetProgramiv(ID, GL_LINK_STATUS, &linkSuccess);
	if (!linkSuccess) {
		glGetProgramInfoLog(ID, 512, NULL, infoLogProgram);
		cout << "Linking error : " << infoLogProgram << endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}

