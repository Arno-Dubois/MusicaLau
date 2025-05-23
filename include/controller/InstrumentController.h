#pragma once

#include <SDL3/SDL.h>
#include "../Model/Instrument.h"
#include <string>

class InstrumentController {
protected:
    Model::Instrument *instrument;

public:
    InstrumentController(Model::Instrument *instrument);

    virtual ~InstrumentController();

    std::string getInstrumentName() const;

    void setInstrumentName(const std::string &name);

    virtual void render(SDL_Renderer *renderer) = 0;
};