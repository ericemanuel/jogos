#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define title "1.5.2"
#define width 720
#define height 480
#define size 150
#define cycle 1000

int wait, before, newCycle = 1;

int AUX_WaitEventTimeoutCount(SDL_Event event, int ms) {
    if (newCycle) {
        wait = cycle;
        before = ms;
        newCycle = 0;
        SDL_Log("New cycle!");
    } else {
        wait -= SDL_GetTicks() - before;
        if (wait <= 0) {
            wait = 0;
            newCycle = 1;
        }
        SDL_Log("wait = %d - %d", SDL_GetTicks(), before);
    }

    return SDL_WaitEventTimeout(&event, wait);
}

int main(int argc, char* args[]) {
    // inicialização
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, width,
                                          height, SDL_WINDOW_SHOWN);
    SDL_Renderer* render = SDL_CreateRenderer(window, -1, 0);

    // execução
    int mx, my, isEvent, invertX, invertY;
    isEvent = 0;
    invertX = invertY = 1;

    SDL_Color c1 = {255, 0, 0, 0};
    SDL_Color c2 = {0, 255, 0, 0};
    SDL_Color c3 = {0, 0, 255, 0};

    SDL_Rect r1 = {width / 4 - size / 2, height / 2 - size / 2, size, size};
    SDL_Rect r2 = {width / 2 - size / 2, height / 2 - size / 2, size, size};

    SDL_Event event;
    while (1) {
        // redesenho
        SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
        SDL_RenderClear(render);

        SDL_SetRenderDrawColor(render, c1.r, c1.g, c1.b, c1.a);
        SDL_RenderFillRect(render, &r1);

        SDL_SetRenderDrawColor(render, c2.r, c2.g, c2.b, c2.a);
        SDL_RenderFillRect(render, &r2);

        SDL_GetMouseState(&mx, &my);
        SDL_Rect r3 = {mx, my, size, size};
        SDL_SetRenderDrawColor(render, c3.r, c3.g, c3.b, c3.a);
        SDL_RenderFillRect(render, &r3);

        SDL_RenderPresent(render);

        isEvent = AUX_WaitEventTimeoutCount(event, SDL_GetTicks());
        if (isEvent) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_UP:
                            if (r2.y > 0) {
                                r2.y -= size / 5;
                            }
                            break;
                        case SDLK_DOWN:
                            if (r2.y < (height - size))
                                r2.y += size / 5;
                            break;
                        case SDLK_LEFT:
                            if (r2.x > 0)
                                r2.x -= size / 5;
                            break;
                        case SDLK_RIGHT:
                            if (r2.x < (width - size))
                                r2.x += size / 5;
                            break;
                    }
                    break;

                case SDL_QUIT:
                    SDL_Quit();
                    return (0);
            }

            AUX_WaitEventTimeoutCount(event, SDL_GetTicks());

        } else {
            if (r1.x >= (width - size))
                invertX = -1;
            else if (r1.x <= 0)
                invertX = 1;

            if (r1.y >= (height - size))
                invertY = -1;
            else if (r1.y <= 0)
                invertY = 1;

            r1.x += size / 20 * invertX;
            r1.y += size / 20 * invertY;
        }
    }
}