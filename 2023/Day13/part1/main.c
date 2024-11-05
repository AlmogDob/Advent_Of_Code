#include <stdlib.h>
#include <string.h>
#define ALMOG_AOC_IMPLEMENTATION
#include "Almog_AoC.h"
#include <math.h>

typedef struct {
    size_t length;
    size_t capacity;
    int* elements;
    int nx;
    int ny;
} Patern;

typedef struct {
    size_t length;
    size_t capacity;
    Patern* elements;
} ada_Patern_array;

size_t offset2D(size_t x, size_t y, size_t nx);
void print_patern(Patern *patern);

int main(int argc, char const *argv[])
{
    char input_dir[MAXDIR];
    FILE *fp;

    if (--argc != 1) {
        fprintf(stderr, "%s:%d: [ERROR]: not right usage\n", __FILE__, __LINE__);
        return -1;
    }
    strncpy(input_dir, (*(++argv)), MAXDIR);

    fp = fopen(input_dir, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: couldn't find file path\n");
        return -1;
    }

/*------------------------------------------------------------*/
    int line_len, ny = 0, counter = 0; 
    char current_line[MAX_LEN_LINE], c;
    Patern temp_patern;
    ada_Patern_array paterns;

    ada_create_array(Patern, paterns);

    while ((line_len = aaoc_get_line(fp, current_line)) != -1) {
        dprintSTRING(current_line);
    }

    return 0;
}

size_t offset2D(size_t x, size_t y, size_t nx)
{
    return x + y * nx;
}

void print_patern(Patern *patern)
{
    printf("----------\n");
    for (int y = 0; y < patern->ny; y++) {
        for (int x = 0; x < patern->nx; x++) {
            printf("%d ", patern->elements[offset2D(x, y, patern->nx)]);
        }
        printf("\n");
    }
}
