#pragma once

#include <SDL3/SDL.h>
#include "Controller.h"
#include "../Model/Piano.h"
#include "../View/PianoView.h"
#include "../Core/Note.h"
#include "../Audio/AudioEngine.h"

class PianoAppController : public Controller {
private:
    Piano *piano;
    PianoView *pianoView;

public:
    PianoAppController(int windowWidth, int windowHeight, MusicApp::Audio::AudioEngine *audioE);

    ~PianoAppController();

    void processButtonAction(int buttonIndex);

    void handlePianoKeyClick(float mouseX, float mouseY);

    void handlePianoKeyHover(float mouseX, float mouseY);

    // Accesseur pour le piano
    Piano *getPiano() const { return piano; }

    void render(SDL_Renderer *renderer, int windowWidth, int windowHeight) override;
};