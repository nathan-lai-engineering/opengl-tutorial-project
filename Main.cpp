#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>	
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>


#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Texture.h"

const unsigned int width = 800;
const unsigned int height = 800;

// vertices to draw an equilateral triangle
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
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
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);

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
	glViewport(0, 0, width, height);

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

	// test for depth to avoid depth glitches
	glEnable(GL_DEPTH_TEST);

	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	while (!glfwWindowShouldClose(window)) 
	{
		// get our color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// activate shader program
		shaderProgram.Activate();

		// rotate by half a degree every 60th of a second
		double crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1 / 60) 
		{
			rotation += 0.5;
			prevTime = crntTime;
		}

		// local coordinates: origin same as origin of object
		// world coordinate: origin at center of world, contains objects
		// view coordinates: origin same origin as camera/viewport
		// clip coordinates: origin same as view, but clip(delete) coordinates
		// screen coordinates: everything is flattened to view on screen

		// initializing the matrix so it would have values, these are used to move
		// coordinate systems
		
		// model matrix: local coords -> world coords
		glm::mat4 model = glm::mat4(1.0f);

		// view matrix: world coords -> view coords
		glm::mat4 view = glm::mat4(1.0f);

		// projection matrix: view coords -> clip coords
		glm::mat4 proj = glm::mat4(1.0f);


		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));

		// translate the world a bit so we can see the object
		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));

		// 0.1f is minimum distance to redner, 100.0f is maxium distance to render
		// anything beyond that range is clipped off
		// degrees are converted to radians
		proj = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 100.0f);

		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		// assign value to uniform which must be done after activating shader program
		glUniform1f(uniID, 0.5f);

		// bind texture so it appears
		penguinTex.Bind();

		// bind the VAO to use this specifically
		VAO1.Bind();

		// specify primitive, starting index of vertices, and vertex count
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

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