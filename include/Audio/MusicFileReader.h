#ifndef MUSICFILEREADER_H
#define MUSICFILEREADER_H

#include <string>
#include <vector>
#include "SDLAudioEngine.h"

// Assuming Core::Note and SDLAudioEngine are accessible
// #include "Note.h"
// #include "SDLAudioEngine.h"

struct MusicalEvent {
    std::string pitchName; // Note name like "C4", "A#5", or "0" for silence
    float durationSeconds; // Duration of the note or silence
};

std::vector<MusicalEvent> parseMusicFile(const std::string& filePath);

// Assuming 'audioEngine' is an instance of YourApp::Audio::SDLAudioEngine
// and 'currentInstrument' is a std::string like "Piano", "Xylophone", etc.

void playSong(MusicApp::Audio::SDLAudioEngine& audioEngine,
              const std::vector<MusicalEvent>& songEvents,
              const std::string& instrumentName);


#endif //MUSICFILEREADER_H
