#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define title "1.6"
#define width 720
#define height 480
#define size 150
#define cycle 100

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
    int mx, wait, red, green, blue;
    mx = red = green = blue = 0;
    wait = cycle;
    char winner[10];

    SDL_Color c0 = {255, 255, 255, 0};
    SDL_Color c1 = {255, 0, 0, 0};
    SDL_Color c2 = {0, 255, 0, 0};
    SDL_Color c3 = {0, 0, 255, 0};

    SDL_Rect r0 = {width - size / 5, 0, size / 5, height};
    SDL_Rect r1 = {0, height / 2 - size, size, size};
    SDL_Rect r2 = {0, height / 2 - size / 2, size, size};
    SDL_Rect r3 = {mx, height / 2, size, size};

    SDL_Event event;
    while (1) {
        while (red == 0 || green == 0 || blue == 0) {
            // redesenho
            SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
            SDL_RenderClear(render);

            SDL_SetRenderDrawColor(render, c0.r, c0.g, c0.b, c0.a);
            SDL_RenderFillRect(render, &r0);

            SDL_SetRenderDrawColor(render, c1.r, c1.g, c1.b, c1.a);
            SDL_RenderFillRect(render, &r1);

            SDL_SetRenderDrawColor(render, c2.r, c2.g, c2.b, c2.a);
            SDL_RenderFillRect(render, &r2);

            SDL_SetRenderDrawColor(render, c3.r, c3.g, c3.b, c3.a);
            SDL_RenderFillRect(render, &r3);

            SDL_RenderPresent(render);

            if (AUX_WaitEventTimeoutCount(&event, &wait)) {
                switch (event.type) {
                    case SDL_MOUSEMOTION:
                        if (!SDL_HasIntersection(&r0, &r3))
                            SDL_GetMouseState(&r3.x, 0);
                        else {
                            r3.x = width - r0.w / 2 - r3.w;
                            blue = 1;
                        }

                        break;

                    case SDL_KEYDOWN:
                        switch (event.key.keysym.sym) {
                            case SDLK_LEFT:
                                if (!SDL_HasIntersection(&r0, &r2) &&
                                    (r2.x > 0))
                                    r2.x -= size / 20;
                                break;

                            case SDLK_RIGHT:
                                if (!SDL_HasIntersection(&r0, &r2))
                                    r2.x += size / 20;
                                else {
                                    r2.x = width - r0.w / 2 - r2.w;
                                    green = 1;
                                }

                                break;
                        }
                        break;

                    case SDL_QUIT:
                        SDL_Quit();
                        return (0);
                }
            } else {
                if (!SDL_HasIntersection(&r0, &r1))
                    r1.x += size / 20;
                else {
                    r1.x = width - r0.w / 2 - r1.w;
                    red = 1;
                }
            }

            if (red && !green && !blue)
                strcpy(winner, "Red");
            if (!red && green && !blue)
                strcpy(winner, "Green");
            if (!red && !green && blue)
                strcpy(winner, "Blue");
        }

        mx = r1.x = r2.x = mx = red = green = blue = 0;
        SDL_Log("%s won!", winner);
    }
}