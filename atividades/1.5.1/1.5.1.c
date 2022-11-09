#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define width 200
#define height 100

int main(int argc, char* args[]) {
    // inicialização
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("1.5.1", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, width,
                                          height, SDL_WINDOW_SHOWN);
    SDL_Renderer* render = SDL_CreateRenderer(window, -1, 0);

    // execução
    int mx, my, isEvent, limitX, limitY, wait;
    isEvent = 0;
    limitX = limitY = 1;
    wait = 200;

    Uint32 before;

    SDL_Color c1 = {255, 0, 0, 0};
    SDL_Color c2 = {0, 255, 0, 0};
    SDL_Color c3 = {0, 0, 255, 0};

    SDL_Rect r1 = {width / 4, height / 2, 10, 10};
    SDL_Rect r2 = {width / 2, height / 2, 10, 10};
    SDL_Rect r3 = {3 * width / 4, height / 2, 10, 10};

    SDL_Event event;
    while (1) {
        SDL_SetRenderDrawColor(render, 255, 255, 255, 0);
        SDL_RenderClear(render);

        SDL_SetRenderDrawColor(render, c1.r, c1.g, c1.b, c1.a);
        SDL_RenderFillRect(render, &r1);

        SDL_SetRenderDrawColor(render, c2.r, c2.g, c2.b, c2.a);
        SDL_RenderFillRect(render, &r2);

        SDL_GetMouseState(&mx, &my);
        SDL_SetRenderDrawColor(render, c3.r, c3.g, c3.b, c3.a);
        SDL_RenderFillRect(render, &r3);

        SDL_RenderPresent(render);

        before = SDL_GetTicks();
        isEvent = SDL_WaitEventTimeout(&event, wait);

        if (isEvent) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_UP:
                            if (r2.y > 0)
                                r2.y -= 5;
                            break;
                        case SDLK_DOWN:
                            if (r2.y < (height - r2.h))
                                r2.y += 5;
                            break;
                        case SDLK_LEFT:
                            if (r2.x > 0)
                                r2.x -= 5;
                            break;
                        case SDLK_RIGHT:
                            if (r2.x < (width - r2.w))
                                r2.x += 5;
                            break;
                    }
                    break;

                case SDL_QUIT:
                    // finalização
                    SDL_DestroyRenderer(render);
                    SDL_DestroyWindow(window);
                    SDL_Quit();
            }

            wait -= (SDL_GetTicks() - before);

        } else {
            if (r1.x >= (width - r1.w))
                limitX = -1;
            else if (r1.x <= 0)
                limitX = 1;

            if (r1.y >= (height - r1.h))
                limitY = -1;
            else if (r1.y <= 0)
                limitY = 1;

            r1.x += 5 * limitX;
            r1.y += 5 * limitY;

            wait = 200;
        }
    }
}