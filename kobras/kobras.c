#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#define title "Kobras"

#define windowWidth 700
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
    int direction[3];
    SDL_Color color;
} Snake;

typedef struct {
    int x;
    int y;
    SDL_Color color;
} Food;

typedef struct {
    int number;
    SDL_Color color;
} Winner;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

Snake snake[2];
Food food;
Winner winner;

SDL_Rect r1 = {-windowWidth * 2, 0, windowWidth * 2, windowHeight / 3};
SDL_Rect r2 = {-windowWidth * 2.5, windowHeight / 3, windowWidth * 2, windowHeight / 3};
SDL_Rect r3 = {-windowWidth * 3, windowHeight * 2 / 3, windowWidth * 2, windowHeight / 3};

int cycle = 100;
bool gameOver = false;
bool close = false;
int queue = 1;
int loser;

void init() {
    // inicializações
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, windowWidth,
                              windowHeight, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, 0);
}

void start() {
    // define a cobra 0
    snake[0].size = 3;
    for (int i = 0; i < snake[0].size; i++) {
        snake[0].body[i].x = 3;
        snake[0].body[i].y = gridHeight / 2;
    }
    snake[0].direction[0] = 1;
    snake[0].direction[1] = 1;
    snake[0].direction[2] = -1;
    snake[0].color = (SDL_Color){0, 255, 0, 0};

    // define a cobra 1
    snake[1].size = 3;
    for (int i = 0; i < snake[1].size; i++) {
        snake[1].body[i].x = gridWidth - 4;
        snake[1].body[i].y = gridHeight / 2;
    }
    snake[1].direction[0] = 3;
    snake[1].direction[1] = 3;
    snake[1].direction[2] = -1;
    snake[1].color = (SDL_Color){0, 0, 255, 0};

    // define a comida
    food.x = gridWidth / 2;
    food.y = gridHeight / 2;
    food.color = (SDL_Color){255, 0, 0, 0};
}

void render() {
    // limpa a tela
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // desenha as cobras
    for (int s = 0; s < 2; s++) {
        SDL_SetRenderDrawColor(renderer, snake[s].color.r, snake[s].color.g, snake[s].color.b, snake[s].color.a);
        for (int i = 0; i < snake[s].size; i++) {
            SDL_Rect snakeRect = {snake[s].body[i].x * gridSize, snake[s].body[i].y * gridSize, gridSize, gridSize};
            SDL_RenderFillRect(renderer, &snakeRect);
        }
    }

    // redesenha a cobra 0 caso ela perca, para deixar claro ao jogador
    if (winner.number != 0) {
        SDL_SetRenderDrawColor(renderer, snake[0].color.r, snake[0].color.g, snake[0].color.b, snake[0].color.a);
        for (int i = 0; i < snake[0].size; i++) {
            SDL_Rect snakeRect = {snake[0].body[i].x * gridSize, snake[0].body[i].y * gridSize, gridSize, gridSize};
            SDL_RenderFillRect(renderer, &snakeRect);
        }
    }

    // desenha a comida
    SDL_SetRenderDrawColor(renderer, food.color.r, food.color.g, food.color.b, food.color.a);
    SDL_Rect foodRect = {food.x * gridSize, food.y * gridSize, gridSize, gridSize};
    SDL_RenderFillRect(renderer, &foodRect);

    // desenha os retângulos
    SDL_SetRenderDrawColor(renderer, winner.color.r, winner.color.g, winner.color.b, winner.color.a);
    SDL_RenderFillRect(renderer, &r1);
    SDL_RenderFillRect(renderer, &r2);
    SDL_RenderFillRect(renderer, &r3);

    // renderiza a tela
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderPresent(renderer);
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

void input(SDL_Event* event) {
    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
    switch (event->type) {
        case SDL_KEYDOWN:
            switch (event->key.keysym.sym) {
                // inputs da cobra 0
                case SDLK_w:
                    snake[0].direction[queue] = 0;
                    break;
                case SDLK_d:
                    snake[0].direction[queue] = 1;
                    break;
                case SDLK_s:
                    snake[0].direction[queue] = 2;
                    break;
                case SDLK_a:
                    snake[0].direction[queue] = 3;
                    break;

                // inputs da cobra 1
                case SDLK_UP:
                    snake[1].direction[queue] = 0;
                    break;
                case SDLK_RIGHT:
                    snake[1].direction[queue] = 1;
                    break;
                case SDLK_DOWN:
                    snake[1].direction[queue] = 2;
                    break;
                case SDLK_LEFT:
                    snake[1].direction[queue] = 3;
                    break;
            }
            break;

        case SDL_QUIT:
            close = true;
            break;
    }
    // estabelece uma fila de direções
    queue = 2;
}

void move() {
    for (int s = 0; s < 2; s++) {
        // atualiza a posição das cobras
        for (int i = snake[s].size - 1; i > 0; i--)
            snake[s].body[i] = snake[s].body[i - 1];

        // ignora comandos que movimentem a cobra contra ela mesma
        if (snake[s].direction[1] == 0 && snake[s].direction[0] == 2)
            snake[s].direction[1] = 2;
        else if (snake[s].direction[1] == 1 && snake[s].direction[0] == 3)
            snake[s].direction[1] = 3;
        else if (snake[s].direction[1] == 2 && snake[s].direction[0] == 0)
            snake[s].direction[1] = 0;
        else if (snake[s].direction[1] == 3 && snake[s].direction[0] == 1)
            snake[s].direction[1] = 1;

        // altera o movimento de acordo com o próximo sentido da fila
        if (snake[s].direction[1] == 0)
            snake[s].body[0].y--;
        else if (snake[s].direction[1] == 1)
            snake[s].body[0].x++;
        else if (snake[s].direction[1] == 2)
            snake[s].body[0].y++;
        else if (snake[s].direction[1] == 3)
            snake[s].body[0].x--;

        // grava o sentido do movimento atual
        snake[s].direction[0] = snake[s].direction[1];

        // avança a fila de inputs, se houver
        if (snake[s].direction[2] != -1) {
            snake[s].direction[1] = snake[s].direction[2];
            snake[s].direction[2] = -1;
        }
    }
}

void collide() {
    for (int s = 0; s < 2; s++) {
        // verifica colisão com as paredes
        if (snake[s].body[0].x < 0 || snake[s].body[0].x >= gridWidth || snake[s].body[0].y < 0 || snake[s].body[0].y >= gridHeight) {
            gameOver = true;
            if (s == 0) {
                winner.number = 1;
                winner.color = snake[1].color;
            } else {
                winner.number = 0;
                winner.color = snake[0].color;
            }
        }

        // verifica colisão com si mesmas
        for (int i = 1; i < snake[s].size; i++)
            if (snake[s].body[0].x == snake[s].body[i].x && snake[s].body[0].y == snake[s].body[i].y) {
                gameOver = true;
                if (s == 0) {
                    winner.number = 1;
                    winner.color = snake[1].color;
                } else {
                    winner.number = 0;
                    winner.color = snake[0].color;
                }
            }
    }

    // verifica colisão com o adversário
    for (int i = 1; i < snake[1].size; i++)
        if (snake[0].body[0].x == snake[1].body[i].x && snake[0].body[0].y == snake[1].body[i].y) {
            gameOver = true;
            winner.number = 1;
            winner.color = snake[1].color;
        }

    for (int i = 1; i < snake[0].size; i++)
        if (snake[1].body[0].x == snake[0].body[i].x && snake[1].body[0].y == snake[0].body[i].y) {
            gameOver = true;
            winner.number = 0;
            winner.color = snake[0].color;
        }

    if (snake[0].body[0].x == snake[1].body[0].x && snake[0].body[0].y == snake[1].body[0].y) {
        gameOver = true;
        winner.number = 2;
        winner.color = food.color;
    }
}

void eat() {
    for (int s = 0; s < 2; s++)
        // verifica se alguma cobra comeu a comida
        if (snake[s].body[0].x == food.x && snake[s].body[0].y == food.y) {
            // aumenta o tamanho da cobra correspondente
            snake[s].size++;
            snake[s].body[snake[s].size - 1].x = snake[s].body[snake[s].size - 2].x;
            snake[s].body[snake[s].size - 1].y = snake[s].body[snake[s].size - 2].y;

            // aumenta ligeiramente a velocidade do jogo
            cycle -= 2;

            // gera uma nova comida em uma posição aleatória
            srand(time(0));
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

void quit() {
    // libera os recursos alocados pelo SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    init();
    start();

    SDL_Event event;
    int wait = cycle;

    // loop principal
    while (!close) {
        int moves = 0;
        while (!close && !gameOver) {
            render();
            if (timeout(&event, &wait))
                input(&event);
            else {
                move();
                queue = 1;
                moves++;
                if (moves >= 3)
                    collide();
                eat();
            }
        }
        cycle = 15;

        render();

        if (timeout(&event, &wait))
            input(&event);
        else {
            if (r3.x < windowWidth) {
                r1.x += 15;
                r2.x += 15;
                r3.x += 15;

                // reinicia posições
                if (r3.x >= -windowWidth) {
                    start();
                }
            } else {
                // reinicia o resto
                cycle = 100;
                r1.x = -windowWidth * 2;
                r2.x = -windowWidth * 2.5;
                r3.x = -windowWidth * 3;

                gameOver = false;
            }
        }
    }

    quit();
    return 0;
}