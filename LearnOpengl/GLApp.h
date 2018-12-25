#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class GLApp
{
public:
	GLApp();
	~GLApp();

	std::string loadFileToStr(std::string file_name);

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void processInput(GLFWwindow *window);

private:
	void ColorTriangle();
};

