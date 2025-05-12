#include "SimpleSynthInstrument.h"

// Constructor
MusicApp::Instruments::SimpleSynthInstrument::SimpleSynthInstrument(){
    setName("Synth");
}

// Plays a note using the provided audio engine
void MusicApp::Instruments::SimpleSynthInstrument::playNote(
    const Core::Note& note,
    Audio::AudioEngine& audioEngine) const {
    // The instrument tells the audio engine to play a sound,
    // identifying itself by name and passing the note details.
    // The audio engine is responsible for how this translates to actual audio.
    audioEngine.playSound(getName(), note);
}
