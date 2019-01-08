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
	static GLApp *curApp;

	std::string loadFileToStr(std::string file_name);

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xpos, double ypos);

	void setCameraPos(glm::vec3);

	friend bool processInput(GLFWwindow *window, GLApp* app);
	friend void processMouseInput(GLApp *app, double xpos, double ypos);
	friend void processScrollInput(GLApp *app, double xoffset, double yoffset);

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

	bool m_firstMouse;
	float m_yaw;
	float m_pitch;
	float m_lastX;
	float m_lastY;
	float m_fov;
};

bool processInput(GLFWwindow *window, GLApp* app);
void processMouseInput(GLApp *app, double xpos, double ypos);
void processScrollInput(GLApp *app, double xoffset, double yoffset);