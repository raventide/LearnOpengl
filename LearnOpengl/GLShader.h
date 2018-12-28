#pragma once
#include <GL/glew.h>
#include <string>

class GLShader
{
public:
	GLShader();
	~GLShader();
	void readFrag(std::string file_name);
	void readVert(std::string file_name);
	void compile();
	void checkCompileErrors(GLuint shader, std::string type);
	void use();
	GLuint id();

private:
	GLuint m_id;
	std::string m_fragCode;
	std::string m_vertCode;

	std::string readText(std::string file_name);
};
