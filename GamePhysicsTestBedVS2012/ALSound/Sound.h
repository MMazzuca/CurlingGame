#pragma once
#include "OpenAlSoft\include\AL\al.h"
#include "OpenAlSoft\include\AL\alc.h"
#include <iostream>
class Sound{
	public:
		bool overPlay;
		void* data;
		unsigned int bufferID;
		unsigned int sourceID;
		int format;
		int sampleRate;
		int size;
		Sound(int Format, int SampleRate, void *Data, int Size, bool OverPlay);//Overplay let's you play it on top of each other
		static Sound* loadWAVE(const char* fileName, bool OverPlay);
		void play();
};

//Instruction
/*
//Put this in your main or something(global variables)
ALCdevice* device = NULL;
ALCcontext* context = NULL;
Sound *s;

//Then put this in your setup or somewhere that is made after the window opens(I think it matters)
device = alcOpenDevice(NULL);
context = alcCreateContext(device,NULL);
alcMakeContextCurrent(context);

//This will load the wav(Put it in a setup as well
s = Sound::loadWAVE("Filename.wav",true);//The bool is for the over play option

s->play();//Plays the sound
*/