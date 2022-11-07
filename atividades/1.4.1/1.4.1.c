#include <stdio.h>
#include <SDL2/SDL.h>
#include <winuser.h>

#define width 200
#define height 100

int main(int argc, char *args[])
{
    // inicialização
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("1.4.1",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          width, height, SDL_WINDOW_SHOWN);
    SDL_Renderer *render = SDL_CreateRenderer(window, -1, 0);

    // execução
    int mx, my;

    SDL_Rect r = {40, 20, 10, 10};

    SDL_Event event;
    while (1)
    {
        switch (mi)
        {
        case 1:
            SDL_Rect m = {mx, my, 10, 10};
        }

        // como fazer para ele gerar retângulos diferentes?

        SDL_SetRenderDrawColor(render, 0xFF, 0xFF, 0xFF, 0x00);
        SDL_RenderClear(render);
        SDL_SetRenderDrawColor(render, 0x00, 0x00, 0xFF, 0x00);
        SDL_RenderFillRect(render, &r);

        SDL_RenderPresent(render);

        SDL_WaitEvent(&event);
        switch (event.type)
        {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
                if (r.y > 0)
                    r.y -= 5;
                break;
            case SDLK_DOWN:
                if (r.y < (height - 10))
                    r.y += 5;
                break;
            case SDLK_LEFT:
                if (r.x > 0)
                    r.x -= 5;
                break;
            case SDLK_RIGHT:
                if (r.x < (width - 10))
                    r.x += 5;
                break;
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
            SDL_GetMouseState(&mx, &my);
            break;

        case SDL_QUIT:
            // finalização
            SDL_DestroyRenderer(render);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }
    }
}