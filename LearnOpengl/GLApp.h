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
	static void processInput(GLFWwindow *window,GLApp* app);

	void setCameraPos(glm::vec3);

private:
	void ColorTriangle();
	void Texture();
	void Cube();
	void Transform();

	GLFWwindow* m_window;
	glm::vec3 m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float m_deltaTime = 0.0f;
	float m_lastFrame = 0.0f;
};
