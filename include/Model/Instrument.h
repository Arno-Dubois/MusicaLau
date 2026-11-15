#pragma once

#include <string>

namespace Model {
    class Instrument {
    protected:
        std::string name;
        int id;

    public:
        Instrument(const std::string &name = "", int id = 0);

        virtual ~Instrument() = default;

        std::string getName() const { return name; }

        int getId() const { return id; }

        void setName(const std::string &newName);

        void setId(int newId);
    };
} // namespace Model