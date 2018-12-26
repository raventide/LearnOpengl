#include "GLShader.h"
#include <sstream>
#include <fstream>
#include <iostream>

GLShader::GLShader()
{
}

GLShader::~GLShader()
{
}

void GLShader::readFrag(std::string file_name)
{
	m_fragCode = readText(file_name);
}

void GLShader::readVert(std::string file_name)
{
	m_vertCode = readText(file_name);
}

void GLShader::compile()
{
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char *vertexShaderSource = m_vertCode.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	checkCompileErrors(vertexShader, "VERTEX");

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char *fragmentShaderSource = m_fragCode.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	checkCompileErrors(fragmentShader,"FRAGMENT");

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	checkCompileErrors(shaderProgram, "PROGRAM");
	m_id = shaderProgram;

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void GLShader::checkCompileErrors(GLuint shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

std::string GLShader::readText(std::string file_name)
{
	std::string shaderCode;
	std::ifstream FragmentShaderStream(file_name, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		shaderCode = sstr.str();
		FragmentShaderStream.close();
		return shaderCode;
	}
	else
	{
		std::cout << "[ERROR]Failed to open : " << file_name << std::endl;
		return "0";
	}
}

void GLShader::use()
{
	glUseProgram(m_id);
}