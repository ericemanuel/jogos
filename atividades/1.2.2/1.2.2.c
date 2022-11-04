#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#define title "1.2.2"
#define width 400
#define height 300

int main(int argc, char *args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *win = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                       width, height, SDL_WINDOW_SHOWN);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF, 0x00);
    SDL_RenderClear(ren);

    thickLineColor(ren, 0, 0, width, height, width / 30, 0xFF000000);
    filledCircleColor(ren, width / 2, height / 2, width / 4, 0xFFFF0000); // aaBBGGRR
    filledEllipseColor(ren, width / 4, height / 2, width / 8, height / 8, 0xFF00FF00);
    filledEllipseColor(ren, 3 * width / 4, height / 2, width / 8, height / 8, 0xFF0000FF);

    SDL_RenderPresent(ren);
    SDL_Delay(5000);

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}