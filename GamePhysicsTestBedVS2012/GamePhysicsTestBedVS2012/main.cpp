#include "PhysicsDemo.h"
#include "FreeGLUTCallbacks.h"
#include "al.h"
#include "alc.h"
#include "Sound.h"

int main(int argc, char** argv)
{
	ALCdevice* device = NULL;
	ALCcontext* context = NULL;
	Sound *s;

	PhysicsDemo demo;
	return glutmain(argc, argv, 1024, 768, "Game Physics 2012 Test Bed", &demo);


}