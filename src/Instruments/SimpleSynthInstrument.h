#ifndef MUSICAPP_INSTRUMENTS_SIMPLESYNTHINSTRUMENT_H
#define MUSICAPP_INSTRUMENTS_SIMPLESYNTHINSTRUMENT_H

#include "../Core/Instrument.h"
#include "../Audio/AudioEngine.h" // For AudioEngine parameter in playNote
#include <string>

namespace MusicApp {
namespace Instruments {

/**
 * @brief A simple synthesizer instrument.
 *
 * This instrument tells the AudioEngine to play a note, identifying itself
 * as a "SimpleSynth". The actual sound generation is handled by the AudioEngine.
 */
class SimpleSynthInstrument : public Core::Instrument {
public:
    SimpleSynthInstrument();
    ~SimpleSynthInstrument() override = default;

    std::string getName() const override;

    void playNote(const Core::Note& note, Audio::AudioEngine& audioEngine) const override;

private:
    std::string name_;
};

} // namespace Instruments
} // namespace MusicApp

#endif // MUSICAPP_INSTRUMENTS_SIMPLESYNTHINSTRUMENT_H
