#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#define title "Kobras"

#define windowWidth 800
#define windowHeight 800
#define gridSize 40
#define spriteSize 64

#define gridWidth (windowWidth / gridSize)
#define gridHeight (windowHeight / gridSize)
#define gridColumns (gridWidth * gridSize)
#define gridRows (gridHeight * gridSize)

#define initialCycle 150

typedef struct {
    int x;
    int y;
} Segment;

typedef struct {
    Segment body[1000];  // tamanho máximo da cobra
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

typedef struct {
    Segment body[60];
    int size;
    int toggle;
    int orientation;
} Wall;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

Snake snake[2];
Food food;
Winner winner;
Wall wall;

SDL_Rect r1 = {-windowWidth * 2, 0, windowWidth * 2, windowHeight / 3 + 1};
SDL_Rect r2 = {-windowWidth * 2.5, windowHeight / 3, windowWidth * 2, windowHeight / 3 + 1};
SDL_Rect r3 = {-windowWidth * 3, windowHeight * 2 / 3, windowWidth * 2, windowHeight / 3 + 1};

SDL_Texture* snakesSprite;
SDL_Texture* foodSprite;
SDL_Texture* wallSprite;

int cycle = initialCycle;
bool gameOver = false;
bool close = false;
int queue = 1;

void init() {
    // inicializações
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    IMG_Init(0);

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, windowWidth,
                              windowHeight, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    snakesSprite = IMG_LoadTexture(renderer, "kobras/resources/sprites/snakes.png");
    foodSprite = IMG_LoadTexture(renderer, "kobras/resources/sprites/food.png");
    wallSprite = IMG_LoadTexture(renderer, "kobras/resources/sprites/wall.png");
}

void start(int* foods, int* moves) {
    // prepara eventos aleatorios
    srand(time(0));

    // define a cobra 0
    snake[0].size = 3;
    for (int i = 0; i < snake[0].size; i++) {
        snake[0].body[i].x = -1;
        snake[0].body[i].y = gridHeight / 2;
    }
    snake[0].direction[0] = 1;
    snake[0].direction[1] = 1;
    snake[0].direction[2] = -1;
    snake[0].color = (SDL_Color){100, 160, 100, 255};

    // define a cobra 1
    snake[1].size = 3;
    for (int i = 0; i < snake[1].size; i++) {
        snake[1].body[i].x = gridWidth + 1;
        snake[1].body[i].y = gridHeight / 2;
    }
    snake[1].direction[0] = 3;
    snake[1].direction[1] = 3;
    snake[1].direction[2] = -1;
    snake[1].color = (SDL_Color){70, 130, 190, 255};

    // define a comida
    food.x = gridWidth / 2;
    food.y = gridHeight / 2;
    food.color = (SDL_Color){220, 70, 70, 255};

    // define a parede
    wall.toggle = 0;
    wall.size = gridWidth - 6;

    // define contadores
    *foods = 0;
    *moves = 0;
}

void render() {
    // limpa a tela
    SDL_SetRenderDrawColor(renderer, 250, 230, 150, 255);
    SDL_RenderClear(renderer);

    // desenha as cobras
    int x, y;
    for (int s = 0; s < 2; s++) {
        for (int i = 0; i < snake[s].size; i++) {
            if (i == 0) {  // cabeça
                if (snake[s].direction[0] == 0) {
                    x = 3;
                    y = 0;
                } else if (snake[s].direction[0] == 1) {
                    x = 4;
                    y = 0;
                } else if (snake[s].direction[0] == 2) {
                    x = 4;
                    y = 1;
                } else if (snake[s].direction[0] == 3) {
                    x = 3;
                    y = 1;
                }

            } else if (i == snake[s].size - 1) {  // cauda
                if (snake[s].body[i].y > snake[s].body[i - 1].y) {
                    x = 3;
                    y = 2;
                } else if (snake[s].body[i].x < snake[s].body[i - 1].x) {
                    x = 4;
                    y = 2;
                } else if (snake[s].body[i].y < snake[s].body[i - 1].y) {
                    x = 4;
                    y = 3;
                } else if (snake[s].body[i].x > snake[s].body[i - 1].x) {
                    x = 3;
                    y = 3;
                }

            } else {  // corpo
                x = 1;
                y = 1;

                // sem curvas
                if (snake[s].body[i].y == snake[s].body[i - 1].y) {
                    x = 1;
                    y = 0;
                } else if (snake[s].body[i].x == snake[s].body[i - 1].x) {
                    x = 2;
                    y = 1;
                }

                // subindo
                if (snake[s].body[i].y < snake[s].body[i + 1].y) {
                    if (snake[s].body[i].x < snake[s].body[i - 1].x) {  // virou à direita
                        x = 0;
                        y = 0;
                    } else if (snake[s].body[i].x > snake[s].body[i - 1].x) {  // virou à esquerda
                        x = 2;
                        y = 0;
                    }
                }

                // direita
                else if (snake[s].body[i].x > snake[s].body[i + 1].x) {
                    if (snake[s].body[i].y < snake[s].body[i - 1].y) {  // virou para baixo
                        x = 2;
                        y = 0;
                    } else if (snake[s].body[i].y > snake[s].body[i - 1].y) {  // virou para cima
                        x = 2;
                        y = 2;
                    }
                }

                // descendo
                else if (snake[s].body[i].y > snake[s].body[i + 1].y) {
                    if (snake[s].body[i].x < snake[s].body[i - 1].x) {  // virou à direita
                        x = 0;
                        y = 1;
                    } else if (snake[s].body[i].x > snake[s].body[i - 1].x) {  // virou à esquerda
                        x = 2;
                        y = 2;
                    }
                }

                // esquerda
                else if (snake[s].body[i].x < snake[s].body[i + 1].x) {
                    if (snake[s].body[i].y < snake[s].body[i - 1].y) {  // virou para baixo
                        x = 0;
                        y = 0;
                    } else if (snake[s].body[i].y > snake[s].body[i - 1].y) {  // virou para cima
                        x = 0;
                        y = 1;
                    }
                }
            }

            if (s == 1)
                x += 5;

            SDL_Rect spriteMask = {x * spriteSize, y * spriteSize, spriteSize, spriteSize};
            SDL_Rect spritePosition = {snake[s].body[i].x * gridSize, snake[s].body[i].y * gridSize, gridSize, gridSize};
            SDL_RenderCopy(renderer, snakesSprite, &spriteMask, &spritePosition);
        }
    }

    // desenha a comida
    SDL_Rect spriteMask = {0, 0, spriteSize, spriteSize};
    SDL_Rect spritePosition = {food.x * gridSize, food.y * gridSize, gridSize, gridSize};
    SDL_RenderCopy(renderer, foodSprite, &spriteMask, &spritePosition);

    // desenha a parede
    if (wall.toggle > 0)
        for (int i = 0; i < wall.size; i++) {
            if (wall.orientation == 0) {
                x = 0;
                if (i == 0)
                    y = 0;
                else if (i == wall.size - 1) {
                    y = 2;
                } else
                    y = 1;
            }

            else if (wall.orientation == 1) {
                y = 1;
                if (i == 0)
                    x = 1;
                else if (i == wall.size - 1)
                    x = 3;
                else
                    x = 2;
            }

            if (wall.toggle == 1)
                SDL_SetTextureAlphaMod(wallSprite, 70);
            else
                SDL_SetTextureAlphaMod(wallSprite, 255);

            SDL_Rect spriteMask = {x * spriteSize, y * spriteSize, spriteSize, spriteSize};
            SDL_Rect spritePosition = {wall.body[i].x * gridSize, wall.body[i].y * gridSize, gridSize, gridSize};
            SDL_RenderCopy(renderer, wallSprite, &spriteMask, &spritePosition);
        }

    // desenha o game over
    SDL_SetRenderDrawColor(renderer, winner.color.r, winner.color.g, winner.color.b, winner.color.a);
    SDL_RenderFillRect(renderer, &r1);
    SDL_RenderFillRect(renderer, &r2);
    SDL_RenderFillRect(renderer, &r3);

    // renderiza a tela
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

        // altera o movimento de acordo com o próximo da fila
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
        // verifica colisão com as bordas
        if (snake[s].body[0].x < 0 || snake[s].body[0].x >= gridWidth ||
            snake[s].body[0].y < 0 || snake[s].body[0].y >= gridHeight) {
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
            if (snake[s].body[0].x == snake[s].body[i].x &&
                snake[s].body[0].y == snake[s].body[i].y) {
                gameOver = true;
                if (s == 0) {
                    winner.number = 1;
                    winner.color = snake[1].color;
                } else {
                    winner.number = 0;
                    winner.color = snake[0].color;
                }
            }

        // verifica colisão com a parede
        if (wall.toggle == 2) {
            for (int i = 0; i < wall.size; i++)
                if (snake[s].body[0].x == wall.body[i].x &&
                    snake[s].body[0].y == wall.body[i].y) {
                    gameOver = true;
                    if (s == 0) {
                        winner.number = 1;
                        winner.color = snake[1].color;
                    } else {
                        winner.number = 0;
                        winner.color = snake[0].color;
                    }
                }

            // se for corpo com parede
            for (int i = 1; i < snake[s].size; i++)
                for (int j = 0; j < wall.size; j++) {
                    if (snake[s].body[i].x == wall.body[j].x &&
                        snake[s].body[i].y == wall.body[j].y) {
                        snake[s].size--;
                    }
                }
        }
    }

    // verifica colisão com o adversário
    for (int i = 1; i < snake[1].size; i++)
        if (snake[0].body[0].x == snake[1].body[i].x &&
            snake[0].body[0].y == snake[1].body[i].y) {
            gameOver = true;
            winner.number = 1;
            winner.color = snake[1].color;
        }

    for (int i = 1; i < snake[0].size; i++)
        if (snake[1].body[0].x == snake[0].body[i].x &&
            snake[1].body[0].y == snake[0].body[i].y) {
            gameOver = true;
            winner.number = 0;
            winner.color = snake[0].color;
        }

    // se for cabeça com cabeça
    if (snake[0].body[0].x == snake[1].body[0].x &&
        snake[0].body[0].y == snake[1].body[0].y) {
        gameOver = true;
        winner.number = 2;
        winner.color = food.color;
    }
}

int eat() {
    for (int s = 0; s < 2; s++)
        // verifica se alguma cobra comeu a comida
        if (snake[s].body[0].x == food.x && snake[s].body[0].y == food.y) {
            // aumenta o tamanho da cobra correspondente
            snake[s].size++;
            snake[s].body[snake[s].size - 1].x =
                snake[s].body[snake[s].size - 2].x;
            snake[s].body[snake[s].size - 1].y =
                snake[s].body[snake[s].size - 2].y;

            // aumenta ligeiramente a velocidade do jogo
            cycle -= 2;

            // gera uma nova comida em uma posição aleatória
            food.x = rand() % gridWidth;
            food.y = rand() % gridHeight;

            // garante que a comida não foi gerada sobre as cobras
            for (int s = 0; s < 2; s++) {
                bool onBody = true;
                while (onBody) {
                    onBody = false;
                    for (int i = 0; i < snake[s].size; i++)
                        if (food.x == snake[s].body[i].x &&
                            food.y == snake[s].body[i].y) {
                            food.x = rand() % gridWidth;
                            food.y = rand() % gridHeight;
                            onBody = true;
                            break;
                        }
                }
            }

            return 1;
        }

    // garante que a comida não está sobre a parede
    if (wall.toggle == 2) {
        bool onWall = true;
        while (onWall) {
            onWall = false;
            for (int i = 0; i < wall.size; i++)
                if (food.x == wall.body[i].x && food.y == wall.body[i].y) {
                    food.x = rand() % gridWidth;
                    food.y = rand() % gridHeight;
                    onWall = true;
                    break;
                }
        }
    }

    return 0;
}

void toggle(int* foods, int* moves) {
    // controla o surgimento da parede
    static bool set;
    static int movesSnapshot = 0;

    if (wall.toggle == 0)
        set = true;

    if (set) {
        if (*foods >= 3) {
            wall.orientation = rand() % 2;
            for (int i = 0; i <= wall.size; i++) {
                if (wall.orientation == 0) {
                    wall.body[i].x = gridWidth / 2;
                    wall.body[i].y = i + 3;
                } else {
                    wall.body[i].x = i + 3;
                    wall.body[i].y = gridWidth / 2;
                }
            }
            wall.toggle = 1;
            movesSnapshot = *moves;

            set = false;
        }
    } else {
        if (*moves - movesSnapshot == 30) {
            wall.toggle = 2;
            *foods = 0;
        }

        if (wall.toggle == 2)
            if (*foods == 3) {
                wall.toggle = 0;
                *foods = 0;

                set = true;
            }
    }
}

void quit() {
    // libera os recursos alocados pelo SDL
    SDL_DestroyTexture(snakesSprite);
    SDL_DestroyTexture(foodSprite);
    SDL_DestroyTexture(wallSprite);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    int moves;
    int foods;

    init();
    start(&foods, &moves);

    SDL_Event event;
    int wait = cycle;

    // loop principal
    while (!close) {
        moves = 0;
        foods = 0;
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

                foods += eat();
                toggle(&foods, &moves);
            }
        }
        cycle = 15;

        render();
        if (timeout(&event, &wait))
            input(&event);
        else {
            if (r3.x < windowWidth) {
                r1.x += 20;
                r2.x += 20;
                r3.x += 20;

                // reinicia posições
                if (r3.x >= -windowWidth) {
                    start(&foods, &moves);
                }
            } else {
                // reinicia o resto
                cycle = initialCycle;
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