#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define GRID_SIZE 81

void print_grid(int* grid) {
    for (int i = 0; i < GRID_SIZE; i++)
    {
        if (((i%3) == 0) && (i != 0) && ((i%9) != 0)) printf("| ");
        if ((i%9) == 0) printf("\n");
        if ((i%27) == 0) printf("------+-------+------ \n");
        printf("%d ", grid[i]);
    }  
}

void print_solution(int* initial_grid, int* solution) {
    printf("\n");
    printf("Initial Grid: ");
    print_grid(initial_grid);
    printf("\n");

    printf("\n");
    printf("Solution: ");
    print_grid(solution);
    printf("\n");
}

static
bool num_fits(int* solution, int row, int col, int num){
    // checking row ...
    for (int i = col; i < col + 9; i++)
    {
        if (solution[i] == num) return false;
    }

    // checking column
    for (int i = row; i < row + 73; i += 9)
    {
        if (solution[i] == num) return false;
    }
    
    // checking box
    int box_start = (row - row%3) + (col - col%27);
    for (int i = box_start; i < box_start + 3; i++)
    {
        if (solution[i] == num) return false;
        if (solution[i+9] == num) return false;
        if (solution[i+18] == num) return false;
    }
    
    return true;
}

static
void place_num(int* initial_grid, int* solution, int index){
    int row = index%9;
    int col = index - row;

    if (solution[index] == 0)
    { 
        for (int num = 1; num < 10; num++)
        {
            if (num_fits(solution, row, col, num))
            {
                solution[index] = num;
                if (index >= GRID_SIZE - 1) print_solution(initial_grid, solution); else place_num(initial_grid, solution, index + 1);
                solution[index] = 0;
            } 
        }
    } else
    {
        if (index >= GRID_SIZE - 1) print_solution(initial_grid, solution); else place_num(initial_grid, solution, index + 1);
    }
    
    
}

void sudoku(int* grid){
    int solution[GRID_SIZE] = {0};
    for (int i = 0; i < GRID_SIZE; i++)
    {
        solution[i] = grid[i];
    } 

    place_num(grid, solution, 0);
}

void test_sudoku(int* grid) {
    double time;
    clock_t begin = clock();
    sudoku(grid);
    clock_t end = clock();

    time = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("time: %.4f sec \n", time);
}

int* readGridFromFile(char* filename) {
    int* grid = (int*)malloc(GRID_SIZE * sizeof(int));
    FILE* file = fopen(filename, "r"); 

    if (file == NULL) {
        printf("Error: file not found'%s'\n", filename);
        exit(1);
    }


    for (int i = 0; i < GRID_SIZE; i++) {
        int value;
        if (fscanf(file, "%d, ", &value) != 1) {
            printf("Error while reading file'%s'\n", filename);
            exit(1);
        }
        grid[i] = value;
    }

    fclose(file); 

    return grid;
}

int main() {
    int* sudoku_grid_easy = readGridFromFile("../../resources/sudoku_grid_easy.txt"); 
    int* sudoku_grid_hard = readGridFromFile("../../resources/sudoku_grid_hard.txt");
    int* sudoku_grid_extrem = readGridFromFile("../../resources/sudoku_grid_extrem.txt");

    printf("Easy Sudoku: \n");
    test_sudoku(sudoku_grid_easy);

    printf("\nHard Sudoku: \n");
    test_sudoku(sudoku_grid_hard);

    printf("\nExtrem Sudoku: \n");
    test_sudoku(sudoku_grid_extrem);

    return 0;
}