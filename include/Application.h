#pragma once

#include <SDL3/SDL.h>
#include "controller/Controller.h"
#include "controller/PianoAppController.h"
#include "controller/XylophoneAppController.h"
#include "controller/VideoGameAppController.h"
#include "utils/DropdownMenu.h"

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