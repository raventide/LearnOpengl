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

	void setFloat(const std::string& name, const GLfloat&);
	void setMat4(const std::string& name, glm::mat4 value);
	void setVec3(const std::string& name, glm::vec3 value);
	void setVec3(const std::string& name,
		const GLfloat& x,
		const GLfloat& y,
		const GLfloat& z);

private:
	GLuint m_id;
	std::string m_fragCode;
	std::string m_vertCode;

	std::string readText(std::string file_name);
};
