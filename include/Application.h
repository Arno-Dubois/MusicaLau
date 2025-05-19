#pragma once

#include <SDL3/SDL.h>
#include "Controller/Controller.h"
#include "Controller/PianoAppController.h"
#include "Controller/XylophoneAppController.h"
#include "Controller/VideoGameAppController.h"
#include "Utils/DropdownMenu.h"
#include "Audio/SDLAudioEngine.h"

enum class InstrumentType {
    PIANO,
    XYLOPHONE,
    VIDEO_GAME
};

class Application {
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    Controller *mainController;
    MusicApp::Audio::AudioEngine *audioEngine;
    int windowWidth;
    int windowHeight;
    bool initialized;
    InstrumentType currentInstrument;
    DropdownMenu *instrumentMenu;

    void initializeInstrumentMenu();

public:
    Application(int width = 1440, int height = 1024);

    ~Application();

    bool initialize();

    bool run();

    void cleanup();

    void setInstrument(InstrumentType instrument);
};