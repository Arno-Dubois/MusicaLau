//
// Created by coren on 24/03/2025.
//

#include "../../include/Model/Xylophone.h"

Xylophone::Xylophone(float x, float y, float width, float height, int bars)
        : x(x), y(y), width(width), height(height), bars(bars) {
}

void Xylophone::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
}

void Xylophone::setDimensions(float newWidth, float newHeight) {
    width = newWidth;
    height = newHeight;
}

void Xylophone::addBar() {
    if (bars < 12) bars++;
}

void Xylophone::removeBar() {
    if (bars > 4) bars--;
}