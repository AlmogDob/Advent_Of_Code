#include <string.h>
#define ALMOG_AOC_IMPLEMENTATION
#include "Almog_AoC.h"
#define VEC2_IMPLEMENTATION
#include "Vec2.h"
#include <math.h>

typedef struct {
    size_t length;
    size_t capacity;
    int *elements;
} ada_int_array;

typedef struct {
    size_t length;
    size_t capacity;
    Vec2 *elements;
} ada_Vec2_array;

typedef struct {
    Vec2 galx1;
    Vec2 galx2;
    Vec2 des;
    int path_length;
} Glax_Pair;

typedef struct {
    size_t length;
    size_t capacity;
    Glax_Pair *elements;
} ada_Galx_Pair_array;

typedef struct {
    int nx;
    int ny;
    ada_int_array image;
    ada_Vec2_array galaxes;
    ada_Galx_Pair_array pairs;
} Image;

void init_image(Image *image);
void print_image_with_data(Image *image);
int offset2D(size_t x, size_t y, size_t nx);
int row_is_empty(Image *image, int y);
int col_is_empty(Image *image, int x);
void copy_image_to_image(Image *dst, Image *src);
void add_row(Image *image, int y);
void add_col(Image *image, int x);
void expand_image(Image *expended, Image *src);
void set_galaxes_array(Image *image);
void set_pairs_array(Image *image);

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
    int line_len, nx, ny = 0;
    char current_line[MAX_LEN_LINE];
    Image first_image, current_image;

    init_image(&first_image);
    init_image(&current_image);

    while ((line_len = aaoc_get_line(fp, current_line)) != -1) {
        dprintSTRING(current_line);
        ny++;
        nx = line_len;
        for (int i = 0; i < line_len; i++) {
            if (current_line[i] == '.') {
                ada_appand(int, first_image.image, 0);
            }
            if (current_line[i] == '#') {
                ada_appand(int, first_image.image, 1);
            }
        }
    }

    dprintINT(nx);
    dprintINT(ny);

    first_image.nx = nx;
    first_image.ny = ny;

    print_image_with_data(&first_image);

    copy_image_to_image(&current_image, &first_image);

    expand_image(&current_image, &first_image);

    print_image_with_data(&current_image);

    set_galaxes_array(&current_image);

    VEC2_PRINT(current_image.galaxes.elements[0]);
    dprintSIZE_T(current_image.galaxes.length);

    set_pairs_array(&current_image);

    dprintSIZE_T(current_image.pairs.length);
    VEC2_PRINT(current_image.pairs.elements[6].galx1);
    VEC2_PRINT(current_image.pairs.elements[6].galx2);
    dprintINT(current_image.pairs.elements[6].path_length);

    int sum = 0;

    for (size_t i = 0; i < current_image.pairs.length; i++) {
        sum += current_image.pairs.elements[i].path_length;
    }

    dprintINT(sum);


    return 0;
}

void init_image(Image *image)
{
    ada_array(int, image->image);
    ada_array(Vec2, image->galaxes);
    ada_array(Glax_Pair, image->pairs);
}

void print_image_with_data(Image *image)
{
    printf("\n  | ");
    for (int x = 0; x < image->nx; x++) {
        printf("%d ", col_is_empty(image, x));
    }
    printf("\n");
    printf("--+");
    for (int x = 0; x < image->nx; x++) {
        printf("--");
    }
    printf("\n");
    
    for (int y = 0; y < image->ny; y++) {
        printf("%d | ", row_is_empty(image, y));
        for (int x = 0; x < image->nx; x++) {
            printf("%d ", image->image.elements[offset2D(x, y, image->nx)]);
        }
        printf("\n");
    }
    printf("\n");
}

 int offset2D(size_t x, size_t y, size_t nx)
{
    return x + y * nx;
}

int row_is_empty(Image *image, int y)
{
    for (int x = 0; x < image->nx; x++){
        if (image->image.elements[offset2D(x, y, image->nx)] == 1) {
            return 0;
        }
    }
    return 1;
}

int col_is_empty(Image *image, int x)
{
    for (int y = 0; y < image->ny; y++){
        if (image->image.elements[offset2D(x, y, image->nx)] == 1) {
            return 0;
        }
    }
    return 1;
}


void copy_image_to_image(Image *dst, Image *src)
{
    dst->nx = src->nx;
    dst->ny = src->ny;

    for (size_t i = 0; i < src->image.length; i++) {
        ada_appand(int, dst->image, src->image.elements[i]);
    }
}

void add_row(Image *image, int y)
{
    int first_index = offset2D(0, y, image->nx);

    for (int i = first_index; i < first_index + image->nx; i++) {
        ada_insert(int, image->image, 0, (size_t)i);
    }
    image->ny++;
}

void add_col(Image *image, int x)
{
    for (int i = 0; i < image->ny; i++) {
        ada_insert(int, image->image, 0, (size_t)(i * image->nx + x + i + 1));
    }
    image->nx++;
}

void expand_image(Image *expended, Image *src)
{
    for (int y = 0; y < src->ny; y++) {
        if (row_is_empty(src, y)) {
            add_row(expended, y + (expended->ny - src->ny));
        }
    }
    for (int x = 0; x < src->nx; x++) {
        if (col_is_empty(src, x)) {
            add_col(expended, x + (expended->nx - src->nx));
        }
    }
}

void set_galaxes_array(Image *image)
{
    for (int y = 0; y < image->ny; y++) {
        for (int x = 0; x < image->nx; x++) {
            if (image->image.elements[offset2D(x, y, image->nx)]) {
                ada_appand(Vec2, image->galaxes, vec2_new(x, y));
            }
        }
    }
}

void set_pairs_array(Image *image)
{
    Glax_Pair temp_pair;

    for (size_t i = 0; i < image->galaxes.length; i++) {
        for (size_t j = i + 1; j < image->galaxes.length; j++) {
            temp_pair.galx1 = image->galaxes.elements[i];
            temp_pair.galx2 = image->galaxes.elements[j];
            vec2_sub(&(temp_pair.des), &(temp_pair.galx2), &(temp_pair.galx1));
            temp_pair.path_length = abs(temp_pair.des.x) + abs(temp_pair.des.y);
            ada_appand(Glax_Pair, image->pairs, temp_pair);
        }
    }
}
