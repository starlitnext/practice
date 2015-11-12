#include "glut.h"

static GLint vertics[] = {
	25, 25,
	100, 325,
	175, 25,
	175, 325,
	250, 25,
	325, 325
};

static GLfloat colors[] = {
	1.0, 0.2, 0.2,
	0.2, 0.2, 1.0,
	0.8, 1.0, 0.2,
	0.75, 0.75, 0.75,
	0.35, 0.35, 0.35,
	0.5, 0.5, 0.5
};

static GLfloat intertwined[] = {
	1.0, 0.2, 0.2, 25, 25,
	0.2, 0.2, 1.0, 100, 325,
	0.8, 1.0, 0.2, 175, 25,
	0.75, 0.75, 0.75, 175, 325,
	0.35, 0.35, 0.35, 250, 25,
	0.5, 0.5, 0.5, 325, 325
};
	
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	// step 1，启用数组
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	// step 2，指定数组数据
	// glColorPointer(3, GL_FLOAT, 0, colors);
	// glVertexPointer(2, GL_INT, 0, vertics);
	glColorPointer(3, GL_FLOAT, 5 * sizeof(GLfloat), &intertwined[0]);
	glVertexPointer(2, GL_FLOAT, 5 * sizeof(GLfloat), &intertwined[3]);
	
	// step 3, 解引用和渲染
	glBegin(GL_TRIANGLES);
	glArrayElement(2);
	glArrayElement(3);
	glArrayElement(5);
	glEnd();
	
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
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
	glutCreateWindow("顶点数组");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}