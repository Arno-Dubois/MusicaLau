//
// Created by coren on 24/03/2025.
//

#include "../../include/Controller/InstrumentController.h"

InstrumentController::InstrumentController(Instrument *instrument)
        : instrument(instrument) {
}

InstrumentController::~InstrumentController() {
    delete instrument;
}

std::string InstrumentController::getInstrumentName() const {
    return instrument->getName();
}

void InstrumentController::setInstrumentName(const std::string &name) {
    instrument->setName(name);
}