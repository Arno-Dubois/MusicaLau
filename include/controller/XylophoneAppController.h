#pragma once

#include <SDL3/SDL.h>
#include "Controller.h"
#include "../Model/Xylophone.h"
#include "../View/XylophoneView.h"

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