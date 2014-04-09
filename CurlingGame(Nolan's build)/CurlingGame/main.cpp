#include <iostream>
#include <glut.h>
//#include <gl/glu.h>
#include <gl/GLU.h>
#include "Rock.h"

int width = 800;
int height = 600;
bool KEYS[256];
//bool gameRunning = false;
char buffer[256];
Rock rock(1,0);

using namespace::std;

void keyboardDown(unsigned char key, int x, int y)
{
	KEYS[key] = true;
}

void keyboardUP(unsigned char key, int x, int y)
{
	KEYS[key] = false;
}

void preEvents()
{
	if(KEYS[' '])
	{
		if(rock.isThrown)rock.move();
		cout << "You da best" << endl;
	}

	else
	{
		rock.state = IDLE;
	}
}

void drawScene()
{
	preEvents();
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport (0, 0, width, height);
	glLoadIdentity();
	gluLookAt (0.0, 0.0, 30.0,
			0.0, 0.0, 0.0,
			0.0, 1.0, 0.0);

	rock.draw();
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
   glutIdleFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyboardDown);
   glutKeyboardUpFunc(keyboardUP);
  // glutPostRedisplay();
   glutMainLoop();

   return 0;  
}