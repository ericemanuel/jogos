#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#define title "1.7"
#define width 720
#define height 480
#define size 30
#define cycle 10

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
    IMG_Init(0);
    SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, width,
                                          height, SDL_WINDOW_SHOWN);
    SDL_Renderer* render = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture* sprite = IMG_LoadTexture(render, "sprite.png");

    // execução
    int mx, my, invertX;
    invertX = 1;
    Uint32 wait = cycle;

    SDL_Color c1 = {0, 0, 255, 0};
    SDL_Color c2 = {255, 0, 0, 0};

    SDL_Rect r1 = {width / 40, height / 2 - size * 2, size, size * 4};
    SDL_Rect r2 = {39 * width / 40 - size, height / 2 - size * 2, size,
                   size * 4};
    SDL_Rect r3 = {width / 2 - size / 2, height / 2 - size / 2, size, size};

    SDL_Event event;
    while (1) {
        // redesenho
        SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
        SDL_RenderClear(render);

        SDL_SetRenderDrawColor(render, c1.r, c1.g, c1.b, c1.a);
        SDL_RenderFillRect(render, &r1);
        SDL_RenderFillRect(render, &r2);

        // SDL_SetRenderDrawColor(render, c2.r, c2.g, c2.b, c2.a);
        // SDL_RenderFillRect(render, &r3);
        SDL_RenderCopy(render, sprite, NULL, &r3);

        SDL_RenderPresent(render);

        if (AUX_WaitEventTimeoutCount(&event, &wait)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_UP:
                            if (r1.y <= 0)
                                r1.y = 0;
                            else
                                r1.y -= height / 40;
                            break;
                        case SDLK_DOWN:
                            if (r1.y >= height - r1.h)
                                r1.y = height - r1.h;
                            else
                                r1.y += height / 40;
                            break;
                    }
                    break;

                case SDL_QUIT:
                    SDL_Quit();
                    return (0);
            }
        } else {
            r3.x = r3.x + width / 60 * invertX;

            if (r3.x >= r2.x - size) {
                invertX = -1;
                r3.x = r2.x - size;
            } else if (r1.y >= (height / 2 - r1.h) && r1.y <= height / 2 &&
                       r3.x <= width / 40 + r1.w) {
                invertX = 1;
                r3.x = width / 40 + r1.w;
            }

            if (r3.x <= 0 || r3.x >= width)
                r3.x = width / 2 - size / 2;
        }
    }
}