#include <SDL2/SDL.h>

#define title "1.5.3"
#define width 200
#define height 100

int main(int argc, char* args[]) {
    // inicialização
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, width,
                                          height, SDL_WINDOW_SHOWN);
    SDL_Renderer* render = SDL_CreateRenderer(window, -1, 0);

    // execução
    int isEvent, invertX, wait;
    invertX = 1;
    wait = 150;

    Uint32 before;

    SDL_Color c1 = {0, 0, 255, 0};
    SDL_Color c2 = {255, 0, 0, 0};

    SDL_Rect r1 = {20, 35, 10, 30};
    SDL_Rect r2 = {170, 35, 10, 30};
    SDL_Rect r3 = {100, 35, 10, 10};

    SDL_Event event;
    while (1) {
        SDL_SetRenderDrawColor(render, 0xFF, 0xFF, 0xFF, 0x00);
        SDL_RenderClear(render);

        SDL_SetRenderDrawColor(render, c1.r, c1.g, c1.b, 0);
        SDL_RenderFillRect(render, &r1);
        SDL_RenderFillRect(render, &r2);

        SDL_SetRenderDrawColor(render, c2.r, c2.g, c2.b, 0);
        SDL_RenderFillRect(render, &r3);

        SDL_RenderPresent(render);

        before = SDL_GetTicks();
        isEvent = SDL_WaitEventTimeout(&event, wait);

        if (isEvent) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_UP:
                            if (r1.y <= 0)
                                r1.y = 0;
                            else
                                r1.y = r1.y - 4;
                            break;
                        case SDLK_DOWN:
                            if (r1.y >= 70)
                                r1.y = 70;
                            else
                                r1.y = r1.y + 4;
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
            if (r3.x >= 160)
                invertX = -1;
            else if (r1.y >= 30 && r1.y <= 60 && r3.x <= 30)
                invertX = 1;

            r3.x = r3.x + 10 * invertX;

            wait = 150;
        }
    }
}