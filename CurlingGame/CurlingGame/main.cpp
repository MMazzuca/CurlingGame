#include <iostream>
#include <gl/glut.h>
#include <gl/glu.h>

int width = 800;
int height = 600;

void drawScene()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport (0, 0, width, height);
	glLoadIdentity();
	gluLookAt (0.0, 0.0, 30.0,
			0.0, 0.0, 0.0,
			0.0, 1.0, 0.0);

	glutPostRedisplay();
	glutSwapBuffers();
}

void resize(int h, int w)
{
	glViewport (0,0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5,5,-5,5,5,250);
	glMatrixMode(GL_MODELVIEW);

	width = w;
	height = h;
}

int main(int argc, char **argv) 
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); 
   glutInitWindowSize(400, 400);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow ("CurlingGame.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   //glutKeyboardFunc(keyInput);
   //glutSpecialFunc(specialKeyInput);
  // glutPostRedisplay();
   glutMainLoop();

   return 0;  
}