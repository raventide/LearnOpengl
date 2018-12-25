#include "GLApp.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include "GLShader.h"

GLApp::GLApp()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Project Template", NULL, NULL);
	if (window == NULL)
	{
		printf("Failed to create GLFW window!!\n");
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);

	// Needed in core profile
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		printf("Failed to initialize GLEW!!\n");
		exit(-1);
	}
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	const char *vertexShaderSource = loadFileToStr("shaders/vertex.vert").c_str();

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("ERROR:<%s> compile failed! INFO:\n", vertexShaderSource);
		printf("%s\n", infoLog);
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char *fragmentShaderSource = loadFileToStr("shaders/fragment.frag").c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("ERROR:<%s> compile failed! INFO:\n", fragmentShaderSource);
		printf("%s\n", infoLog);
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("%s\n", infoLog);
	}
	//glUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLShader shader;
	shader.readFrag("shaders/fragment.frag");
	shader.readVert("shaders/vertex.vert");
	shader.compile();

	// prepare rendering
	float vertices[] = {
		-0.5f,0.0f,0.0f,
		 0.5f,0.0f,0.0f,
		 0.0f,0.5f,0.0f,
		 0.0f,-0.5f,0.0f
	};
	float vertices2[] = {
		0.5f,0.5f,0.0f,
		-0.5f,0.5f,0.0f,
		0.0f,-0.6f,0.0f
	};
	int unsigned indices[] = {
		0,1,2,
		0,1,3
	};
	float color_data[] =
	{
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,0.0f,1.0f,
		0.1f,0.5f,0.9f
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//GLuint EBO;
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//GLuint VBO_color;
	//glGenBuffers(1, &VBO_color);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(color_data), color_data, GL_STATIC_DRAW);
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(1);

	//unsigned int VAO2;
	//glGenVertexArrays(1, &VAO2);
	//glBindVertexArray(VAO2);

	unsigned int VBO2;
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_data), color_data, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// rendering here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		//glUseProgram(shaderProgram);

		//int uniform_loc = glGetUniformLocation(shaderProgram, "inputColor");
		//glUniform4f(uniform_loc, 0.5f, 0.1f, 0.2f, 1.0f);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 4);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glDrawElements(GL_TRIANGLES, 4, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//glBindVertexArray(VAO2);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}


GLApp::~GLApp()
{
}

void GLApp::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void GLApp::processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

std::string GLApp::loadFileToStr(std::string file_name)
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
	return "0";
}

void GLApp::ColorTriangle()
{

}