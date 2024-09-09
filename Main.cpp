#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>	

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Texture.h"

// vertices to draw an equilateral triangle
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
	-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
	 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
	 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
};

GLuint indices[] =
{
	0, 2, 1, // Upper triangle
	0, 3, 2 // Lower triangle
};

int main()
{
	glfwInit();

	// tell GLFW the version and profile we are using of OPENGL
	// we use CORE because that has the modern functions
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
	// openGL will automatically create gradients if vertices have different colors
	// this is called interpolation
	// parameters VBO, layout index, number of components, stride, and offset
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	// unbing to stop accidental modification
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// get ID of uniform scale
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// texture parammeters
	int widthImg, heightImg, numColCh;

	// stbi and opengl are reversed vertically so we must flip to right side up
	stbi_set_flip_vertically_on_load(true);

	Texture penguinTex("penguin.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	penguinTex.texUnit(shaderProgram, "tex0", 0);

	while (!glfwWindowShouldClose(window)) 
	{
		// get our color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// activate shader program
		shaderProgram.Activate();

		// assign value to uniform which must be done after activating shader program
		glUniform1f(uniID, 0.5f);

		// bind texture so it appears
		penguinTex.Bind();

		// bind the VAO to use this specifically
		VAO1.Bind();

		// specify primitive, starting index of vertices, and vertex count
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// swap to show changes
		glfwSwapBuffers(window);

		// handles all GLFW events
		glfwPollEvents();
	}

	// clean up objects we created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	penguinTex.Delete();
	shaderProgram.Delete();

	// clean up once done
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}