#pragma once

#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <iostream> // For cerr/cout
#include "SDL3/SDL_timer.h" // For SDL_Delay
#include "MusicFileReader.h" // For MusicalEvent
#include "SDLAudioEngine.h"  // For SDLAudioEngine and Core::Note
// Note: Ensure SDLAudioEngine.h includes or forward declares Core::Note correctly if it's in a namespace

namespace MusicApp {
    namespace Audio {

        class SongPlayer {
        public:
            SongPlayer(MusicApp::Audio::SDLAudioEngine* audioEngine);
            ~SongPlayer();

            // Starts playing the given song events with the specified instrument.
            // Returns true if playback started, false otherwise (e.g., if already playing).
            bool playSong(const std::vector<MusicalEvent>& events, const std::string& instrumentName);

            // Stops the currently playing song, if any.
            void stopSong();

            bool isPlaying() const;

        private:
            void playbackLoop(); // The function that will run in the playback thread

            MusicApp::Audio::SDLAudioEngine* audioEngine_; // Non-owning pointer
            std::vector<MusicalEvent> currentSongEvents_;
            std::string currentInstrumentName_;

            std::thread playbackThread_;
            std::atomic<bool> stopPlaybackSignal_;
            std::atomic<bool> isCurrentlyPlaying_;
        };

    } // namespace Audio
} // namespace MusicApp
