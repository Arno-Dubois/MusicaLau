//
// Created by coren on 24/03/2025.
//

#include "../../include/Model/VideoGame.h"

VideoGame::VideoGame(float x, float y, float width, float height, int keys)
        : x(x), y(y), width(width), height(height), keys(keys) {
}

void VideoGame::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
}

void VideoGame::setDimensions(float newWidth, float newHeight) {
    width = newWidth;
    height = newHeight;
}

void VideoGame::addKeys() {
    if (keys < 24) keys++;
}

void VideoGame::removeKeys() {
    if (keys > 8) keys--;
}