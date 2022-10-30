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
    int i, j;

    /* EXECUÇÃO */
	SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00); 
        SDL_RenderClear(ren);
    
    SDL_SetRenderDrawColor(ren, 0xFF,0x00,0x0,0x00);
    for(i=-50; i<=250; i=i+20)
        SDL_RenderDrawLine(ren, i,0, i+50,100);

    SDL_SetRenderDrawColor(ren, 0x00,0x00,0xAA,0x00);
    for(i=-50; i<=250; i=i+20)
        SDL_RenderDrawLine(ren, i,0, i-50,100);

    SDL_SetRenderDrawColor(ren, 0x00,0x00,0x00,0x00);
    for(j=0; j<=100; j=j+40)    
        for(i=0; i<=200; i=i+20)
            SDL_RenderDrawPoint(ren, i, j);
    
    for(j=20; j<=100; j=j+40)    
        for(i=10; i<=200; i=i+20)
            SDL_RenderDrawPoint(ren, i, j);
    
    SDL_SetRenderDrawColor(ren, 0x00,0x00,0xAA,0x00);
    SDL_Rect r1 = { 0,0, 200,5 };
    SDL_RenderDrawRect(ren, &r1);
    SDL_RenderFillRect(ren, &r1);
    SDL_Rect r2 = { 195,5, 200,95 };
    SDL_RenderDrawRect(ren, &r2);
    SDL_RenderFillRect(ren, &r2);
    SDL_Rect r3 = { 0,95, 200,100 };
    SDL_RenderDrawRect(ren, &r3);
    SDL_RenderFillRect(ren, &r3);
    SDL_Rect r4 = { 0,5, 5,95 };
    SDL_RenderDrawRect(ren, &r4);
    SDL_RenderFillRect(ren, &r4);


	//SDL_RenderDrawPoint(ren, 40+i, 20);
	SDL_RenderPresent(ren);
    SDL_Delay(5000);

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}