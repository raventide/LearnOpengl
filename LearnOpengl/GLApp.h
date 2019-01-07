#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>

class GLApp
{
public:
	GLApp();
	~GLApp();

	std::string loadFileToStr(std::string file_name);

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	void setCameraPos(glm::vec3);

	friend bool processInput(GLFWwindow *window, GLApp* app);

private:
	void ColorTriangle();
	void Texture();
	void Cube();
	void Transform();
	void DiffuseLight();
	void SpecularLight();

	GLFWwindow* m_window;
	glm::vec3 m_cameraPos;
	glm::vec3 m_cameraFront;
	glm::vec3 m_cameraUp;
	float m_deltaTime;
	float m_lastFrame;
};

bool processInput(GLFWwindow *window, GLApp* app);