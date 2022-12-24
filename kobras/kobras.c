#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#define title "Kobra"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define GRID_SIZE 20
#define GRID_WIDTH (WINDOW_WIDTH / GRID_SIZE)
#define GRID_HEIGHT (WINDOW_HEIGHT / GRID_SIZE)

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)

typedef struct {
    int x;
    int y;
} SnakeSegment;

typedef struct {
    SnakeSegment body[100];
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

bool game_over = false;

void init() {
    // Inicializa a SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error: %s\n", SDL_GetError());
        exit(1);
    }

    // Cria a janela
    window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (window == NULL) {
        printf("Error: %s\n", SDL_GetError());
        exit(1);
    }

    // Cria o renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Error: %s\n", SDL_GetError());
        exit(1);
    }
}

void process_input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                game_over = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        snake.direction = 0;
                        break;
                    case SDLK_RIGHT:
                        snake.direction = 1;
                        break;
                    case SDLK_DOWN:
                        snake.direction = 2;
                        break;
                    case SDLK_LEFT:
                        snake.direction = 3;
                        break;
                }
                break;
        }
    }
}

void update() {
    // Atualiza a posição da cobra
    for (int i = snake.size - 1; i > 0; i--) {
        snake.body[i] = snake.body[i - 1];
    }

    // Muda a direção da cobra de acordo com a direção atual
    if (snake.direction == 0) {
        snake.body[0].y--;
    } else if (snake.direction == 1) {
        snake.body[0].x++;
    } else if (snake.direction == 2) {
        snake.body[0].y++;
    } else if (snake.direction == 3) {
        snake.body[0].x--;
    }

    // Verifica se a cobra colidiu com a parede
    if (snake.body[0].x < 0 || snake.body[0].x >= GRID_WIDTH ||
        snake.body[0].y < 0 || snake.body[0].y >= GRID_HEIGHT) {
        game_over = true;
    }

    // Verifica se a cobra colidiu com a própria cauda
    for (int i = 1; i < snake.size; i++) {
        if (snake.body[0].x == snake.body[i].x && snake.body[0].y == snake.body[i].y) {
            game_over = true;
        }
    }

    // Verifica se a cobra comeu a comida
    if (snake.body[0].x == food.x && snake.body[0].y == food.y) {
        // Aumenta o tamanho da cobra
        snake.size++;

        // Gera uma nova comida em uma posição aleatória
        food.x = rand() % GRID_WIDTH;
        food.y = rand() % GRID_HEIGHT;
    }
}

void render() {
    // Limpa a tela
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Desenha a cobra
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < snake.size; i++) {
        SDL_Rect rect = {
            snake.body[i].x * GRID_SIZE,
            snake.body[i].y * GRID_SIZE,
            GRID_SIZE, GRID_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }

    // Desenha a comida
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect food_rect = {
        food.x * GRID_SIZE,
        food.y * GRID_SIZE,
        GRID_SIZE, GRID_SIZE};
    SDL_RenderFillRect(renderer, &food_rect);

    // Atualiza a tela
    SDL_RenderPresent(renderer);
}

void close() {
    // Libera os recursos alocados pela SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    init();

    // Inicializa a cobra
    snake.body[0].x = GRID_WIDTH / 2;
    snake.body[0].y = GRID_HEIGHT / 2;
    snake.size = 1;
    snake.direction = 1;

    // Gera uma comida em uma posição aleatória
    food.x = rand() % GRID_WIDTH;
    food.y = rand() % GRID_HEIGHT;

    // Loop do jogo
    while (!game_over) {
        // Lê os inputs do jogador
        process_input();

        // Atualiza o estado do jogo
        update();

        // Renderiza o jogo
        render();

        // Espera para manter a taxa de quadros por segundo
        SDL_Delay(FRAME_TARGET_TIME);
    }

    // Finaliza o jogo
    close();

    return 0;
}