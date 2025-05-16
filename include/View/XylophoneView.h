#pragma once

#include <SDL3/SDL.h>

// Déclaration anticipée
class Xylophone;

class XylophoneView {
private:
    Xylophone *xylophone;

public:
    XylophoneView(Xylophone *xylophone);

    void render(SDL_Renderer *renderer);
};