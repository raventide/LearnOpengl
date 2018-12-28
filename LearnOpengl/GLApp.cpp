#include "GLApp.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include "GLShader.h"
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLApp::GLApp()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(800, 800, "OpenGL Project Template", NULL, NULL);
	if (m_window == NULL)
	{
		printf("Failed to create GLFW window!!\n");
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(m_window);

	// Needed in core profile
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		printf("Failed to initialize GLEW!!\n");
		exit(-1);
	}
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

	Texture();


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

	while (!glfwWindowShouldClose(m_window))
	{
		processInput(m_window);

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

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
}

void GLApp::Texture()
{
	GLShader shader;
	shader.readFrag("shaders/05texture/fragment.frag");
	shader.readVert("shaders/05texture/vertex.vert");
	shader.compile();

	GLfloat vertices[] = {
		0.5f,0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		-0.5f,-0.5f,0.0f,
		-0.5f,0.5f,0.0f
	};
	GLuint indices[] = {
		0,1,3,
		1,2,3
	};
	GLfloat colors[] = {
		0.8f,0.0f,0.0f,
		0.0f,0.8f,0.0f,
		0.0f,0.0f,0.8f,
		0.7f,0.5f,0.2f
	};
	GLfloat tex_cord[] =
	{
		1.0f,1.0f,
		1.0f,0.0f,
		0.0f,0.0f,
		0.0f,1.0f
	};

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// allocate enough room
	glNamedBufferStorage(VBO, 
		sizeof(vertices) + sizeof(colors) + sizeof(tex_cord),
		nullptr,
		GL_DYNAMIC_STORAGE_BIT);

	glNamedBufferSubData(VBO, 0, sizeof(vertices), vertices);
	glNamedBufferSubData(VBO, sizeof(vertices), sizeof(colors), colors);
	glNamedBufferSubData(VBO, sizeof(vertices) + sizeof(colors), sizeof(tex_cord), tex_cord);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// colors
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)sizeof(vertices));
	//glEnableVertexAttribArray(1);

	//// texture coord attribute
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(vertices) + sizeof(colors)));
	//glEnableVertexAttribArray(2);

	for (int i = 0; i < 12; ++i)
	{
		printf("%.2f ", colors[i]);
		if ((i + 1) % 3 == 0)
			std::cout << std::endl;
	}

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)sizeof(vertices));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(vertices) + sizeof(colors)));

	// load and create a texture
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("res/text.jpg",&width,&height,&nrChannels,0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "[ERROR] Failed to load texture file" << std::endl;
	}
	stbi_image_free(data);

	while (!glfwWindowShouldClose(m_window))
	{
		processInput(m_window);

		// rendering here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, texture);

		glm::mat4 transform;
		//transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
		transform = glm::scale(transform, glm::vec3(0.5, 0.5, 0.5));
		transform = glm::rotate(transform, (float)glfwGetTime() * 2, glm::vec3(0.0f, 1.0f, 0.0f));
		
		shader.use();

		GLuint transformLoc = glGetUniformLocation(shader.id(), "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAO);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 6 * 3);

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
}
