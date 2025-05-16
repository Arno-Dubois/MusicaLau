#pragma once

#include <SDL3/SDL.h>
#include "Controller.h"

// Déclarations anticipées
class Xylophone;

class XylophoneView;

class XylophoneAppController : public Controller {
private:
    Xylophone *xylophone;
    XylophoneView *xylophoneView;

public:
    XylophoneAppController(int windowWidth, int windowHeight);

    ~XylophoneAppController();

    void processButtonAction(int buttonIndex);

    void render(SDL_Renderer *renderer, int windowWidth, int windowHeight) override;
};