
#include "org_pielot_openal_OpenAlBridge.h"
#include "openalwrapper.h"


JNIEXPORT jint JNICALL Java_org_pielot_openal_OpenAlBridge_init
	(JNIEnv * env, jclass klass) {
		init();
	}

JNIEXPORT jint JNICALL Java_org_pielot_openal_OpenAlBridge_addBuffer 
	(JNIEnv * env, jclass klass, jstring filename) {	
		const char* fnameptr = (*env)->GetStringUTFChars(env, filename, NULL); /* Get the file name */
		return addBuffer(fnameptr);
	}
	
JNIEXPORT jint JNICALL Java_org_pielot_openal_OpenAlBridge_releaseBuffer
	(JNIEnv * env, jclass klass, jint bufferId) {
		return releaseBuffer(bufferId);
	}


JNIEXPORT jint JNICALL Java_org_pielot_openal_OpenAlBridge_addSource 
	(JNIEnv * env, jclass klass, jint bufferId){	
		return addSource(bufferId);
	}
	
JNIEXPORT jint JNICALL Java_org_pielot_openal_OpenAlBridge_releaseSource
	(JNIEnv * env, jclass klass, jint sourceId) {
		return releaseSource(sourceId);
	}


JNIEXPORT void JNICALL Java_org_pielot_openal_OpenAlBridge_setPosition 
	(JNIEnv * env, jclass klass, jint sourceId, jfloat x, jfloat y , jfloat z){
		setPositon(sourceId, x, y, z);		
	}

JNIEXPORT void JNICALL Java_org_pielot_openal_OpenAlBridge_setPitch
	(JNIEnv * env, jclass klass, jint sourceId, jfloat pitch) {
		setPitch(sourceId, pitch);
	}
	
JNIEXPORT void JNICALL Java_org_pielot_openal_OpenAlBridge_setGain
	(JNIEnv * env, jclass klass, jint sourceId, jfloat gain) {
		setGain(sourceId, gain);
	}
	
JNIEXPORT void JNICALL Java_org_pielot_openal_OpenAlBridge_setRolloffFactor
	(JNIEnv * env, jclass klass, jint sourceId, jfloat rolloff) {
		setRolloff(sourceId, rolloff);
	}


JNIEXPORT jint JNICALL Java_org_pielot_openal_OpenAlBridge_play 
	(JNIEnv * env, jclass klass, jint sourceId, jboolean loop){
		return play(sourceId, loop);
	 }
	 
JNIEXPORT jint JNICALL Java_org_pielot_openal_OpenAlBridge_stop
	(JNIEnv * env, jclass klass, jint sourceId) {
		return stop(sourceId);
	}

JNIEXPORT jint JNICALL Java_org_pielot_openal_OpenAlBridge_setListenerPos
	(JNIEnv * env, jclass klass, jfloat x, jfloat y , jfloat z) {
		return setListenerPosition(x, y, z);
	}

JNIEXPORT jint JNICALL Java_org_pielot_openal_OpenAlBridge_setListenerOrientation
	(JNIEnv * env, jclass klass, jfloat xAt, jfloat yAt, jfloat zAt) {
		return setListenerOrientation(xAt, yAt, zAt, 0, 1, 0);
	}

JNIEXPORT jint JNICALL Java_org_pielot_openal_OpenAlBridge_close 
	(JNIEnv * env, jclass klass) {
		return close();
	}
