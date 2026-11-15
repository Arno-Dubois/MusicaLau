#include "Audio/MusicFileReader.h"
#include "Utils/Log.h"
#include <SDL3/SDL.h>
#include <fstream>
#include <iostream> // For debugging
#include <sstream>
#include <string>
#include <vector>

std::vector<MusicalEvent> parseMusicFile(const std::string &filePath) {
  std::vector<MusicalEvent> events;
  std::ifstream file(filePath);
  std::string line;

  if (!file.is_open()) {
    std::cerr << "Error: Could not open file " << filePath << std::endl;
    return events; // Return empty vector on error
  }

  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string pitchPart;
    float durationPart;

    ss >> pitchPart;
    if (ss >> durationPart) {
      if (pitchPart == "Unknown") { // Skip "Unknown" lines or treat as silence
        // For now, let's treat it as a silence of specified duration
        events.push_back({"0", durationPart});
      } else {
        events.push_back({pitchPart, durationPart});
      }
    } else {
      std::cerr << "Warning: Could not parse line: " << line << std::endl;
    }
  }
  file.close();
  return events;
}

// Assuming 'audioEngine' is an instance of YourApp::Audio::SDLAudioEngine
// and 'currentInstrument' is a std::string like "Piano", "Xylophone", etc.

void playSong(MusicApp::Audio::SDLAudioEngine &audioEngine,
              const std::vector<MusicalEvent> &songEvents,
              const std::string &instrumentName) {
  if (songEvents.empty()) {
    MUSICALAU_LOG("No events to play.");
    return;
  }

  for (const auto &event : songEvents) {
    int delayMilliseconds = static_cast<int>(event.durationSeconds * 1000.0f);

    if (event.pitchName != "0") {
      // It's a note
      MusicApp::Core::Note noteToPlay(event.pitchName);
      audioEngine.playSound(instrumentName, noteToPlay,
                            1.0f); // Using default velocity 1.0f

      if (delayMilliseconds > 0) {
        SDL_Delay(delayMilliseconds);
      }
      audioEngine.stopSound(instrumentName, noteToPlay);

    } else {
      // It's a silence
      if (delayMilliseconds > 0) {
        SDL_Delay(delayMilliseconds);
      }
    }
    // Small delay between events if needed, or to ensure stopSound processes
    SDL_Delay(10); // e.g., 10ms, adjust as needed
  }
  MUSICALAU_LOG("Playback finished.");
}