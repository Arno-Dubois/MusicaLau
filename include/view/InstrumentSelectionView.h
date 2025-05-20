#pragma once

#include "View.h"
#include <SDL3/SDL.h>

class InstrumentSelectionView : public View {
public:
    InstrumentSelectionView();

    ~InstrumentSelectionView() override = default;

    void render(SDL_Renderer *renderer, int windowWidth = 0, int windowHeight = 0) override;
};