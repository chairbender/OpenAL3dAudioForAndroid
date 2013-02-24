
#include <AL/al.h>
#include <AL/alc.h>

#include <jni.h>

#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include <android/log.h>




#define SUCCESS 1
#define ERROR 0

#define LOG_VERBOSE(...) // __android_log_print(ANDROID_LOG_VERBOSE, "OpenALNative", __VA_ARGS__);
#define LOG_DEBUG(...) __android_log_print(ANDROID_LOG_DEBUG, "OpenALNative", __VA_ARGS__);
#define LOG(...) __android_log_print(ANDROID_LOG_INFO, "OpenALNative", __VA_ARGS__);
#define LOG_WARN(...) __android_log_print(ANDROID_LOG_WARN, "OpenALNative", __VA_ARGS__);
#define LOG_ERROR(...) __android_log_print(ANDROID_LOG_ERROR, "OpenALNative", __VA_ARGS__);


#define AL_CHECK(...) \
{ \
	if ((err = alGetError()) != AL_NO_ERROR) { \
		LOG_WARN("OpenAL error: %x at %s(%i)", err, __FILE__, __LINE__); \
		check_error(); \
		return ERROR; \
	} \
}
	
#define EXCEPTION(...) \
{ \
	LOG_ERROR(__VA_ARGS__); \
	return ERROR; \
}

#define INITIALIZED context != NULL & device != NULL
	

// Global reference to Context and Device
static ALCdevice* device = NULL;
static ALCcontext* context = NULL;
static const ALint context_attribs[] = { ALC_FREQUENCY, 22050, 0 };
static ALenum err;





typedef struct {
  char  riff[4];//'RIFF'
  unsigned int riffSize;
  char  wave[4];//'WAVE'
  char  fmt[4];//'fmt '
  unsigned int fmtSize;
  unsigned short format;
  unsigned short channels;
  unsigned int samplesPerSec;
  unsigned int bytesPerSec;
  unsigned short blockAlign;
  unsigned short bitsPerSample;
  char  data[4];//'data'
  unsigned int dataSize;
}BasicWAVEHeader;



// Initialization & Release
int init();
int close();

// Buffer handling
int addBuffer(char* fnameptr);
int releaseBuffer(int bufferId);

// Source handling
int addSource(int bufferId);
int releaseSource(int sourceId);
int setPositon(int sourceId, float x, float y, float z);
int setPitch(int sourceId, float pitch);
int setGain(int sourceId, float gain);
int setRolloff(int sourceId, float rolloff);
int play(int sourceId, int loop);
int stop(int sourceId);

// Listener handling
int setListenerPosition(float x, float y, float z);
int setListenerOrientation(float xAt, float yAt, float zAt, float xUp, float yUp, float zUp);

// Helpers
char* readWav(char*,BasicWAVEHeader*);
ALuint createBufferFromWave(char*,BasicWAVEHeader);
int check_error();
