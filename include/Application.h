#pragma once

#include <SDL3/SDL.h>
#include "controllers/Controller.h"
#include "controllers/PianoAppController.h"
#include "controllers/XylophoneAppController.h"
#include "controllers/VideoGameAppController.h"

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
    int windowWidth;
    int windowHeight;
    bool initialized;
    InstrumentType currentInstrument;

public:
    Application(int width = 1440, int height = 1024);

    ~Application();

    bool initialize();

    bool run();

    void cleanup();

    void setInstrument(InstrumentType instrument);
};