#include <SDL2/SDL.h>
#include<SDL2/SDL2_gfxPrimitives.h>

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("1.2.2",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         200, 100, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
	SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00); 
    SDL_RenderClear(ren);

    filledCircleColor (ren, 100, 50, 50, 0x0075F0);
    filledEllipseColor (ren, 50, 50, 30, 15, 0x0115FA);
    filledEllipseColor (ren, 150, 50, 30, 15, 0x4B00E0);
    thickLineColor (ren, 0, 0, 200, 100, 5, 0xBD0CF7);

	SDL_RenderPresent(ren);
    SDL_Delay(5000);

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}