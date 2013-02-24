package org.pielot.openal;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.util.Log;

/**
 * The sound environment which hosts all sound buffers and sound sources. It
 * also allows to define the position and the orientation of the listener.
 * @author Martin Pielot
 */
public class SoundEnv {

	// ========================================================================
	// Fields
	// ========================================================================

	private static final String	TAG	= "SoundEnv";

	private Activity			activity;

	private List<Buffer>		buffers;
	private List<Source>		sources;

	private boolean				released;

	// ========================================================================
	// Singleton
	// ========================================================================

	private SoundEnv(Activity activity) {
		this.activity = activity;
		System.loadLibrary("openal");
		System.loadLibrary("openalwrapper");
		this.buffers = new ArrayList<Buffer>();
		this.sources = new ArrayList<Source>();
		OpenAlBridge.init();
		Log.i(TAG, "Initializing Sound Environment");
	}

	private static SoundEnv	instance;

	public static SoundEnv getInstance(Activity activity) {
		if (instance == null)
			instance = new SoundEnv(activity);

		return instance;
	}

	// ========================================================================
	// Methods
	// ========================================================================

	/**
	 * Creates a new buffer with a sound file stored in the assets folder of the
	 * Android project and adds it to the internal list of active buffers.
	 * @param name Name of the sound file without file extension = "lake"
	 *            instead of "lake.wav". This will also become the name of the
	 *            buffer.
	 * @throws IOException if the sound file cannot be found
	 */
	public Buffer addBuffer(String name) throws IOException {
		Buffer buffer = Buffer.createFrom(activity, name);
		Log.d(TAG, "addBuffer( " + buffer + " )");
		this.buffers.add(buffer);
		return buffer;
	}

	/**
	 * Creates a new buffer by specifying the file path of the .wav file and
	 * adds it to the internal list of active buffers.
	 * @param name Name of the buffer that can be used to retrieve the buffer
	 *            via {@link SoundEnv#findBufferByName(String)}.
	 * @param path Path of the file containing the .wav sound
	 * @throws IOException if the sound file cannot be found
	 */
	public Buffer addBuffer(String name, String path) throws IOException {
		Buffer buffer = Buffer.createFrom(name, path);
		Log.d(TAG, "addBuffer( " + buffer + " )");
		this.buffers.add(buffer);
		return buffer;
	}

	/**
	 * Allows retrieving a buffer by the name specified on its creation
	 * @param name name of the buffer
	 * @return the buffer of NULL if no buffer with the given name was found
	 */
	public Buffer findBufferByName(String name) {
		for (Buffer buffer : buffers) {
			if (name.equals(buffer.getName())) {
				return buffer;
			}
		}
		return null;
	}

	/**
	 * Creates a new sound source for the given buffer and adds it to the
	 * internal list of sources
	 * @param buffer the buffer
	 */
	public Source addSource(Buffer buffer) {
		Source source = new Source(buffer);
		Log.d(TAG, "addSource( " + source + " )");
		this.sources.add(source);
		return source;
	}

	/**
	 * Moves the listener to the given coordinates.
	 */
	public void setListenerPos(float x, float y, float z) {
		OpenAlBridge.setListenerPos(x, y, z);
	}

	/**
	 * Rotates the listener to face into the given direction. For simplification
	 * this method assumes that the listener always stands upright, like a
	 * person when walking. In fact, OpenAL also allows rotating the listener
	 * around any of the three possible axis.
	 * @param heading the direction the listener should face
	 */
	public void setListenerOrientation(double heading) {
		double zv = -Math.cos(Math.toRadians(heading));
		double xv = Math.sin(Math.toRadians(heading));
		this.setListenerOrientation((float) xv, 0, (float) zv);
	}

	/**
	 * Rotates the listener to face into the given direction.
	 */
	public void setListenerOrientation(float xv, float yv, float zv) {
		OpenAlBridge.setListenerOrientation(xv, yv, zv);
	}

	/**
	 * Plays all registered sources (useful for debugging).
	 * @param loop 'true' = endlessly repeated, 'false' = played once only
	 */
	public void playAllSources(boolean loop) {
		for (Source source : sources)
			source.play(loop);
	}

	/**
	 * Stops the playback of all registered sources.
	 */
	public void stopAllSources() {
		for (Source source : sources)
			source.stop();
	}

	/**
	 * Releases all sources and buffers and closes the OpenAL device.
	 */
	public synchronized void release() {
		if (!released) {
			Log.i(TAG, "release()");

			for (Source source : sources)
				source.stop();

			for (Source source : sources)
				source.release();

			for (Buffer buffer : buffers) {
				buffer.release();
			}

			OpenAlBridge.close();

			released = true;
		}
	}

	private boolean	memoryLow;

	public void onLowMemory() {
		Log.w(TAG, "memory is low, stopping to add buffers");
		this.memoryLow = true;
	}

	void testMaxBuffers() {
		try {
			Log.i(TAG, "testMaxBuffers()");

			int i = 0;
			do {
				i++;
				Buffer b = addBuffer("lake");
				Log.d(TAG, "\tbuffer" + i + " = " + b);
				Thread.sleep(10);
			} while (!memoryLow);
			Log.i(TAG, "allocated " + i + " buffers");

		} catch (Exception e) {
			Log.e(TAG, e + " in testEnv()", e);
		}
	}

	void testEnv0() {
		try {
			Log.i(TAG, "testEnv()");

			Buffer lake = addBuffer("lake");
			Buffer park = addBuffer("park");
			Buffer building = addBuffer("building");

			Source lake1 = addSource(lake);
			Source lake2 = addSource(lake);
			Source park1 = addSource(park);
			Source building1 = addSource(building);

			lake1.setPosition(0, 0, -10);
			lake2.setPosition(-6, 0, 4);
			park1.setPosition(6, 0, -12);
			building1.setPosition(10, 0, 15);

			lake2.setPitch(1.1f);

			lake1.play(true);
			lake2.play(true);
			park1.play(true);
			building1.play(true);

		} catch (Exception e) {
			Log.e(TAG, e + ", please copy file into 'assets' folder");
		}
	}

}
