#include"VBO.h"

VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{
	// generate 1 buffer object
	glGenBuffers(1, &ID);

	// bind the VBO, making it the current object to make it modifiable
	glBindBuffer(GL_ARRAY_BUFFER, ID);

	// STREAM = modify once, use a few times
	// STATIC = modify once, use many many times
	// DYNAMIC = modify many times, use many times
	// Pack the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}