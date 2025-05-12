#pragma once

#include <SDL3/SDL.h>
#include "controllers/Controller.h"
#include "controllers/PianoAppController.h"

class Application {
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    Controller *mainController;
    int windowWidth;
    int windowHeight;
    bool initialized;

public:
    Application(int width = 1440, int height = 1024);

    ~Application();

    bool initialize();

    bool run();

    void cleanup();
};