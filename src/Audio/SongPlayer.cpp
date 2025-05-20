#include "../../include/Audio/SongPlayer.h"

namespace MusicApp {
    namespace Audio {

        SongPlayer::SongPlayer(MusicApp::Audio::SDLAudioEngine* audioEngine) // Parameter is SDLAudioEngine*
            : audioEngine_(audioEngine), // This should store the passed audioEngine
              stopPlaybackSignal_(false),
              isCurrentlyPlaying_(false) {
            std::cout << "SongPlayer CONSTRUCTOR: Entered." << std::endl;
            std::cout << "SongPlayer CONSTRUCTOR: audioEngine parameter (received) address: " << audioEngine << std::endl; // Log received address
            std::cout << "SongPlayer CONSTRUCTOR: this->audioEngine_ (stored member) address: " << this->audioEngine_ << std::endl; // Log stored address

            if (!this->audioEngine_) { // Check the member
                std::cerr << "SongPlayer CONSTRUCTOR ERROR: Stored audioEngine_ is null after assignment!" << std::endl;
            } else {
                std::cout << "SongPlayer CONSTRUCTOR: Stored audioEngine_ seems valid (not null)." << std::endl;
            }
        }

        SongPlayer::~SongPlayer() {
            std::cout << "SongPlayer DESTRUCTOR: Entered." << std::endl;
            stopSong(); 
            std::cout << "SongPlayer DESTRUCTOR: Exiting." << std::endl;
        }

        bool SongPlayer::playSong(const std::vector<MusicalEvent>& events, const std::string& instrumentName) {
            std::cout << "SongPlayer::playSong: Attempting to play song. Current isPlaying: " << isCurrentlyPlaying_.load() 
                      << ", audioEngine_ address: " << audioEngine_ << std::endl;
            if (isCurrentlyPlaying_.load()) { // Explicit load
                std::cout << "SongPlayer: A song is already playing. Stop it first or wait for it to finish." << std::endl;
                return false;
            }
            if (!audioEngine_) {
                std::cerr << "SongPlayer FATAL ERROR: AudioEngine is not initialized at playSong entry (audioEngine_ is null). Cannot play song." << std::endl;
                return false;
            }
            std::cout << "SongPlayer::playSong: AudioEngine seems valid (not null)." << std::endl;
            if (events.empty()) {
                std::cout << "SongPlayer: No events to play." << std::endl;
                return false;
            }
            std::cout << "SongPlayer::playSong: Events list is not empty, size: " << events.size() << std::endl;

            currentSongEvents_ = events; // Copy events
            currentInstrumentName_ = instrumentName;
            stopPlaybackSignal_ = false;

            if (playbackThread_.joinable()) {
                std::cout << "SongPlayer::playSong: Previous playback thread is joinable. Joining now..." << std::endl;
                playbackThread_.join(); 
                std::cout << "SongPlayer::playSong: Previous playback thread joined." << std::endl;
            }

            try {
                std::cout << "SongPlayer::playSong: Setting isCurrentlyPlaying_ to true." << std::endl;
                isCurrentlyPlaying_ = true; 
                std::cout << "SongPlayer::playSong: About to create new playback thread for instrument: " << currentInstrumentName_ << std::endl;
                playbackThread_ = std::thread(&SongPlayer::playbackLoop, this);
                std::cout << "SongPlayer::playSong: Playback thread object created." << std::endl;
                // Note: Thread starts executing immediately. Success here doesn't mean the thread function itself is error-free.
            } catch (const std::system_error& e) {
                std::cerr << "SongPlayer FATAL ERROR: Failed to create playback thread: " << e.what() << " (Code: " << e.code() << ")" << std::endl;
                isCurrentlyPlaying_ = false; // Reset if thread creation failed
                return false;
            }
            return true;
        }

        void SongPlayer::stopSong() {
            if (isCurrentlyPlaying_ && playbackThread_.joinable()) {
                std::cout << "SongPlayer: Signaling playback thread to stop..." << std::endl;
                stopPlaybackSignal_ = true;
                playbackThread_.join(); // Wait for the thread to finish
                std::cout << "SongPlayer: Playback thread stopped." << std::endl;
            }
            isCurrentlyPlaying_ = false; // Ensure this is reset even if thread wasn't joinable for some reason
            stopPlaybackSignal_ = false; // Reset signal for next play
        }

        bool SongPlayer::isPlaying() const {
            return isCurrentlyPlaying_;
        }

        void SongPlayer::playbackLoop() {
            std::cout << "SongPlayer: Playback loop entered for instrument: " << currentInstrumentName_ << " with " << currentSongEvents_.size() << " events." << std::endl;

            if (!audioEngine_) {
                std::cerr << "SongPlayer FATAL ERROR: audioEngine_ is null at the start of playbackLoop! Aborting loop." << std::endl;
                isCurrentlyPlaying_ = false;
                return;
            }

            for (size_t i = 0; i < currentSongEvents_.size(); ++i) {
                const auto& event = currentSongEvents_[i];
                std::cout << "SongPlayer: Processing event " << i << ": Note '" << event.pitchName << "', Duration " << event.durationSeconds << std::endl;

                if (stopPlaybackSignal_) {
                    std::cout << "SongPlayer: Stop signal received in playback loop (before processing event " << i << ")." << std::endl;
                    break;
                }

                int delayMilliseconds = static_cast<int>(event.durationSeconds * 1000.0f);

                if (event.pitchName != "0" && !event.pitchName.empty()) {
                    std::cout << "SongPlayer: Playing note '" << event.pitchName << "'" << std::endl;
                    MusicApp::Core::Note noteToPlay(event.pitchName);
                    
                    audioEngine_->playSound(currentInstrumentName_, noteToPlay, 1.0f); // Default velocity
                    std::cout << "SongPlayer: Note '" << event.pitchName << "' sound played command sent." << std::endl;

                    if (delayMilliseconds > 0) {
                        std::cout << "SongPlayer: Delaying for " << delayMilliseconds << " ms for note '" << event.pitchName << "'" << std::endl;
                        SDL_Delay(delayMilliseconds);
                        std::cout << "SongPlayer: Delay finished for note '" << event.pitchName << "'" << std::endl;
                    }

                    if (stopPlaybackSignal_) { 
                        std::cout << "SongPlayer: Stop signal received after delay for note '" << event.pitchName << "'. Stopping sound." << std::endl;
                        audioEngine_->stopSound(currentInstrumentName_, noteToPlay);
                        break;
                    }
                    std::cout << "SongPlayer: Stopping note '" << event.pitchName << "'" << std::endl;
                    audioEngine_->stopSound(currentInstrumentName_, noteToPlay);
                    std::cout << "SongPlayer: Note '" << event.pitchName << "' sound stopped command sent." << std::endl;

                } else { // Silence
                    std::cout << "SongPlayer: Processing silence for " << delayMilliseconds << " ms." << std::endl;
                    if (delayMilliseconds > 0) {
                        SDL_Delay(delayMilliseconds);
                    }
                    std::cout << "SongPlayer: Silence finished." << std::endl;
                }

                if (stopPlaybackSignal_) {
                     std::cout << "SongPlayer: Stop signal received after event " << i << " processing." << std::endl;
                    break; 
                }
                std::cout << "SongPlayer: Inter-event delay (10ms)." << std::endl;
                SDL_Delay(10); 
                std::cout << "SongPlayer: Inter-event delay finished." << std::endl;
            }

            if (stopPlaybackSignal_) {
                std::cout << "SongPlayer: Playback loop terminated by stop signal." << std::endl;
            }
            else {
                std::cout << "SongPlayer: Playback loop finished naturally." << std::endl;
            }
            isCurrentlyPlaying_ = false;
            // stopPlaybackSignal_ is reset by stopSong() or at the start of playSong()
        }

    } // namespace Audio
} // namespace MusicApp
