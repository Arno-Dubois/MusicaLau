#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "../include/Application.h"

int main(int argc, char *argv[]) {
    Application app;
    if (!app.initialize()) {
        return -1;
    }

    app.run();

    return 0;
}