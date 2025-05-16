#pragma once

#include <SDL3/SDL.h>
#include "Controller.h"
#include "../Model/Piano.h"
#include "../View/PianoView.h"

class PianoAppController : public Controller {
private:
    Piano *piano;
    PianoView *pianoView;

public:
    PianoAppController(int windowWidth, int windowHeight);

    ~PianoAppController();

    void processButtonAction(int buttonIndex);

    void render(SDL_Renderer *renderer, int windowWidth, int windowHeight) override;
};