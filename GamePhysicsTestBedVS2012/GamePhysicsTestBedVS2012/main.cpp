#include "PhysicsDemo.h"
#include "FreeGLUTCallbacks.h"
#include "al.h"
#include "alc.h"
#include "Sound.h"
#include "Game.h"

int main(int argc, char** argv)
{
	ALCdevice* device = NULL;
	ALCcontext* context = NULL;
	Sound *s;

	Game game;
	return glutmain(argc, argv, 1024, 768, "Game Physics 2012 Test Bed", &game);


}