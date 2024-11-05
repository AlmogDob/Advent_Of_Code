#include <stdlib.h>
#include <string.h>
#define ALMOG_AOC_IMPLEMENTATION
#include "Almog_AoC.h"
#include <math.h>
#include <assert.h>

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
void print_list_of_paterns(ada_Patern_array *list);
int same_row(Patern *patern, int r1, int r2);
int same_col(Patern *patern, int c1, int c2);
int get_num_same_row(Patern *patern, int r1, int r2);
int get_num_same_col(Patern *patern, int c1, int c2);
int get_max_num_same_row(Patern *patern);
int get_max_num_same_col(Patern *patern);

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
    int line_len, max_num_same_col = 0, max_num_same_row = 0;
    size_t sum = 0;
    char current_line[MAX_LEN_LINE], c;
    Patern temp_patern;
    ada_Patern_array paterns;

    ada_create_array(Patern, paterns);

    while ((line_len = aaoc_get_line(fp, current_line)) != -1) {
        ada_create_array(int, temp_patern);
        temp_patern.nx = 0;
        temp_patern.ny = 0;
        while (line_len != 0 && line_len != -1) {
            temp_patern.ny++;
            temp_patern.nx = line_len;

            for (int i = 0; i < line_len; i++) {
                c = current_line[i];
                if (c == '#') ada_appand(int, temp_patern, 1);
                if (c == '.') ada_appand(int, temp_patern, 0);
            }
            line_len = aaoc_get_line(fp, current_line);
        }

        ada_appand(Patern, paterns, temp_patern);
    }

    print_list_of_paterns(&paterns);

    for (size_t i = 0; i < paterns.length; i++) {
        max_num_same_col = get_max_num_same_col(&(paterns.elements[i])); 
        max_num_same_row = get_max_num_same_row(&(paterns.elements[i])); 
        // dprintINT(max_num_same_col);
        // dprintINT(max_num_same_row);
        if (max_num_same_col > max_num_same_row) {
            for (int j = 0; j < paterns.elements[i].nx - 1; j++) {
                if (max_num_same_col == get_num_same_col(&(paterns.elements[i]), j, j+1)) {
                    sum += j + 1;
                }
            }
        } else {
            for (int j = 0; j < paterns.elements[i].ny - 1; j++) {
                if (max_num_same_row == get_num_same_row(&(paterns.elements[i]), j, j+1)) {
                    sum += 100 * (j + 1);
                }
            }
        }
    }
    dprintSIZE_T(sum);

    return 0;
}

size_t offset2D(size_t x, size_t y, size_t nx)
{
    return x + y * nx;
}

void print_patern(Patern *patern)
{
    printf("===============\n");
    for (int y = 0; y < patern->ny; y++) {
        for (int x = 0; x < patern->nx; x++) {
            printf("%d ", patern->elements[offset2D(x, y, patern->nx)]);
        }
        printf("\n");
    }
    printf("===============\n");
}

void print_list_of_paterns(ada_Patern_array *list)
{
    for (size_t i = 0; i < list->length; i++) {
        printf("\n");
        print_patern(&(list->elements[i]));
        printf("\n");
    }
}

int same_row(Patern *patern, int r1, int r2)
{
    assert(r1 < patern->ny);
    assert(r2 < patern->ny);

    for (int x = 0; x < patern->nx; x++) {
        if (patern->elements[offset2D(x, r1, patern->nx)] != patern->elements[offset2D(x, r2, patern->nx)]) {
            return 0;
        }
    } 
    return 1;
}

int same_col(Patern *patern, int c1, int c2)
{
    assert(c1 < patern->nx);
    assert(c2 < patern->nx);

    for (int y = 0; y < patern->ny; y++) {
        if (patern->elements[offset2D(c1, y, patern->nx)] != patern->elements[offset2D(c2, y, patern->nx)]) {
            return 0;
        }
    } 
    return 1;

}

int get_num_same_row(Patern *patern, int r1, int r2)
{
    int num = 0, i = 0, temp;
    
    if (r1 > r2) {
        temp = r1;
        r1 = r2; 
        r2 = temp;
    }
    
    while (r1 - i >= 0 && r2 + i < patern->ny) {
        if (!same_row(patern, r1 - i, r2 + i)) {
            break;
        }
        num++;
        i++;
    }
    return num;
}

int get_num_same_col(Patern *patern, int c1, int c2)
{
    int num = 0, i = 0, temp;
    
    if (c1 > c2) {
        temp = c1;
        c1 = c2; 
        c2 = temp;
    }

    while (c1 - i >= 0 && c2 + i < patern->nx) {
        if (!same_col(patern, c1 - i, c2 + i)) {
            break;
        }
        num++;
        i++;
    }
    return num;
}

int get_max_num_same_row(Patern *patern)
{
    int max_num = 0, temp; 
    
    for (int i = 0; i < patern->ny - 1; i++) {
        temp = get_num_same_row(patern, i, i+1);
        if (max_num < temp) max_num = temp;
    }
    return max_num;
}

int get_max_num_same_col(Patern *patern)
{
    int max_num = 0, temp; 
    
    for (int i = 0; i < patern->nx - 1; i++) {
        temp = get_num_same_col(patern, i, i+1);
        if (max_num < temp) max_num = temp;
    }
    return max_num;
}

