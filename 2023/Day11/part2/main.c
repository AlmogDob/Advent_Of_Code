#include <string.h>
#define ALMOG_AOC_IMPLEMENTATION
#include "Almog_AoC.h"
#define VEC2_IMPLEMENTATION
#include "Vec2.h"
#include <math.h>

typedef struct {
    int value;
    Vec2 pos;
} Tile;

typedef struct {
    size_t length;
    size_t capacity;
    Tile *elements;
} ada_Tile_array;

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
    ada_Tile_array image;
    ada_Vec2_array galaxes;
    ada_Galx_Pair_array pairs;
} Image;

void init_image(Image *image);
void print_image_with_data(Image *image);
void set_tiles_pos(Image *image);
int offset2D(size_t x, size_t y, size_t nx);
int row_is_empty(Image *image, int y);
int col_is_empty(Image *image, int x);
void copy_image_to_image(Image *dst, Image *src);
void set_galaxes_array(Image *image);
void set_pairs_array(Image *image);
void update_col_values(Image *Image, int from_col, int to_col, int update_value);
void update_row_values(Image *Image, int from_row, int to_row, int update_value);
void expand_image(Image *expended, Image *src, int value);

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
    Tile temp_tile;
    Image first_image, current_image;

    init_image(&first_image);
    init_image(&current_image);

    while ((line_len = aaoc_get_line(fp, current_line)) != -1) {
        dprintSTRING(current_line);
        ny++;
        nx = line_len;
        for (int i = 0; i < line_len; i++) {
            if (current_line[i] == '.') {
                temp_tile.value = 0;
                ada_appand(Tile, first_image.image, temp_tile);
            }
            if (current_line[i] == '#') {
                temp_tile.value = 1;
                ada_appand(Tile, first_image.image, temp_tile);
            }
        }
    }

    dprintINT(nx);
    dprintINT(ny);

    first_image.nx = nx;
    first_image.ny = ny;

    set_tiles_pos(&first_image);

    print_image_with_data(&first_image);

    copy_image_to_image(&current_image, &first_image);

    expand_image(&current_image, &first_image, 1e6 - 1);

    set_galaxes_array(&current_image);

    set_pairs_array(&current_image);


    size_t sum = 0;

    for (size_t i = 0; i < current_image.pairs.length; i++) {
        sum += current_image.pairs.elements[i].path_length;
    }

    dprintSIZE_T(sum);


    return 0;
}

void init_image(Image *image)
{
    ada_array(Tile, image->image);
    ada_array(Vec2, image->galaxes);
    ada_array(Glax_Pair, image->pairs);
}

void set_tiles_pos(Image *image)
{
    for (int x = 0; x < image->nx; x++) {
        for (int y = 0; y < image->ny; y++) {
            image->image.elements[offset2D(x, y, image->nx)].pos = vec2_new(x, y);
        }
    }
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
            printf("%d ", image->image.elements[offset2D(x, y, image->nx)].value);
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
        if (image->image.elements[offset2D(x, y, image->nx)].value == 1) {
            return 0;
        }
    }
    return 1;
}

int col_is_empty(Image *image, int x)
{
    for (int y = 0; y < image->ny; y++){
        if (image->image.elements[offset2D(x, y, image->nx)].value == 1) {
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
        ada_appand(Tile, dst->image, src->image.elements[i]);
    }
}

void set_galaxes_array(Image *image)
{
    for (int y = 0; y < image->ny; y++) {
        for (int x = 0; x < image->nx; x++) {
            if (image->image.elements[offset2D(x, y, image->nx)].value) {
                ada_appand(Vec2, image->galaxes, image->image.elements[offset2D(x, y, image->nx)].pos);
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
            temp_pair.path_length = fabsf(temp_pair.des.x) + fabsf(temp_pair.des.y);
            ada_appand(Glax_Pair, image->pairs, temp_pair);
        }
    }
}

void update_col_values(Image *Image, int from_col, int to_col, int update_value)
{
    for (int x = from_col; x < to_col; x++) {
        for (int y = 0; y < Image->ny; y++) {
            Image->image.elements[offset2D(x, y, Image->nx)].pos.x += update_value;
        }
    }
}

void update_row_values(Image *Image, int from_row, int to_row, int update_value)
{
    for (int y = from_row; y < to_row; y++) {
        for (int x = 0; x < Image->nx; x++) {
            Image->image.elements[offset2D(x, y, Image->nx)].pos.y += update_value;
        }
    }
}

void expand_image(Image *expended, Image *src, int value)
{
    for (int y = 0; y < src->ny; y++) {
        dprintINT(y);
        if (row_is_empty(src, y)) {
            update_row_values(expended, y+1, expended->ny, value);
        }
    }
    for (int x = 0; x < src->nx; x++) {
        dprintINT(x);
        if (col_is_empty(src, x)) {
            update_col_values(expended, x+1, expended->nx, value);
        }
    }
}
