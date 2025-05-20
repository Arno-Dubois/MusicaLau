#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <vector>
#include <string>
#include <cmath>
#include <thread>
#include <atomic>
#include "Button.h"
#include "../View/View.h"
#include "../Audio/MusicFileReader.h"

class ButtonView;

// Forward declaration for the callback
static void FileDialogCallback(void *userdata, const char *const *filePaths, int numFiles);

namespace MusicApp {
    namespace Audio {
        class AudioEngine;
    }
}

class Controller {
protected:
    std::vector<Button> buttons;
    TTF_Font *font;

    // UI parameters
    float buttonWidth = 121.0f;
    float buttonHeight = 90.0f;
    float buttonSpacing = 50.0f;
    float toolbarY = 20.0f;
    float startX = 63.0f;

    // Suivi des dimensions actuelles de la fenêtre
    int currentWindowWidth;
    int currentWindowHeight;

    // Audio engine
    MusicApp::Audio::AudioEngine *audioEngine;

    // Imported song data
    std::string importedFilePath;
    std::string importedFileName;
    std::vector<MusicalEvent> currentSongEvents;
    bool songLoaded;

    // Button View
    ButtonView *buttonView_; // Pointer to ButtonView

    // Song playback threading
    std::thread songPlaybackThread_;
    std::atomic<bool> stopSongPlayback_;
    std::atomic<bool> isSongPlaying_;

private:
    // Method to be run in the song playback thread
    void songPlaybackLoop(std::string instrumentName, std::vector<MusicalEvent> eventsToPlay);

public:
    Controller();

    Controller(MusicApp::Audio::AudioEngine *audioE);

    virtual ~Controller();

    friend void FileDialogCallback(void *userdata, const char *const *filePaths, int numFiles);

    void initializeButtons();

    int handleButtonClick(float x, float y);

    virtual void render(SDL_Renderer *renderer, int windowWidth, int windowHeight) = 0;

    // Nouvelles méthodes pour calculer les dimensions relatives
    float calculateRelativeWidth(int windowWidth, float percentage);

    float calculateRelativeHeight(int windowHeight, float percentage);

    void updateDimensions(int windowWidth, int windowHeight);

    // Methods for song import and playback
    void handleImportSong();

    void handlePlaySong(const std::string &instrumentName);

    void stopCurrentlyPlayingSong();

    std::string getImportedFileName() const;
};