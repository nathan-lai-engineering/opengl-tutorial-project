#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

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
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.5,
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.5,
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.5,
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

	// create a vertex shader by compiling the source code at the top
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// create a fragment shader by compiling the source code at the top
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// create and wrap the shader program 
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// delete old shaders once saved in program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Vertex Array Object helps points to the VBO references
	// Vertex Buffer Object, typically an array of references
	GLuint VAO, VBO;

	// generate VAO before VBO
	glGenVertexArrays(1, &VAO);

	// generate 1 buffer object
	glGenBuffers(1, &VBO);

	// bind the VAO to work with it
	glBindVertexArray(VAO);

	// bind the VBO, making it the current object to make it modifiable
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// STREAM = modify once, use a few times
	// STATIC = modify once, use many many times
	// DYNAMIC = modify many times, use many times
	// Pack the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// configure VAO to work with VBO
	// 0 is index of vertex attribute, 3 vertex values, type of value is float
	// integer coordinates is false, amount of data between vertex is just 3 floats,
	// offset is pointer to beginning of array, which is the start of the array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// enable the vertex attribute array at position 0
	glEnableVertexAttribArray(0);

	// unbind the VAO and VBO to avoid accidentally changing it by binding to 0
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// specify the color of the background to put in back buffer
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	// clear the back buffer and assign the background to it
	glClear(GL_COLOR_BUFFER_BIT); 

	// swap back and front buffer
	glfwSwapBuffers(window);

	while (!glfwWindowShouldClose(window)) 
	{
		// get our color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// activate shader program
		glUseProgram(shaderProgram);

		// bind the VAO to use this specifically
		glBindVertexArray(VAO);

		// specify primitive, starting index of vertices, and vertex count
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// swap to show changes
		glfwSwapBuffers(window);

		// handles all GLFW events
		glfwPollEvents();
	}

	// clean up objects we created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// clean up once done
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}