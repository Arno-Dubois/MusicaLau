#pragma once

#include "SDL3/SDL.h"
#include <string>

class VideoGame {
private:
    float x, y, width, height;
    int keys;

public:
    VideoGame(float x = 0, float y = 0, float width = 0, float height = 0, int keys = 16);

    float getX() const { return x; }

    float getY() const { return y; }

    float getWidth() const { return width; }

    float getHeight() const { return height; }

    int getKeys() const { return keys; }

    void setPosition(float newX, float newY);

    void setDimensions(float newWidth, float newHeight);

    void addKeys();

    void removeKeys();
};