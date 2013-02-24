#include "openalwrapper.h"


// Initialization & Release
// -----------------------------------

int init() {
	LOG("\n============================================TESTEST");
	LOG_DEBUG("init()");

	if (device && context) {
		LOG("Already initialized");
		return SUCCESS;
	}
		
	device = alcOpenDevice(NULL);		
    if (!device) {
        EXCEPTION("Failed to open OpenAL Device");
    }
   
	context = alcCreateContext(device, context_attribs);		
    if (!context) {
        EXCEPTION("Failed to create OpenAL Context");
    }
	
    alcMakeContextCurrent(context);
	
	LOG("Device and context successfully initialized");
	return SUCCESS;
}

int close() {
	LOG("close()");

    if (context) {
        alcMakeContextCurrent(NULL);
        alcDestroyContext(context);
		context = NULL;
    }
	
	if (device) {
        alcCloseDevice(device);
		device = NULL;
    }

	return SUCCESS;
}



// Buffer Handling
// -----------------------------------

int addBuffer(char* fnameptr) {
	LOG_DEBUG("addBuffer( %s )", fnameptr);
	
	BasicWAVEHeader header;
	
	char* data = readWav(fnameptr,&header);
	if (data){
		//Now We've Got A Wave In Memory, Time To Turn It Into A Usable Buffer
		ALuint buffer = createBufferFromWave(data,header);
		if (buffer){
			return (jint) buffer;
			
		} else LOG_ERROR("Failed to create buffer");
		free(data);
		
	} else LOG_ERROR("Failed to read file");
	
	return ERROR;
}

int releaseBuffer(int bufferId) {
	LOG_DEBUG("releaseBuffer( %i )", bufferId);
	ALuint buffer = bufferId;	
	alDeleteBuffers(1, &buffer);
	
	AL_CHECK();
	return SUCCESS;
}

// Source Handling
// -----------------------------------

int addSource(int bufferId) {
	
	ALuint src = 0;
	alGenSources(1, &src );
		
	LOG_DEBUG("addSource( %i ) -> id %i", bufferId, src);	
	AL_CHECK();
	
	alSourcei(src, AL_BUFFER, bufferId);
	alSourcef(src, AL_ROLLOFF_FACTOR,  0.25);
	
	AL_CHECK();
	return src;
}

int releaseSource(int sourceId) {
	LOG_DEBUG("releaseSource( %i )", sourceId);	
	
	ALuint source = sourceId;	
	alDeleteSources(1, &source);
	
	AL_CHECK();
	return SUCCESS;
}

int setPositon(int sourceId, float x, float y, float z) {
	if(INITIALIZED) {
		// LOG_VERBOSE("setPositon( %i, %.2f, %.2f, %.2f )", sourceId, x, y, z);
		float pos [] = {x, y, z};
		alSourcefv(sourceId, AL_POSITION, pos);
		
		AL_CHECK();
	}
	return SUCCESS;
}

int setPitch(int sourceId, float pitch) {
	if(INITIALIZED) {
		LOG_DEBUG("setPitch( %i, %.2f )", sourceId, pitch);
		alSourcef(sourceId, AL_PITCH, pitch);
		
		AL_CHECK();
	}
	return SUCCESS;
}

int setGain(int sourceId, float gain) {
	if(INITIALIZED) {
		LOG_DEBUG("setGain( %i, %.2f )", sourceId, gain);
		alSourcef(sourceId, AL_GAIN, gain);
		
		AL_CHECK();
	}
	return SUCCESS;
}

int setRolloff(int sourceId, float rolloff) {
	if(INITIALIZED) {
		LOG_DEBUG("setRolloff( %i, %.2f )", sourceId, rolloff);
		alSourcef(sourceId, AL_ROLLOFF_FACTOR, rolloff);
		
		AL_CHECK();
	}
	return SUCCESS;
}

int play(int sourceId, int loop) {
	if(INITIALIZED) {
		LOG_DEBUG("play( %i, %i )", sourceId, loop);
		alSourcei (sourceId, AL_LOOPING,  loop);
		alSourcePlay(sourceId);
		
		AL_CHECK();
	}
	return SUCCESS;
}

int stop(int sourceId) {
	if(INITIALIZED) {
		LOG_DEBUG("stop( %i )", sourceId);
		alSourceStop(sourceId);
		
		AL_CHECK();
	}
	return SUCCESS;
}


// Listener Handling
// -----------------------------------

int setListenerPosition(float x, float y, float z) {
	if(INITIALIZED) {
		// LOG_VERBOSE("setListenerPosition( %.2f, %.2f, %.2f )", x, y, z);
		float listenerPos []  = {x, y, z};
		alListenerfv(AL_POSITION, listenerPos);
		
		AL_CHECK();
	}
	return SUCCESS;
}


int setListenerOrientation(float xAt, float yAt, float zAt, float xUp, float yUp, float zUp) {
	if(INITIALIZED) {
		// LOG_VERBOSE("setListenerOrientation( %.2f, %.2f, %.2f, %.2f, %.2f, %.2f  )", xAt, yAt, zAt, xUp, yUp, zUp);
		float listenerOri []  = {xAt, yAt, zAt, xUp, yUp, zUp};
		alListenerfv(AL_ORIENTATION, listenerOri);
		
		AL_CHECK();
	}
	return SUCCESS;
}



// Initialization & Release
// -----------------------------------

//WARNING: This Doesn't Check To See If These Pointers Are Valid
// This method is from http://www.gamedev.net/community/forums/topic.asp?topic_id=505152&whichpage=1&#3296091
char* readWav(char* filename,BasicWAVEHeader* header){
  char* buffer = 0;
  //FILE* file = fopen("/sdcard/accessiblemap/lake.wav","rb");
  FILE* file = fopen(filename,"rb");
  if (!file) {
	LOG_WARN("readWAVE() - fopen failed");
	return 0;
  }
  
  if (fread(header,sizeof(BasicWAVEHeader),1,file)){
    if (!(//these things *must* be valid with this basic header
      memcmp("RIFF",header->riff,4) ||
      memcmp("WAVE",header->wave,4) ||
      memcmp("fmt ",header->fmt,4)  ||
      memcmp("data",header->data,4)
    )){
      buffer = (char*)malloc(header->dataSize);
      if (buffer){
        if (fread(buffer,header->dataSize,1,file)){
          fclose(file);
          return buffer;
        } else {
			LOG_WARN("readWAVE() - fread (inner) failed");
		}
        free(buffer);
      } else {
		LOG_WARN("readWAVE() - buffer not allocated");
	  }
    }else{
		LOG_WARN("readWAVE() - header not valid");
	}
  }else{
	LOG_WARN("readWAVE() - fread failed");
  }
  fclose(file);
  return 0;
}

// This method is from http://www.gamedev.net/community/forums/topic.asp?topic_id=505152&whichpage=1&#3296091
ALuint createBufferFromWave(char* data,BasicWAVEHeader header){

  ALuint buffer = 0;
  
  ALuint format = 0;
  switch (header.bitsPerSample){
    case 8:
      format = (header.channels == 1) ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
      break;
    case 16:
      format = (header.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
      break;
    default:
      LOG_WARN("readWAVE() - unsupported bitrate %i", header.bitsPerSample);
      return 0;
  }
  
  alGenBuffers(1,&buffer);
  alBufferData(buffer,format,data,header.dataSize,header.samplesPerSec);
  return buffer;
}

int check_error() {
	ALenum error = alGetError();

	if(error == AL_NO_ERROR)
		return SUCCESS;
						
	if(error == AL_INVALID_NAME)
		LOG_WARN("AL_INVALID_NAME, error code %i", error);
		
	if(error == AL_INVALID_ENUM)
		LOG_WARN("AL_INVALID_ENUM, error code %i", error);
	
	if(error == AL_INVALID_VALUE)
		LOG_WARN("AL_INVALID_VALUE, error code %i", error);
		
	if(error == AL_INVALID_OPERATION)
		LOG_WARN("AL_INVALID_OPERATION, error code %i", error);
	
	if(error == AL_OUT_OF_MEMORY)
		LOG_WARN("AL_OUT_OF_MEMORY, error code %i", error);	
		
	return ERROR;
}
