#include <SFML/Graphics.h>
#include <string.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600
#define CELL_SIZE 4

char world[WIDTH / CELL_SIZE][HEIGHT / CELL_SIZE] = { 0 };

void update() {
    char tmp[WIDTH / CELL_SIZE][HEIGHT / CELL_SIZE] = { 0 };
    
    for (int x = 0; x < WIDTH / CELL_SIZE; ++x) {
        for (int y = HEIGHT / CELL_SIZE - 1; y >= 0; --y) {
            if (world[x][y] == 1) {
                tmp[x][y] = 1;
                // Essayer de tomber en bas
                if (y < HEIGHT / CELL_SIZE - 1 && !world[x][y + 1]) {
                    tmp[x][y] = 0;
                    tmp[x][y + 1] = 1;
                }
                else if (x > 0 && y < HEIGHT / CELL_SIZE - 1 && !world[x - 1][y + 1]) {
                    tmp[x][y] = 0;
                    tmp[x - 1][y + 1] = 1;
                }
                else if (x < WIDTH / CELL_SIZE - 1 && y < HEIGHT / CELL_SIZE - 1 && !world[x + 1][y + 1]) {
                    tmp[x][y] = 0;
                    tmp[x + 1][y + 1] = 1;
                }
            }
            if (world[x][y] == 2) {
                char moved = 0;

                tmp[x][y] = 2;
                // Essayer de tomber en bas
                if (y < HEIGHT / CELL_SIZE - 1 && !world[x][y + 1]) {
                    tmp[x][y] = 0;
                    tmp[x][y + 1] = 2;
                    moved = 1;
                }

                // Essayer de tomber en bas à gauche
                if (!moved && x > 0 && y < HEIGHT / CELL_SIZE - 1 && !world[x - 1][y + 1]) {
                    tmp[x][y] = 0;
                    tmp[x - 1][y + 1] = 2;
                    moved = 1;
                }

                // Essayer de tomber en bas à droite
                if (!moved && x < WIDTH / CELL_SIZE - 1 && y < HEIGHT / CELL_SIZE - 1 && !world[x + 1][y + 1]) {
                    tmp[x][y] = 0;
                    tmp[x + 1][y + 1] = 2;
                    moved = 1;
                }
                
                if (!moved && x > 0 && !world[x - 1][y]) {
                    tmp[x][y] = 0;
                    tmp[x - 1][y] = 2;
                    moved = 1;
                }
                
                if (!moved && x < WIDTH / CELL_SIZE - 1 && !world[x + 1][y]) {
                    tmp[x][y] = 0;
                    tmp[x + 1][y] = 2;
                    moved = 1;
                }

            }
        }
    }

    memcpy(world, tmp, sizeof(world));
}

void draw(sfRenderWindow* window) {
    sfRectangleShape* rect = sfRectangleShape_create();
    for (int x = 0; x < WIDTH / CELL_SIZE; ++x) {
        for (int y = 0; y < HEIGHT / CELL_SIZE; ++y) {
            if (world[x][y] == 1) {
                sfRectangleShape_setFillColor(rect, sfColor_fromRGB(255, 255, 0));
            } else if (world[x][y] == 2) {
                sfRectangleShape_setFillColor(rect, sfColor_fromRGB(0, 0, 255));
            }
            if (world[x][y]) {
                sfRectangleShape_setPosition(rect, (sfVector2f) { x * CELL_SIZE, y * CELL_SIZE });
                sfRectangleShape_setSize(rect, (sfVector2f) { CELL_SIZE, CELL_SIZE });
                sfRenderWindow_drawRectangleShape(window, rect, NULL);
            }
        }
    }
    sfRectangleShape_destroy(rect);
}

void print_scene() {
    for (int x = 0; x < WIDTH / CELL_SIZE; ++x) {
        for (int y = 0; y < HEIGHT / CELL_SIZE; ++y) {
            printf("%d ", world[x][y]);
        }
        printf("\n");
    }
}

int main() {
    sfRenderWindow* window = sfRenderWindow_create(
        (sfVideoMode) { WIDTH, HEIGHT, 32 },
        "Falling Sand Simulation",
        sfClose,
        NULL
    );

    sfRenderWindow_setFramerateLimit(window, 30);

    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
        }

        sfVector2i mouse_position = sfMouse_getPositionRenderWindow(window);
        int cellX = mouse_position.x / CELL_SIZE;
        int cellY = mouse_position.y / CELL_SIZE;

        if (sfMouse_isButtonPressed(sfMouseLeft)) {
            if (cellX >= 0 && cellX < WIDTH / CELL_SIZE && cellY >= 0 && cellY < HEIGHT / CELL_SIZE) {
                world[cellX][cellY] = 1;
            }
        } else if (sfMouse_isButtonPressed(sfMouseRight)) {
            if (cellX >= 0 && cellX < WIDTH / CELL_SIZE && cellY >= 0 && cellY < HEIGHT / CELL_SIZE) {
                world[cellX][cellY] = 2;
            }
        }

        update();

        sfRenderWindow_clear(window, sfBlack);
        draw(window);
        sfRenderWindow_display(window);
    }

//    print_scene();
    sfRenderWindow_destroy(window);

    return 0;
}
