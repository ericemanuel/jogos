#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define width 200
#define height 100

int main(int argc, char* args[]) {
    // inicialização
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("1.4.1", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, width,
                                          height, SDL_WINDOW_SHOWN);
    SDL_Renderer* render = SDL_CreateRenderer(window, -1, 0);

    // execução
    int mx, my, contador = -1;

    SDL_Rect m[10];
    SDL_Color c[10];
    for (int i = 0; i <= 9; i++) {
        m[i].x = -100;
        m[i].y = -100;
        m[i].w = rand() % 60;
        m[i].h = rand() % 60;

        c[i].r = rand() % 255;
        c[i].g = rand() % 255;
        c[i].b = rand() % 255;
    }

    SDL_Rect r = {40, 20, 10, 10};

    SDL_Event event;
    while (1) {
        SDL_SetRenderDrawColor(render, 255, 255, 255, 0);
        SDL_RenderClear(render);

        for (int i = 0; i <= 9; i++) {
            SDL_SetRenderDrawColor(render, c[i].r, c[i].g, c[i].b, 0);
            SDL_RenderFillRect(render, &m[i]);
        }

        SDL_SetRenderDrawColor(render, 0, 0, 255, 0);
        SDL_RenderFillRect(render, &r);

        SDL_RenderPresent(render);

        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        if (r.y > 0)
                            r.y -= 5;
                        break;
                    case SDLK_DOWN:
                        if (r.y < (height - r.h))
                            r.y += 5;
                        break;
                    case SDLK_LEFT:
                        if (r.x > 0)
                            r.x -= 5;
                        break;
                    case SDLK_RIGHT:
                        if (r.x < (width - r.w))
                            r.x += 5;
                        break;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                SDL_GetMouseState(&mx, &my);

                contador++;
                if (contador > 10)
                    contador = 0;

                m[contador].x = mx;
                m[contador].y = my;

                break;

            case SDL_QUIT:
                // finalização
                SDL_DestroyRenderer(render);
                SDL_DestroyWindow(window);
                SDL_Quit();
        }
    }
}