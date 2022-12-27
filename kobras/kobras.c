#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#define title "Kobras"

#define windowWidth 640
#define windowHeight 480

#define gridSize 20
#define gridWidth (windowWidth / gridSize)
#define gridHeight (windowHeight / gridSize)

#define cycle 50

typedef struct {
    int x;
    int y;
} SnakeSegment;

typedef struct {
    SnakeSegment body[1000];  // tamanho máximo da cobra
    int size;
    int direction;
} Snake;

typedef struct {
    int x;
    int y;
} Food;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

Snake snake;
Food food;

bool gameOver = false;

// inicializações
void init() {
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, windowWidth,
                              windowHeight, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, 0);
}

int timeout(SDL_Event* event, int* wait) {
    int before = SDL_GetTicks();
    int isEvent = SDL_WaitEventTimeout(event, *wait);

    if (isEvent) {
        *wait -= (SDL_GetTicks() - before);

        if (*wait < 0)
            *wait = 0;

    } else
        *wait = cycle;

    return isEvent;
}

void inputs(SDL_Event* event) {
    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);

    switch (event->type) {
        case SDL_KEYDOWN:
            switch (event->key.keysym.sym) {
                case SDLK_UP:
                    if (snake.direction != 2)
                        snake.direction = 0;
                    break;
                case SDLK_RIGHT:
                    if (snake.direction != 3)
                        snake.direction = 1;
                    break;
                case SDLK_DOWN:
                    if (snake.direction != 0)
                        snake.direction = 2;
                    break;
                case SDLK_LEFT:
                    if (snake.direction != 1)
                        snake.direction = 3;
                    break;
            }
            break;

        case SDL_QUIT:
            gameOver = true;
            break;
    }
}

void update() {
    // atualiza a posição da cobra
    for (int i = snake.size - 1; i > 0; i--)
        snake.body[i] = snake.body[i - 1];

    if (snake.direction == 0)
        snake.body[0].y--;
    else if (snake.direction == 1)
        snake.body[0].x++;
    else if (snake.direction == 2)
        snake.body[0].y++;
    else if (snake.direction == 3)
        snake.body[0].x--;

    // colisoes
    if (snake.body[0].x < 0 || snake.body[0].x >= gridWidth ||
        snake.body[0].y < 0 || snake.body[0].y >= gridHeight)
        gameOver = true;

    for (int i = 1; i < snake.size; i++)
        if (snake.body[0].x == snake.body[i].x &&
            snake.body[0].y == snake.body[i].y)
            gameOver = true;

    // verifica se a cobra comeu a comida
    if (snake.body[0].x == food.x && snake.body[0].y == food.y) {
        // aumenta o tamanho da cobra
        snake.size++;

        // gera uma nova comida em uma posição aleatória
        food.x = rand() % gridWidth;
        food.y = rand() % gridHeight;
    }
}

void render() {
    // limpa a tela
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // desenha a cobra
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < snake.size; i++) {
        SDL_Rect rect = {snake.body[i].x * gridSize, snake.body[i].y * gridSize,
                         gridSize, gridSize};
        SDL_RenderFillRect(renderer, &rect);
    }

    // desenha a comida
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect food_rect = {food.x * gridSize, food.y * gridSize, gridSize,
                          gridSize};
    SDL_RenderFillRect(renderer, &food_rect);

    // atualiza a tela
    SDL_RenderPresent(renderer);
}

void quit() {
    // libera os recursos alocados pelo SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    init();

    // inicializa a cobra
    snake.body[0].x = gridWidth / 2;
    snake.body[0].y = gridHeight / 2;
    snake.size = 1;
    snake.direction = 1;

    // gera a comida em uma posição aleatória
    food.x = rand() % gridWidth;
    food.y = rand() % gridHeight;

    SDL_Event event;
    int wait = cycle;

    // loop principal
    while (!gameOver) {
        if (timeout(&event, &wait)) {
            inputs(&event);
        } else
            update();

        render();
    }

    quit();
    return 0;
}