#include "../../include/Instruments/PianoInstrument.h"

namespace MusicApp {
namespace Instruments {

// Constructor
    PianoInstrument::PianoInstrument(){
        setName("Piano");
    }

// Plays a note using the provided audio engine
void PianoInstrument::playNote(
    const Core::Note& note,
    Audio::AudioEngine& audioEngine) const {
    // The instrument tells the audio engine to play a sound,
    // identifying itself by name and passing the note details.
    audioEngine.playSound(getName(), note);
}

} // namespace Instruments
} // namespace MusicApp
