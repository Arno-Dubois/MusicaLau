#pragma once

#include <SDL3/SDL.h>
#include "../Model/Piano.h"

class PianoView {
private:
    Piano *piano;

public:
    PianoView(Piano *piano);

    void render(SDL_Renderer *renderer);
};