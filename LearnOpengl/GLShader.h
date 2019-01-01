#pragma once
#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>

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

	void setMat4(const std::string& name, glm::mat4 value);

private:
	GLuint m_id;
	std::string m_fragCode;
	std::string m_vertCode;

	std::string readText(std::string file_name);
};
