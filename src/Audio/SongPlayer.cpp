#include "Audio/SongPlayer.h"
#include "Utils/Log.h"

namespace MusicApp {
namespace Audio {
SongPlayer::SongPlayer(MusicApp::Audio::SDLAudioEngine *audioEngine)
    : audioEngine_(audioEngine), stopPlaybackSignal_(false),
      isCurrentlyPlaying_(false), isPaused_(false) {
  MUSICALAU_LOG("SongPlayer CONSTRUCTOR: Entered.");
  MUSICALAU_LOG(
      "SongPlayer CONSTRUCTOR: audioEngine parameter (received) address: "
      << audioEngine);
  MUSICALAU_LOG(
      "SongPlayer CONSTRUCTOR: this->audioEngine_ (stored member) address: "
      << this->audioEngine_);

  if (!this->audioEngine_) {
    std::cerr << "SongPlayer CONSTRUCTOR ERROR: Stored audioEngine_ is null "
                 "after assignment!"
              << std::endl;
  } else {
    MUSICALAU_LOG(
        "SongPlayer CONSTRUCTOR: Stored audioEngine_ seems valid (not null).");
  }
}

SongPlayer::~SongPlayer() {
  MUSICALAU_LOG("SongPlayer DESTRUCTOR: Entered.");
  stopSong();
  MUSICALAU_LOG("SongPlayer DESTRUCTOR: Exiting.");
}

bool SongPlayer::playSong(const std::vector<MusicalEvent> &events,
                          const std::string &instrumentName) {
  MUSICALAU_LOG(
      "SongPlayer::playSong: Attempting to play song. Current isPlaying: "
      << isCurrentlyPlaying_.load()
      << ", audioEngine_ address: " << audioEngine_);
  if (isCurrentlyPlaying_.load()) { // Explicit load
    MUSICALAU_LOG("SongPlayer: A song is already playing. Stop it first or "
                  "wait for it to finish.");
    return false;
  }
  if (!audioEngine_) {
    std::cerr << "SongPlayer FATAL ERROR: AudioEngine is not initialized at "
                 "playSong entry (audioEngine_ is null). Cannot play song."
              << std::endl;
    return false;
  }
  MUSICALAU_LOG("SongPlayer::playSong: AudioEngine seems valid (not null).");
  if (events.empty()) {
    MUSICALAU_LOG("SongPlayer: No events to play.");
    return false;
  }
  MUSICALAU_LOG("SongPlayer::playSong: Events list is not empty, size: "
                << events.size());

  currentSongEvents_ = events; // Copy events
  currentInstrumentName_ = instrumentName;
  stopPlaybackSignal_ = false;

  if (playbackThread_.joinable()) {
    MUSICALAU_LOG("SongPlayer::playSong: Previous playback thread is joinable. "
                  "Joining now...");
    playbackThread_.join();
    MUSICALAU_LOG("SongPlayer::playSong: Previous playback thread joined.");
  }

  try {
    MUSICALAU_LOG("SongPlayer::playSong: Setting isCurrentlyPlaying_ to true.");
    isCurrentlyPlaying_ = true;
    MUSICALAU_LOG("SongPlayer::playSong: About to create new playback thread "
                  "for instrument: "
                  << currentInstrumentName_);
    playbackThread_ = std::thread(&SongPlayer::playbackLoop, this);
    MUSICALAU_LOG("SongPlayer::playSong: Playback thread object created.");
    // Note: Thread starts executing immediately. Success here
    // doesn't mean the thread function itself is error-free.
  } catch (const std::system_error &e) {
    std::cerr << "SongPlayer FATAL ERROR: Failed to create playback thread: "
              << e.what() << " (Code: " << e.code() << ")" << std::endl;
    isCurrentlyPlaying_ = false; // Reset if thread creation failed
    return false;
  }
  return true;
}

void SongPlayer::stopSong() {
  if (isCurrentlyPlaying_ && playbackThread_.joinable()) {
    MUSICALAU_LOG("SongPlayer: Signaling playback thread to stop...");
    stopPlaybackSignal_ = true;
    playbackThread_.join();
    MUSICALAU_LOG("SongPlayer: Playback thread stopped.");
  }
  isCurrentlyPlaying_ = false;
  stopPlaybackSignal_ = false;
  isPaused_ = false;
}

bool SongPlayer::isPlaying() const { return isCurrentlyPlaying_; }

bool SongPlayer::isPaused() const { return isPaused_.load(); }

void SongPlayer::togglePause() {
  if (!isCurrentlyPlaying_.load()) {
    MUSICALAU_LOG("SongPlayer: Cannot toggle pause, no song is playing.");
    return;
  }
  isPaused_ = !isPaused_.load();
  if (isPaused_.load()) {
    MUSICALAU_LOG("SongPlayer: Playback PAUSED.");
  } else {
    MUSICALAU_LOG("SongPlayer: Playback RESUMED.");
  }
}

void SongPlayer::playbackLoop() {
  MUSICALAU_LOG("SongPlayer: Playback loop entered for instrument: "
                << currentInstrumentName_ << " with "
                << currentSongEvents_.size() << " events.");

  if (!audioEngine_) {
    std::cerr << "SongPlayer FATAL ERROR: audioEngine_ is null at the start of "
                 "playbackLoop! Aborting loop."
              << std::endl;
    isCurrentlyPlaying_ = false;
    return;
  }

  for (size_t i = 0; i < currentSongEvents_.size(); ++i) {
    const auto &event = currentSongEvents_[i];
    MUSICALAU_LOG("SongPlayer: Processing event "
                  << i << ": Note '" << event.pitchName << "', Duration "
                  << event.durationSeconds);

    while (isPaused_.load()) {
      if (stopPlaybackSignal_)
        break;
      SDL_Delay(50);
    }

    if (stopPlaybackSignal_) {
      MUSICALAU_LOG("SongPlayer: Stop signal received in playback loop (before "
                    "processing event "
                    << i << ").");
      break;
    }

    int delayMilliseconds = static_cast<int>(event.durationSeconds * 1000.0f);

    if (event.pitchName != "0" && !event.pitchName.empty()) {
      MUSICALAU_LOG("SongPlayer: Playing note '" << event.pitchName << "'");
      MusicApp::Core::Note noteToPlay(event.pitchName);

      audioEngine_->playSound(currentInstrumentName_, noteToPlay,
                              1.0f); // Default velocity
      MUSICALAU_LOG("SongPlayer: Note '" << event.pitchName
                                         << "' sound played command sent.");

      if (delayMilliseconds > 0) {
        MUSICALAU_LOG("SongPlayer: Delaying for " << delayMilliseconds
                                                  << " ms for note '"
                                                  << event.pitchName << "'");
        SDL_Delay(delayMilliseconds);
        MUSICALAU_LOG("SongPlayer: Delay finished for note '" << event.pitchName
                                                              << "'");
      }

      if (stopPlaybackSignal_) {
        MUSICALAU_LOG("SongPlayer: Stop signal received after delay for note '"
                      << event.pitchName << "'. Stopping sound.");
        audioEngine_->stopSound(currentInstrumentName_, noteToPlay);
        break;
      }
      MUSICALAU_LOG("SongPlayer: Stopping note '" << event.pitchName << "'");
      audioEngine_->stopSound(currentInstrumentName_, noteToPlay);
      MUSICALAU_LOG("SongPlayer: Note '" << event.pitchName
                                         << "' sound stopped command sent.");

    } else { // Silence
      MUSICALAU_LOG("SongPlayer: Processing silence for " << delayMilliseconds
                                                          << " ms.");
      if (delayMilliseconds > 0) {
        SDL_Delay(delayMilliseconds);
      }
      MUSICALAU_LOG("SongPlayer: Silence finished.");
    }

    if (stopPlaybackSignal_) {
      MUSICALAU_LOG("SongPlayer: Stop signal received after event "
                    << i << " processing.");
      break;
    }
    MUSICALAU_LOG("SongPlayer: Inter-event delay (10ms).");
    SDL_Delay(10);
    MUSICALAU_LOG("SongPlayer: Inter-event delay finished.");
  }

  if (stopPlaybackSignal_) {
    MUSICALAU_LOG("SongPlayer: Playback loop terminated by stop signal.");
  } else {
    MUSICALAU_LOG("SongPlayer: Playback loop finished naturally.");
  }
  isCurrentlyPlaying_ = false;
  stopPlaybackSignal_ = false;
  isPaused_ = false;
}

} // namespace Audio
} // namespace MusicApp
