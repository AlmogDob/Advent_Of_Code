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
    int hori_refl_index;
    int vert_refl_index;
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
int get_num_diff_in_row(Patern *patern, int r1, int r2);
int get_num_diff_in_col(Patern *patern, int c1, int c2);
int get_index_1_diff_row(Patern *patern); 
int get_index_1_diff_col(Patern *patern); 
void set_refl_index(Patern *patern);

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
    int line_len;
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

        temp_patern.hori_refl_index = -1;
        temp_patern.vert_refl_index = -1;

        ada_appand(Patern, paterns, temp_patern);
    }

    // print_list_of_paterns(&paterns);
    print_patern(&(paterns.elements[0]));
    
    dprintINT(get_index_1_diff_col(&(paterns.elements[0])));
    dprintINT(get_max_num_same_col(&(paterns.elements[0])));

    return 0;
}

size_t offset2D(size_t x, size_t y, size_t nx)
{
    return x + y * nx;
}

void print_patern(Patern *patern)
{
    printf("===============\n");
    printf("   | ");
    for (int i = 0; i < patern->nx - 1; i++) {
        printf("%d ", get_num_diff_in_col(patern, i, i+1));
    }
    printf("\n");
    printf("---+");
    for (int i = 0; i < patern->nx; i++) {
        printf("--");
    }
    printf("\n");
    for (int y = 0; y < patern->ny; y++) {
        if (y < patern->ny - 1) {
            printf("%2d | ", get_num_diff_in_row(patern, y, y+1));
        } else {
            printf("   | ");
        }
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
        printf("\n%zu\n", i);
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

    while (same_row(patern, r1 - i, r2 + i)) {
        if (r1 - i == 0 || r2 + i == patern->ny - 1) {
            num = i + 1;
            break;
        }
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

    while (same_col(patern, c1 - i, c2 + i)) {
        if (c1 - i == 0 || c2 + i == patern->nx - 1) {
            num = i + 1;
            break;
        }
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

int get_num_diff_in_row(Patern *patern, int r1, int r2)
{
    int num = 0; 

    for (int i = 0; i < patern->nx; i++) {
        if (patern->elements[offset2D(i, r1, patern->nx)] != patern->elements[offset2D(i, r2, patern->nx)]) {
            num++;
        }
    }
    return num;
}

int get_num_diff_in_col(Patern *patern, int c1, int c2)
{
    int num = 0; 

    for (int i = 0; i < patern->ny; i++) {
        if (patern->elements[offset2D(c1, i, patern->nx)] != patern->elements[offset2D(c2, i, patern->nx)]) {
            num++;
        }
    }
    return num;
}

int get_index_1_diff_row(Patern *patern)
{
    int index = -1, num_diff, current_dist_from_edge, new_dist_from_edge; 

    for (int i = 0; i < patern->ny - 1; i++) {
        num_diff = get_num_diff_in_row(patern, i, i+1);
        if (num_diff == 1 && index == -1) {
            index = i;
        } else if (num_diff == 1 && index != -1) {
            /* calculate current dist from edge */
            if (index < (int)ceil((patern->ny - 1) / 2)) {
                current_dist_from_edge = index;
            } else if (index > (int)ceil((patern->ny - 1) / 2)) {
                current_dist_from_edge = patern->ny - index;
            }
            /* calculate new dist from edge */
            if (i < (int)ceil((patern->ny - 1) / 2)) {
                new_dist_from_edge = i;
            } else if (i > (int)ceil((patern->ny - 1) / 2)) {
                new_dist_from_edge = patern->ny - i;
            }
            if (new_dist_from_edge < current_dist_from_edge) {
                index = i;
            }
        }
    }

    return index;
} 

int get_index_1_diff_col(Patern *patern)
{
    int index = -1, num_diff, current_dist_from_edge, new_dist_from_edge; 

    for (int i = 0; i < patern->nx - 1; i++) {
        num_diff = get_num_diff_in_col(patern, i, i+1);
        if (num_diff == 1 && index == -1) {
            index = i;
        } else if (num_diff == 1 && index != -1) {
            /* calculate current dist from edge */
            if (index < (int)ceil((patern->nx - 1) / 2)) {
                current_dist_from_edge = index;
            } else if (index > (int)ceil((patern->nx - 1) / 2)) {
                current_dist_from_edge = patern->nx - index;
            }
            /* calculate new dist from edge */
            if (i < (int)ceil((patern->nx - 1) / 2)) {
                new_dist_from_edge = i;
            } else if (i > (int)ceil((patern->nx - 1) / 2)) {
                new_dist_from_edge = patern->nx - i;
            }
            if (new_dist_from_edge < current_dist_from_edge) {
                index = i;
            }
        }
    }

    return index;
} 

void set_refl_index(Patern *patern)
{

}
