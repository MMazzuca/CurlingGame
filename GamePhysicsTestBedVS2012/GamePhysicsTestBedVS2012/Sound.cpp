#include "Sound.h"
#include <Windows.h>


Sound::Sound(int Format, int SampleRate, void *Data, int Size, bool OverPlay){
	alGenBuffers(1, &bufferID);
	alBufferData(bufferID, Format, Data, Size, SampleRate);
	
	alGenSources(1, &sourceID);
	alSourcei(sourceID, AL_BUFFER, bufferID);
	this->format = Format;
	this->data = Data;
	this->size = Size;
	this->sampleRate = SampleRate;
	this->overPlay = OverPlay;
}
void Sound::play(){
	int state;
	alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
	if(state != AL_PLAYING || overPlay){
		alSourcePlay(sourceID);
		
	}
}
Sound* Sound::loadWAVE(const char* fileName, bool OverPlay){
	
	FILE *fp = NULL;
	fp = fopen(fileName, "rb");
	if(!fp){
		std::cout << "Could not find " << fileName << std::endl;
		fclose(fp);
		return NULL;
	}
	char* chunkID = new char[5];
	chunkID[4] = '\0';
	fread(chunkID, 4, sizeof(char), fp);
	if(strcmp(chunkID, "RIFF")){
		delete [] chunkID;
		std::cout << "Not RIFF" << std::endl;
		fclose(fp);
		return NULL;
	}
	fseek(fp, 8, SEEK_SET);
	char* format = new char[5];
	format[4] = '\0';
	fread(format, 4, sizeof(char), fp);
	std::cout << format << std::endl;
    if(strcmp(format, "WAVE")){
		delete [] chunkID;
		delete [] format;
		std::cout << "Not WAVE" << std::endl;
		fclose(fp);
		return NULL;
	}
	char* subChunkID = new char[5];
	subChunkID[4] = '\0';
	fread(subChunkID, 4, sizeof(char), fp);
	if(strcmp(subChunkID, "fmt ")){
		delete [] chunkID;
		delete [] format;
		delete [] subChunkID;
		std::cout << "Corrupt!" << std::endl;
		fclose(fp);
		return NULL;
	}
	DWORD subChunkSize;
	fread(&subChunkSize, 1, sizeof(unsigned int), fp);
	DWORD subChunkLocation = (unsigned int)ftell(fp) + subChunkSize;
	unsigned short audioFormat;
	fread(&audioFormat, 1, sizeof(unsigned short), fp);
	if(audioFormat != 1){
		delete [] chunkID;
		delete [] format;
		delete [] subChunkID;
		std::cout << "Not PCM!" << std::endl;
		fclose(fp);
		return NULL;
	}
	unsigned short numChannels;
	fread(&numChannels, 1, sizeof(unsigned short), fp);
	DWORD sampleRate;
	fread(&sampleRate, 1, sizeof(unsigned int), fp);

	fseek(fp,34,SEEK_SET);
	unsigned short bitsPerSample;
	fread(&bitsPerSample, 1, sizeof(unsigned short), fp);

	int ALformat;
	if(numChannels == 1 && bitsPerSample == 8){
		std::cout << "Mono8" << std::endl;
		ALformat = AL_FORMAT_MONO8;
	}
	else if(numChannels == 1 && bitsPerSample == 16){
		std::cout << "Mono16" << std::endl;
		ALformat = AL_FORMAT_MONO16;
	}
	else if(numChannels == 2 && bitsPerSample == 8){
		std::cout << "Stereo8" << std::endl;
		ALformat = AL_FORMAT_STEREO8;
	}
	else if(numChannels == 2 && bitsPerSample == 16){
		std::cout << "Stereo16" << std::endl;
		ALformat = AL_FORMAT_STEREO16;
	}
	else{
		std::cout << "Not proper format" << std::endl;
		delete [] chunkID;
		delete [] format;
		delete [] subChunkID;
		fclose(fp);
		return NULL;
	}

	fseek(fp, subChunkLocation, SEEK_SET);
	char* subChunk2ID = new  char[5];
	subChunk2ID[4] = '\0';
	fread(subChunk2ID, 4, sizeof(char), fp);
	if(strcmp(subChunk2ID, "data")){
		delete [] chunkID;
		delete [] format;
		delete [] subChunkID;
		delete [] subChunk2ID;
		std::cout << "Corrupt!" << std::endl;
		fclose(fp);
		return NULL;
	}
	DWORD subChunk2Size;
	fread(&subChunk2Size, 1, sizeof(unsigned int), fp);
	unsigned char* Data = new unsigned char[subChunk2Size];
	fread(Data, subChunk2Size, sizeof(unsigned char),fp);
	delete [] chunkID;
	delete [] format;
	delete [] subChunkID;
	delete [] subChunk2ID;
	fclose(fp);
		
	return new Sound(ALformat, sampleRate,Data,subChunk2Size, OverPlay);
}