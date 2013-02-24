/****************************************************************************
 * OpenAl
 ****************************************************************************/
package org.pielot.openal;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import android.app.Activity;
import android.util.Log;

/**
 * This class provides an interface to the native implementation of OpenAL and
 * some convenience functions.
 * @author Martin Pielot
 */
public class OpenAlBridge {

	// ========================================================================
	// Constant Fields
	// ========================================================================

	private static final String	TAG		= "OpenAL";

	public static final int		SUCCESS	= 1;
	public static final int		ERROR	= 0;

	// ========================================================================
	// Native Methods
	// ========================================================================

	// Initialization & Release
	// -----------------------------------

	static native int init();

	static native int close();

	// Buffer Handling
	// -----------------------------------

	static native int addBuffer(String filename);

	static native int releaseBuffer(int bufferId);

	// Source Handling
	// -----------------------------------

	static native int addSource(int bufferId);

	static native int releaseSource(int sourceId);

	static native void setPosition(int sourceId, float x, float y, float z);

	static native void setPitch(int sourceId, float pitch);

	static native void setGain(int sourceId, float gain);

	static native void setRolloffFactor(int sourceId, float rollOff);

	static native int play(int sourceId, boolean loop);

	static native int stop(int sourceId);

	// Listener Handling
	// -----------------------------------

	static native int setListenerPos(float x, float y, float z);

	static native int setListenerOrientation(float xAt, float yAt, float zAt);

	// ========================================================================
	// Helpers Methods
	// ========================================================================

	public static String str(int retVal) {
		if (retVal == SUCCESS)
			return "SUCCESS";
		else if (retVal == ERROR)
			return "ERROR";

		return "UNKNOWN";
	}

	public static String getWavPath(Activity activity, String name)
			throws IOException {

		String filename = name + ".wav";
		File file = new File(activity.getFilesDir() , filename);

		if (!file.exists()) {
			Log.w(TAG, file + " not found, copying from assets");
			retrieveFromAssets(activity, filename);
		} else {
		}

		return file.getAbsolutePath();
	}


	private static void retrieveFromAssets(Activity activity, String filename)
			throws IOException {

		InputStream is = activity.getAssets().open(filename);

		// Destination
		File outFile = new File(activity.getFilesDir(), filename);

		Log.i(TAG, "retrieveFromAssets( .. ) copying "
				+ filename
				+ " to "
				+ outFile.getParent());

		FileOutputStream fos = new FileOutputStream(outFile);

		// transfer bytes from the inputfile to the outputfile
		byte[] buffer = new byte[1024];
		int length;
		while ((length = is.read(buffer)) > 0) {
			fos.write(buffer, 0, length);
		}

		// Close the streams
		fos.flush();
		fos.close();
		is.close();

	}

}
