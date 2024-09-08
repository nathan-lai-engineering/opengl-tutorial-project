#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

int main()
{
	glfwInit();

	// tell GLFW the version and profile we are using of OPENGL
	// we use CORE because that has the modern functions
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// vertices to draw an equilateral triangle
	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
		-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
		0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
		0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down
	};

	GLuint indices[] =
	{
		0,3,5,
		3,2,4,
		5,4,1
	};

	// create a GLFW window object to put our stuffs on
	GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL", NULL, NULL);

	// error check to cleanly remove GLFW in case window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// introduce window into the current context (make it visible)
	glfwMakeContextCurrent(window);

	// load GLAD to configure OpenGL
	gladLoadGL();

	// specify viewport of OpenGL in window
	glViewport(0, 0, 800, 800);

	// load in the shaders
	Shader shaderProgram("default.vert", "default.frag");

	// create the Vertex Array Object and bind it
	VAO VAO1;
	VAO1.Bind();

	// instantiate VAO and EBO
	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	// link VBO to VAO then unbind so its not modifiable
	VAO1.LinkVBO(VBO1, 0);
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	while (!glfwWindowShouldClose(window)) 
	{
		// get our color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// activate shader program
		shaderProgram.Activate();

		// bind the VAO to use this specifically
		VAO1.Bind();

		// specify primitive, starting index of vertices, and vertex count
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

		// swap to show changes
		glfwSwapBuffers(window);

		// handles all GLFW events
		glfwPollEvents();
	}

	// clean up objects we created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	// clean up once done
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}