#pragma once
#include <SDL3/SDL.h>
#include <string>

class Piano {
private:
    float x, y, width, height;
    int octaves;

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
};
