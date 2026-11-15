#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <vector>

namespace Model {
    class Instrument {
    protected:
        std::string name;
        int id;

    public:
        Instrument(const std::string &name = "", int id = 0) : name(name), id(id) {}

        virtual ~Instrument() = default;

        std::string getName() const { return name; }

        int getId() const { return id; }

        void setName(const std::string &newName) { name = newName; }

        void setId(int newId) { id = newId; }
    };
} // namespace Model

struct PianoKey {
    SDL_FRect rect;         // Bounding box of the key
    std::string pitchName;  // e.g., "C4", "Db4"
    bool isBlack;           // To help with drawing and layout
    bool isHovered;         // To indicate if the key is hovered
};

class Piano {
private:
    float x, y, width, height;
    int octaves;
    std::vector<PianoKey> pianoKeys; // Stores all keys
    int hoveredKeyIndex;             // Index of the hovered key (-1 if none)

    void calculateKeyLayout(); // Private method to calculate key positions and names

public:
    Piano(float x = 0, float y = 0, float width = 0, float height = 0, int octaves = 3);

    float getX() const { return x; }

    float getY() const { return y; }

    float getWidth() const { return width; }

    float getHeight() const { return height; }

    int getOctaves() const { return octaves; }

    void setPosition(float newX, float newY);

    void setDimensions(float newWidth, float newHeight);

    void addOctave();

    void removeOctave();

    // Method to get pitch name from coordinates
    std::string getPitchAt(float mouseX, float mouseY) const;

    // Method to handle key hover state
    bool updateHoveredKey(float mouseX, float mouseY);

    // Method to clear hover state
    void clearHoveredKey();

    // Getter for the view to access key data for rendering
    const std::vector<PianoKey> &getPianoKeys() const { return pianoKeys; }
};