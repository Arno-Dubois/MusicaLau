#pragma once

#include <SDL3/SDL.h>
#include "../model/Instrument.h"
#include <string>

class InstrumentController {
protected:
    Instrument *instrument;

public:
    InstrumentController(Instrument *instrument);

    virtual ~InstrumentController();

    std::string getInstrumentName() const;

    void setInstrumentName(const std::string &name);

    virtual void render(SDL_Renderer *renderer) = 0;
};