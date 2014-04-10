#include "PhysicsDemo.h"
#include "FreeGLUTCallbacks.h"

int main(int argc, char** argv)
{
	PhysicsDemo demo;
	return glutmain(argc, argv, 1024, 768, "Game Physics 2012 Test Bed", &demo);
}