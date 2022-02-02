#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_W   800
#define SCREEN_H   800
#define CELL_WIDTH 4 //width of cell in pixels
#define WIDTH      (SCREEN_W / CELL_WIDTH)
#define HEIGHT     (SCREEN_H / CELL_WIDTH)
#define ARR_SIZE   (HEIGHT * WIDTH)

bool curr_g[ARR_SIZE];
bool next_g[ARR_SIZE];
bool* curr_ptr = curr_g;
bool* next_ptr = next_g;

void boundsClamp(int* x, int* y) {
    if ((*x) >= WIDTH)  (*x) -= WIDTH;
    else if ((*x) < 0)  (*x) += WIDTH;
    if ((*y) >= HEIGHT) (*y) -= HEIGHT;
    else if ((*y) < 0)  (*y) += HEIGHT;
}

int indexFromCoords(int x, int y, bool isRead) {
	if (isRead) boundsClamp(&x,&y);
    return WIDTH * y + x;
}

int main(void) {
    InitWindow(SCREEN_W, SCREEN_H, "game of life");
    SetTargetFPS(7);
       
    //generate rng alive cells
    srand(time(NULL));
    int num_of_starter_cells = ARR_SIZE / 10;
    for (int i = 0; i < num_of_starter_cells; i++) {        
        int rng = rand() % ARR_SIZE;
        curr_g[rng] = 1;
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
     
        for (int i = 0; i < ARR_SIZE; i++) {
            int x = i % WIDTH;
            int y = i / HEIGHT;
            int currIndex = indexFromCoords(x, y, 0);
            int alive_neighbors = 0;

            if (curr_ptr[indexFromCoords(x    , y - 1, 1)]) alive_neighbors++;
            if (curr_ptr[indexFromCoords(x - 1, y - 1, 1)]) alive_neighbors++;
            if (curr_ptr[indexFromCoords(x - 1, y    , 1)]) alive_neighbors++;
            if (curr_ptr[indexFromCoords(x - 1, y + 1, 1)]) alive_neighbors++;
            if (curr_ptr[indexFromCoords(x    , y + 1, 1)]) alive_neighbors++;
            if (curr_ptr[indexFromCoords(x + 1, y + 1, 1)]) alive_neighbors++;
            if (curr_ptr[indexFromCoords(x + 1, y    , 1)]) alive_neighbors++;
            if (curr_ptr[indexFromCoords(x + 1, y - 1, 1)]) alive_neighbors++;

            if (alive_neighbors >= 4 || alive_neighbors <= 1) {
                next_ptr[indexFromCoords(x, y, 0)] = 0;
            } else if (alive_neighbors == 3) {
                next_ptr[indexFromCoords(x, y, 0)] = 1;
            } else {
                next_ptr[currIndex] = curr_ptr[currIndex];
            }                   
        }
        //draw cells
        int x = 0, y = 0;
        for (int i = 0; i < ARR_SIZE; i++) {
            if (next_ptr[i]) {
                x = (i % WIDTH) * CELL_WIDTH;
                y = (i / HEIGHT) * CELL_WIDTH;
                DrawRectangle(x, y, CELL_WIDTH, CELL_WIDTH, WHITE);
            }
        }
        //swap pointers
        bool* ptr_buffer = curr_ptr;
        curr_ptr = next_ptr;
        next_ptr = ptr_buffer;
     
        EndDrawing();
    }
}
