#include "glew.h"
#include "glut.h"

#define BUFFER_OFFSET(bytes)	((GLubyte*) NULL + (bytes))

#define VERTICES	0
#define INDICES		1
#define NUM_BUFFERS	2
GLuint buffers[NUM_BUFFERS];

GLfloat vertices[][3] = {
	{ -1.0, -1.0, -1.0 },
	{ 1.0, -1.0, -1.0 },
	{ 1.0, 1.0, -1.0 },
	{ -1.0, 1.0, -1.0 },
	{ -1.0, -1.0, 1.0 },
	{ 1.0, -1.0, 1.0 },
	{ 1.0, 1.0, 1.0 },
	{ -1.0, 1.0, 1.0 }
};

GLubyte indices[][4] = {
	{ 0, 1, 2, 3 },
	{ 4, 7, 6, 5 },
	{ 0, 4, 5, 1 },
	{ 3, 2, 6, 7 },
	{ 0, 3, 7, 4 },
	{ 1, 5, 6, 2 }
};
	
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	
	glGenBuffers(NUM_BUFFERS, buffers);
	
	glBindBuffer(GL_ARRAY_BUFFER, buffers[VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDICES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glBindBuffer(GL_ARRAY_BUFFER, buffers[VERTICES]);
	glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDICES]);
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));
	
	glFlush();
	
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("»º³åÇø¶ÔÏó");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}