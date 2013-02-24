package org.pielot.openal;

/**
 * This class represents an OpenAL source. Sources are used to play back the
 * data from a buffer. The source stores all data of the play back, such as the
 * sound's position or its pitch. Each source is assigned to one buffer.
 * However, several source can share a single buffer.
 * @author Martin Pielot
 */
public class Source {

	// ========================================================================
	// Fields
	// ========================================================================

	private int		sourceId;
	private Buffer	buffer;

	// ========================================================================
	// Constructor
	// ========================================================================

	/**
	 * Creates a new sound source for the given buffer.
	 * @param buffer the buffer
	 */
	public Source(Buffer buffer) {
		this.buffer = buffer;
		this.sourceId = OpenAlBridge.addSource(buffer.getId());
	}

	// ========================================================================
	// Methods
	// ========================================================================	
	
	public void setPosition(float x, float y, float z) {
		OpenAlBridge.setPosition(sourceId, x, y, z);
	}
	
	public void setPitch(float pitch) {
		OpenAlBridge.setPitch(sourceId, pitch);
	}
	
	public void setGain(float gain) {
		OpenAlBridge.setGain(sourceId, gain);
	}
	
	public void setRolloffFactor(float rollOff) {
		OpenAlBridge.setRolloffFactor(sourceId, rollOff);
	}
	
	public void play(boolean loop) {
		OpenAlBridge.play(sourceId, loop);
	}
	
	public void stop() {
		OpenAlBridge.stop(sourceId);
	}

	public void release() {
		OpenAlBridge.releaseSource(sourceId);
	}

	public String toString() {
		return "source " + sourceId + " playing " + buffer;
	}

}
