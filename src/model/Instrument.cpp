#include "../../include/Model/Instrument.h"

Model::Instrument::Instrument(const std::string &name, int id)
        : name(name), id(id) {
}

void Model::Instrument::setName(const std::string &newName) {
    this->name = newName;
}

void Model::Instrument::setId(int newId) {
    this->id = newId;
}