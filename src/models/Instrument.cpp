

#include "../../include/models/Instrument.h"

Instrument::Instrument(const std::string& name, int id)
        : name(name), id(id) {
}

void Instrument::setName(const std::string& newName) {
    name = newName;
}

void Instrument::setId(int newId) {
    id = newId;
}