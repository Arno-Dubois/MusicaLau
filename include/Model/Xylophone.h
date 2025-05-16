#pragma once

#include <SDL3/SDL.h>
#include <string>

class Xylophone {
private:
    float x, y, width, height;
    int bars;

public:
    Xylophone(float x = 0, float y = 0, float width = 0, float height = 0, int bars = 8);

    float getX() const { return x; }

    float getY() const { return y; }

    float getWidth() const { return width; }

    float getHeight() const { return height; }

    int getBars() const { return bars; }

    void setPosition(float newX, float newY);

    void setDimensions(float newWidth, float newHeight);

    void addBar();

    void removeBar();
};