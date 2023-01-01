#include <SDL2/SDL.h>

#include <stdbool.h>
#include <stdio.h>

#define title "Kobras"

#define windowWidth 640
#define windowHeight 480

#define gridSize 20
#define gridWidth (windowWidth / gridSize)
#define gridHeight (windowHeight / gridSize)

typedef struct {
    int x;
    int y;
} SnakeSegment;

typedef struct {
    SnakeSegment body[1000];  // tamanho máximo da cobra
    int size;
    int direction;
    SDL_Color color;
} Snake;

typedef struct {
    int x;
    int y;
} Food;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

Snake snake[2];
Food food;

int cycle = 100;
bool gameOver = false;
int loser;

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
                // inputs da cobra 0
                case SDLK_w:
                    if (snake[0].direction != 2)
                        snake[0].direction = 0;
                    break;
                case SDLK_d:
                    if (snake[0].direction != 3)
                        snake[0].direction = 1;
                    break;
                case SDLK_s:
                    if (snake[0].direction != 0)
                        snake[0].direction = 2;
                    break;
                case SDLK_a:
                    if (snake[0].direction != 1)
                        snake[0].direction = 3;
                    break;

                // inputs da cobra 1
                case SDLK_UP:
                    if (snake[1].direction != 2)
                        snake[1].direction = 0;
                    break;
                case SDLK_RIGHT:
                    if (snake[1].direction != 3)
                        snake[1].direction = 1;
                    break;
                case SDLK_DOWN:
                    if (snake[1].direction != 0)
                        snake[1].direction = 2;
                    break;
                case SDLK_LEFT:
                    if (snake[1].direction != 1)
                        snake[1].direction = 3;
                    break;
            }
            break;

        case SDL_QUIT:
            gameOver = true;
            break;
    }
}

void updatePosition() {
    for (int s = 0; s < 2; s++) {
        // atualiza a posição das cobras
        for (int i = snake[s].size - 1; i > 0; i--)
            snake[s].body[i] = snake[s].body[i - 1];

        if (snake[s].direction == 0)
            snake[s].body[0].y--;
        else if (snake[s].direction == 1)
            snake[s].body[0].x++;
        else if (snake[s].direction == 2)
            snake[s].body[0].y++;
        else if (snake[s].direction == 3)
            snake[s].body[0].x--;
    }
}

void checkCollisions() {
    for (int s = 0; s < 2; s++) {
        // verifica colisão com as paredes
        if (snake[s].body[0].x < 0 || snake[s].body[0].x >= gridWidth || snake[s].body[0].y < 0 || snake[s].body[0].y >= gridHeight) {
            gameOver = true;
            loser = s;
            SDL_Log("%d perdeu...", loser);
        }

        // verifica colisão com si mesmas
        for (int i = 1; i < snake[s].size; i++)
            if (snake[s].body[0].x == snake[s].body[i].x && snake[s].body[0].y == snake[s].body[i].y) {
                gameOver = true;
                loser = s;
                SDL_Log("%d perdeu...", loser);
            }
    }

    // verifica colisão entre as cobras
    for (int i = 1; i < snake[0].size; i++) {
        if (snake[0].body[0].x == snake[1].body[i].x && snake[0].body[0].y == snake[1].body[i].y) {
            gameOver = true;
            loser = 0;
            SDL_Log("%d perdeu...", loser);
        }

        if (snake[1].body[0].x == snake[0].body[i].x && snake[1].body[0].y == snake[0].body[i].y) {
            gameOver = true;
            loser = 1;
            SDL_Log("%d perdeu...", loser);
        }
    }
}

void updateFood() {
    for (int s = 0; s < 2; s++)
        // verifica se alguma cobra comeu a comida
        if (snake[s].body[0].x == food.x && snake[s].body[0].y == food.y) {
            // aumenta o tamanho da cobra correspondente
            snake[s].size++;
            snake[s].body[snake[s].size - 1].x = snake[s].body[snake[s].size - 2].x;
            snake[s].body[snake[s].size - 1].y = snake[s].body[snake[s].size - 2].y;

            // aumenta ligeiramente a velocidade do jogo
            cycle--;

            // gera uma nova comida em uma posição aleatória
            food.x = rand() % gridWidth;
            food.y = rand() % gridHeight;

            // garante que a comida não foi gerada sobre o corpo de uma das cobras
            for (int s = 0; s < 2; s++) {
                bool onBody = true;
                while (onBody) {
                    onBody = false;
                    for (int i = 1; i < snake[s].size; i++)
                        if (food.x == snake[s].body[i].x && food.y == snake[s].body[i].y) {
                            food.x = rand() % gridWidth;
                            food.y = rand() % gridHeight;
                            onBody = true;
                            break;
                        }
                }
            }
        }
}

void render() {
    // limpa a tela
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // desenha as cobras
    for (int s = 0; s < 2; s++) {
        SDL_SetRenderDrawColor(renderer, snake[s].color.r, snake[s].color.g, snake[s].color.b, snake[s].color.a);
        for (int i = 0; i < snake[s].size; i++) {
            SDL_Rect rect = {snake[s].body[i].x * gridSize, snake[s].body[i].y * gridSize, gridSize, gridSize};
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    // desenha a comida
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect food_rect = {food.x * gridSize, food.y * gridSize, gridSize, gridSize};
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

    SDL_Event event;
    int wait = cycle;

    // inicializa a cobra 1
    snake[0].body[0].x = gridWidth / 4;
    snake[0].body[0].y = gridHeight / 2;
    snake[0].size = 3;
    snake[0].direction = 1;
    snake[0].color = (SDL_Color){0, 255, 0, 0};

    // inicializa a cobra 2
    snake[1].body[0].x = 2 * gridWidth / 3;
    snake[1].body[0].y = gridHeight / 2;
    snake[1].size = 3;
    snake[1].direction = 1;
    snake[1].color = (SDL_Color){0, 0, 255, 0};

    // gera a comida inicial
    food.x = gridWidth / 2;
    food.y = gridHeight / 2;

    // loop principal
    while (!gameOver) {
        if (timeout(&event, &wait)) {
            inputs(&event);
        } else {
            updatePosition();
            checkCollisions();
            updateFood();
        }

        render();
    }

    quit();
    return 0;
}