package org.pielot.openal;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;

import android.app.Activity;

/**
 * This class is the allows managing OpenAL buffers. OpenAL uses buffers to
 * cache audio data in the memory so it is ready to be played. Instances of this
 * class can be created by passing the paths of a PCM .wav file to
 * #createBuffer(..). The audio data remains in the memory until the buffer is
 * released via #release(). Buffers cannot be played directly. Instead they have
 * to be attached to OpenAL sources.
 * @author Martin Pielot
 */
public class Buffer {

	// ========================================================================
	// Fields
	// ========================================================================

	private String	name;
	private int		bufferId;

	// ========================================================================
	// Constructor
	// ========================================================================

	private Buffer(String name, String path) {
		this.name = name;
		this.bufferId = OpenAlBridge.addBuffer(path);
	}

	/**
	 * Creates a new buffer by specifying the file path of the .wav file
	 * @param name Name of the buffer that can be used to retrieve the buffer via {@link SoundEnv#findBufferByName(String)}.
	 * @param path Path of the file containing the .wav sound
	 * @throws IOException if the sound file cannot be found
	 */
	public static Buffer createFrom(String name, String path)
			throws IOException {
		File file = new File(path);
		if (!file.exists())
			throw new FileNotFoundException("sound file "
					+ name
					+ " could not be found at "
					+ file.getAbsolutePath());
		return new Buffer(name, path);
	}

	/**
	 * Creates a new buffer with a sound file stored in the assets folder of the
	 * Android project.
	 * @param activity the calling activity, which is required to get access to
	 *            the assets folder.
	 * @param name Name of the sound file without file extension = "lake"
	 *            instead of "lake.wav". This will also become the name of the
	 *            buffer.
	 * @throws IOException if the sound file cannot be found
	 */
	public static Buffer createFrom(Activity activity, String name)
			throws IOException {
		String path = OpenAlBridge.getWavPath(activity, name);
		return new Buffer(name, path);
	}

	// ========================================================================
	// Methods
	// ========================================================================

	/**
	 * @return Name of the buffer that can be used to retrieve the buffer via {@link SoundEnv#findBufferByName(String)}.
	 */
	public String getName() {
		return name;
	}

	/**
	 * @return The ID of the buffer assigned by the OpenAL API.
	 */
	public int getId() {
		return bufferId;
	}

	/**
	 * Releases the buffer and frees the allocated memory.
	 */
	public void release() {
		OpenAlBridge.releaseBuffer(bufferId);
	}

	/**
	 * @return Returns the name and the OpenAL ID for this buffer
	 */
	public String toString() {
		return name + " " + bufferId;
	}
}
