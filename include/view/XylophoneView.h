#pragma once

#include "SDL3/SDL.h"
#include "View.h"
#include "../Model/Xylophone.h"

class XylophoneView : public View {
private:
    Xylophone *xylophone;

public:
    explicit XylophoneView(Xylophone *xylophone);

    void render(SDL_Renderer *renderer, int windowWidth = 0, int windowHeight = 0) override;
};