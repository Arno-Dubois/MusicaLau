#pragma once

#include <SDL3/SDL.h>
#include "Controller.h"
#include "../Model/Xylophone.h"
#include "../View/XylophoneView.h"
#include "../Core/Note.h"
#include "../Audio/AudioEngine.h"

class XylophoneAppController : public Controller {
private:
    Xylophone *xylophone;
    XylophoneView *xylophoneView;

public:
    XylophoneAppController(int windowWidth, int windowHeight, MusicApp::Audio::AudioEngine *audioE);

    ~XylophoneAppController();

    void processButtonAction(int buttonIndex);

    void handleXylophoneKeyClick(float mouseX, float mouseY);

    void handleXylophoneKeyHover(float mouseX, float mouseY);

    void render(SDL_Renderer *renderer, int windowWidth, int windowHeight) override;
};