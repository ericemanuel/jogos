#include <SDL2/SDL.h>

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Hello World!",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         200, 100, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    int i, troca;
    troca = 0;

    /* EXECUÇÃO */
    for(i=0; i<=140; i=i+2){
	SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00); 
        SDL_RenderClear(ren);

	SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);    	
	SDL_Rect r1 = { 20,20, 10,60 };
	SDL_RenderFillRect(ren, &r1);

	SDL_Rect r2 = { 170,20, 10,60 };
	SDL_RenderFillRect(ren, &r2);

	SDL_SetRenderDrawColor(ren, 0xFF,0x00,0x00,0x00);    
	SDL_Rect r3 = { 30+i,45, 10,10 };
	SDL_RenderFillRect(ren, &r3);

	if (i == 130)
		troca = 1;	
	else if(i == 0)
		troca = 0;	

	if (troca == 1)
		i=i-4;

	//SDL_RenderDrawPoint(ren, 40+i, 20);
	SDL_RenderPresent(ren);
    SDL_Delay(10);
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}