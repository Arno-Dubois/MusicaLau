#ifndef MUSICAPP_INSTRUMENTS_GUITARINSTRUMENT_H
#define MUSICAPP_INSTRUMENTS_GUITARINSTRUMENT_H

#include "../Core/Instrument.h"
#include "../Audio/AudioEngine.h" // For AudioEngine parameter in playNote
#include <string>

namespace MusicApp {
namespace Instruments {

/**
 * @brief A Guitar instrument.
 *
 * This instrument tells the AudioEngine to play a note, identifying itself
 * as "Guitar". The actual sound generation is handled by the AudioEngine.
 */
class GuitarInstrument : public Core::Instrument {
public:
    GuitarInstrument();
    ~GuitarInstrument() override = default;


    void playNote(const Core::Note& note, Audio::AudioEngine& audioEngine) const override;
};
} // namespace Instruments
} // namespace MusicApp

#endif // MUSICAPP_INSTRUMENTS_GUITARINSTRUMENT_H
