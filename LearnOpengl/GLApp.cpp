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
#include "CommonData.h"

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

	m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	m_deltaTime = 0.0f;
	m_lastFrame = 0.0f;

	DiffuseLight();
	//Transform();

	glfwTerminate();
}


GLApp::~GLApp()
{
}

void GLApp::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

bool processInput(GLFWwindow *window,GLApp* app)
{
	float currentFrame = glfwGetTime();
	app->m_deltaTime = currentFrame - app->m_lastFrame;
	app->m_lastFrame = currentFrame;

	bool changed = false;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float camera_speed = 2.5 * app->m_deltaTime; 
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{ 
		app->m_cameraPos += app->m_cameraFront * camera_speed;
		changed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		app->m_cameraPos -= camera_speed * app->m_cameraFront;
		changed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		app->m_cameraPos -= glm::normalize(glm::cross(app->m_cameraFront, app->m_cameraUp)) * camera_speed;
		changed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		app->m_cameraPos += glm::normalize(glm::cross(app->m_cameraFront, app->m_cameraUp)) * camera_speed;
		changed = true;
	}
	return changed;
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
		processInput(m_window,this);

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
		processInput(m_window,this);

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

void GLApp::Cube()
{
	GLfloat cube_verts[] = {
		-0.5f,0.5f,0.0f,
		0.5f,0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		-0.5f,-0.5f,0.0f,
		-0.5f,0.5f,-0.5f,
		0.5f,0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
	};
	GLuint indices[] = {
		0,1,2,
		1,2,3,
		4,5,6,
		5,6,7,
		0,4,5,
		0,5,1,
		2,6,7,
		2,7,3,
		6,4,0,
		6,0,2,
		3,1,5,
		3,5,7,
	};
	GLfloat tex_coords[] = {
		0.0f,0.0f,
		0.0f,1.0f,
		1.0f,1.0f,
		1.0f,0.0f,
		0.0f,0.0f,
		0.0f,1.0f,
		1.0f,1.0f,
		1.0f,0.0f
	};
	GLfloat colors[] =
	{
		0.8f,0.0f,0.0f,
		0.0f,0.8f,0.0f,
		0.0f,0.0f,0.8f,
		0.7f,0.2f,0.5f,
		0.8f,0.0f,0.0f,
		0.0f,0.8f,0.0f,
		0.0f,0.0f,0.8f,
		0.7f,0.2f,0.5f,
	};

	GLuint VAO;
	// 0:vertices, 1:texture coordinates ,2:colors
	GLuint VBOs[3];
	GLuint EBO;

	GLShader shader;
	shader.readFrag("shaders/cube/fragment.frag");
	shader.readVert("shaders/cube/vertex.vert");
	shader.compile();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_verts), cube_verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &VBOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords), tex_coords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0,(void*)0);

	glGenBuffers(2, &VBOs[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
	unsigned char* data = stbi_load("res/wall.jpg", &width, &height, &nrChannels, 0);
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
		processInput(m_window,this);

		// rendering here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, texture);

		shader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
	glDeleteBuffers(3, &VBOs[0]);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);
}

void GLApp::Transform()
{

	GLfloat cube_verts[] = {
		-0.5f,0.5f,0.0f,
		0.5f,0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		//-0.5f,-0.5f,0.0f,
		//-0.5f,0.5f,-0.5f,
		0.5f,0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
	};
	GLfloat colors[] =
	{
		0.8f,0.0f,0.0f,
		0.0f,0.8f,0.0f,
		0.0f,0.0f,0.8f,
		//0.7f,0.2f,0.5f,
		//0.8f,0.0f,0.0f,
		0.0f,0.8f,0.0f,
		0.0f,0.0f,0.8f,
		0.7f,0.2f,0.5f,
	};
	GLfloat tex_coords[] = {
		0.0f,1.0f,
		1.0f,1.0f,
		1.0f,0.0f,
		1.0f,1.0f,
		1.0f,0.0f,
		0.0f,0.0f
	};

	// world space positions of our cubes
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glEnable(GL_DEPTH_TEST);
	GLShader shader;
	shader.readFrag("shaders/transform/fragment.frag");
	shader.readVert("shaders/transform/vertex.vert");
	shader.compile();

	GLuint VAO;
	GLuint VBO_verts, VBO_colors, VBO_texCoords;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO_verts);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_verts);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_verts), cube_verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &VBO_colors);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_colors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &VBO_texCoords);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_texCoords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords), tex_coords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

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
	unsigned char* data = stbi_load("res/wall.jpg", &width, &height, &nrChannels, 0);
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
		processInput(m_window, this);
		// rendering here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, texture);

		glm::mat4 trans;
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		//unsigned int transformLoc = glGetUniformLocation(shader.id(), "transform");
		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)800, 0.1f, 100.0f);
		//glm::mat4 view = glm::mat4(1.0f);
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		//		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		float camera_speed = 2.50f * m_deltaTime;
		//if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
		//{
		//	m_cameraPos += m_cameraFront * camera_speed;
		//}
		//if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
		//{
		//	m_cameraPos -= camera_speed * m_cameraFront;
		//}
		//if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
		//{
		//	m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * camera_speed;
		//}
		//if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
		//{
		//	m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * camera_speed;
		//}
		glm::mat4 view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
		shader.setMat4("view", view);
		//std::cout << *glm::value_ptr(view) << std::endl;
		shader.use();
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.setMat4("model", model);

			if (i % 3 == 0)
			{
				shader.setMat4("transform", trans);
			}
			else
			{
				shader.setMat4("transform", glm::mat4());
			}

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO_verts);
	glDeleteBuffers(1, &VBO_colors);
}

void GLApp::setCameraPos(glm::vec3 pos)
{
	m_cameraPos = pos;
}

void GLApp::DiffuseLight()
{
	m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	m_cameraFront = glm::vec3(-0.50f, -0.50f, -1.0f);
	m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);

	GLShader lightingShader;
	lightingShader.readFrag("shaders/light/object.frag");
	lightingShader.readVert("shaders/light/object.vert");
	lightingShader.compile();
	GLShader lampShader;
	lampShader.readFrag("shaders/light/light.frag");
	lampShader.readVert("shaders/light/light.vert");
	lampShader.compile();

	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	GLuint cube_vao, cube_vbo;
	glGenVertexArrays(1, &cube_vao);
	glGenBuffers(1, &cube_vbo);
	
	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cube_vao);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	GLuint light_vao;
	glGenVertexArrays(1, &light_vao);
	glBindVertexArray(light_vao);

	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glm::vec3 cam(m_cameraPos.x + 2.0f, m_cameraPos.y + 2.0f, m_cameraPos.z + 2.0f);
	m_cameraPos = cam;
	glm::vec3 light_pos(1.0f, 1.0f, -1.0f);

	while (!glfwWindowShouldClose(m_window))
	{
		processInput(m_window, this);

		glClearColor(0.1f, 0.4f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.use();
		lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		lightingShader.setVec3("lightPos", light_pos);

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)800, 0.1f, 100.0f);
		lightingShader.setMat4("projection", projection);

		glm::mat4 view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
		lightingShader.setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);

		glBindVertexArray(cube_vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		model = glm::mat4(1.0f);

		model = glm::translate(model, light_pos);
		model = glm::scale(model, glm::vec3(0.2f));
		lampShader.setMat4("model", model);

		glBindVertexArray(light_vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cube_vao);
	glDeleteVertexArrays(1, &light_vao);
	glDeleteBuffers(1, &cube_vbo);

}

void GLApp::SpecularLight()
{

}
