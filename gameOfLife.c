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

static bool sCellArr1[ARR_SIZE];
static bool sCellArr2[ARR_SIZE];
static bool* sCurrentGen = sCellArr1;
static bool* sNextGen = sCellArr2;

//If the coords go out of bounds, 
//then wrap them to the other side of the screen.
void boundsWrapAround(int* x, int* y) {
    if (*x >= WIDTH)  *x -= WIDTH;
    else if (*x < 0)  *x += WIDTH;
    if (*y >= HEIGHT) *y -= HEIGHT;
    else if (*y < 0)  *y += HEIGHT;
}

int indexFromCoords(int x, int y, bool shouldDoBoundsCheck) {
	if (shouldDoBoundsCheck) boundsWrapAround(&x,&y);
    return WIDTH * y + x;
}

int main(void) {
    InitWindow(SCREEN_W, SCREEN_H, "game of life");
    SetTargetFPS(7);
    
    //generate random alive starting cells
    srand((unsigned)time(NULL));
    int numStartingCells = ARR_SIZE / 10;
    for (int i = 0; i < numStartingCells; ++i) {        
        int rand_x = rand() % WIDTH, rand_y = rand() % HEIGHT;
        sCellArr1[indexFromCoords(rand_x, rand_y, false)] = 1;
    }

    while (!WindowShouldClose()) {
        
        for (int i = 0; i < ARR_SIZE; ++i) {
            int x = i % WIDTH;
            int y = i / HEIGHT;
            int aliveNeighbors = 0;

            if (sCurrentGen[indexFromCoords(x    , y - 1, true)]) ++aliveNeighbors;
            if (sCurrentGen[indexFromCoords(x - 1, y - 1, true)]) ++aliveNeighbors;
            if (sCurrentGen[indexFromCoords(x - 1, y    , true)]) ++aliveNeighbors;
            if (sCurrentGen[indexFromCoords(x - 1, y + 1, true)]) ++aliveNeighbors;
            if (sCurrentGen[indexFromCoords(x    , y + 1, true)]) ++aliveNeighbors;
            if (sCurrentGen[indexFromCoords(x + 1, y + 1, true)]) ++aliveNeighbors;
            if (sCurrentGen[indexFromCoords(x + 1, y    , true)]) ++aliveNeighbors;
            if (sCurrentGen[indexFromCoords(x + 1, y - 1, true)]) ++aliveNeighbors;

            if (aliveNeighbors >= 4 || aliveNeighbors <= 1) {sNextGen[i] = false;}
            else if (aliveNeighbors == 3) {sNextGen[i] = true;}
            else {sNextGen[i] = sCurrentGen[i];}                          
        }
        
        //draw cells  
        BeginDrawing();
        ClearBackground(BLACK); 
        int x = 0, y = 0;
        for (int i = 0; i < ARR_SIZE; ++i) {
            if (sNextGen[i]) {
                x = (i % WIDTH) * CELL_WIDTH;
                y = (i / HEIGHT) * CELL_WIDTH;
                DrawRectangle(x, y, CELL_WIDTH, CELL_WIDTH, WHITE);
            }
        }
        
        //swap cell arrays
        bool* temp_ptr = sCurrentGen;
        sCurrentGen = sNextGen;
        sNextGen = temp_ptr;

        EndDrawing();
    }
}