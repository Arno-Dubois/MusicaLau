#include "include/views/menu.cpp"

int main(int argc, char *args[]) {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    if (!initSDL(&window, &renderer)) {
        return 1;
    }

    Menu menu(renderer);
    menu.run();

    closeSDL(window, renderer);
    return 0;
}
