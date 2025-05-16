#include "../include/model/Piano.h"

Piano::Piano(float x, float y, float width, float height, int octaves)
        : x(x), y(y), width(width), height(height), octaves(octaves) {
}

void Piano::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
}

void Piano::setDimensions(float newWidth, float newHeight) {
    width = newWidth;
    height = newHeight;
}

void Piano::addOctave() {
    octaves++;
}

void Piano::removeOctave() {
    if (octaves > 1) {
        octaves--;
    }
}
