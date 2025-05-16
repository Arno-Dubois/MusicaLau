#pragma once

#include "SDL3/SDL.h"
#include "../Model/Piano.h"
#include "View.h"

class PianoView : public View {
private:
    Piano *piano;

public:
    explicit PianoView(Piano *piano);

    void render(SDL_Renderer *renderer, int windowWidth = 0, int windowHeight = 0) override;
};