#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define title "2"
#define width 720
#define height 480
#define size 150
#define cycle 50

int AUX_WaitEventTimeoutCount(SDL_Event* event, Uint32* wait) {
    Uint32 before = SDL_GetTicks();
    int isEvent = SDL_WaitEventTimeout(event, *wait);
    if (isEvent) {
        *wait -= (SDL_GetTicks() - before);

        if (*wait >= cycle)
            *wait = cycle;
    } else
        *wait = cycle;

    return isEvent;
}

int main(int argc, char* args[]) {
    // inicialização
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, width,
                                          height, SDL_WINDOW_SHOWN);
    SDL_Renderer* render = SDL_CreateRenderer(window, -1, 0);

    // execução
    int wait, invertX, invertY;
    wait = cycle;
    invertX = invertY = 1;

    SDL_Color c = {0, 0, 255, 0};
    SDL_Rect r = {0, 0, size, size};

    SDL_Event event;
    while (1) {
        // redesenho
        SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
        SDL_RenderClear(render);

        SDL_SetRenderDrawColor(render, c.r, c.g, c.b, c.a);
        SDL_RenderFillRect(render, &r);

        SDL_RenderPresent(render);

        if (AUX_WaitEventTimeoutCount(&event, &wait)) {
            switch (event.type) {
                case SDL_MOUSEMOTION:  // add SDL_MOUSEBUTTONDOWN here!
                    SDL_GetMouseState(&r.x, &r.y);
                    break;

                case SDL_QUIT:
                    SDL_Quit();
                    return (0);
            }
        }
    }
}