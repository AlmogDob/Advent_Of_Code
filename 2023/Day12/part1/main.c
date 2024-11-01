#include <stdlib.h>
#include <string.h>
#define ALMOG_AOC_IMPLEMENTATION
#include "Almog_AoC.h"
#define VEC2_IMPLEMENTATION
#include "Vec2.h"
#include <math.h>

typedef enum {
    OPERATIONAL = '.',
    DAMAGED = '#',
    UNKNOWN = '?'
} Hot_Spring_Type;

typedef struct {
    size_t length;
    size_t capacity;
    int* elements;
} ada_int_array;

typedef struct {
    size_t length;
    size_t capacity;
    char* elements;
    int num_of_unknown;
    int num_of_damage;
} Block;

typedef struct {
    size_t length;
    size_t capacity;
    Block* elements;
} ada_block_array;

typedef struct {
    char record[MAX_LEN_LINE];
    ada_int_array backup;
    ada_block_array blocks;
} Row;

typedef struct {
    size_t length;
    size_t capacity;
    Row* elements;
} ada_Row_array;

size_t choose_operator(int n, int k);
void print_int_array(ada_int_array *array);
void print_Row(Row *row);
void print_Row_array(ada_Row_array *array);
void print_block_array(ada_block_array *array);

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

    int line_len, sum = 1;
    char current_line[MAX_LEN_LINE], current_word[MAX_LEN_LINE], temp_word[MAX_LEN_LINE],
    c;
    Row temp_row;
    Block temp_block;
    ada_Row_array rows;

    ada_create_array(Row, rows);

    while ((line_len = aaoc_get_line(fp, current_line)) != -1) {
        ada_create_array(int, temp_row.backup);
        ada_create_array(Block, temp_row.blocks);

        aaoc_get_word_and_cut(current_word, current_line, ' ');
        strncpy(temp_row.record, current_word, MAX_LEN_LINE);

        while (aaoc_get_word_and_cut(temp_word, current_word, '.')) {
            if (!strcmp(temp_word, ".")) {
                while (!strcmp(temp_word, ".")) {
                    if (!aaoc_get_word_and_cut(temp_word, current_word, '.')) {
                        break;
                    }
                }
            }
            if (!strcmp(temp_word, ".")) {
                break;
            }
            ada_create_array(char, temp_block);
            temp_block.num_of_damage  = 0;
            temp_block.num_of_unknown = 0;
            int i = 0;
            while ((c = temp_word[i]) != '\0') {
                if (c == '?') {
                    temp_block.num_of_unknown++;
                }
                if (c == '#') {
                    temp_block.num_of_damage++;
                }
                ada_appand(char, temp_block, c);
                i++;
            }
            ada_appand(Block, temp_row.blocks, temp_block);
        }

        while (aaoc_get_word_and_cut(current_word, current_line, ',')) {
            ada_appand(int, temp_row.backup, atoi(current_word));
            aaoc_get_word_and_cut(current_word, current_line, ',');
        }

        ada_appand(Row, rows, temp_row);
    }

    print_Row_array(&rows);
    
    int num_of_damaged, len_of_block;

    temp_row = rows.elements[2];
    print_Row(&temp_row);

    size_t j = 0;
    size_t i = 0;


    dprintINT(sum);
    return 0;
}

size_t choose_operator(int n, int k)
{
    if (n < k || n < 0 || k < 0) {
        return 0;
    }

    int ans = 1;

    for (int i = n; i >= n-k+1; i--) {
        ans *= i;
    }
    for (int i = k; i >= 1; i--) {
        ans /= i;
    }

    return (size_t)ans;
}

void print_int_array(ada_int_array *array)
{
    for (size_t i = 0; i < array->length; i++) {
        printf("%d ", array->elements[i]);
    }
    printf("\n");
}

void print_Row(Row *row)
{
    printf("-+-+-+-+-+-+-+-+-+-\n");
    printf("%s\n", row->record);
    print_block_array((&row->blocks));
    print_int_array(&(row->backup));
    printf("-+-+-+-+-+-+-+-+-+-\n");
}

void print_Row_array(ada_Row_array *array)
{
    printf("@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    for (size_t i = 0; i < array->length; i++) {
        print_Row(&(array->elements[i]));
    }
    printf("@@@@@@@@@@@@@@@@@@@@@@@@@\n");
}

void print_block_array(ada_block_array *array)
{
    for (size_t i = 0; i < array->length - 1; i++) {
        for (size_t j = 0; j < array->elements[i].length; j++) {
            printf("%c", array->elements[i].elements[j]);
        }
        printf(" | ");
    }
    for (size_t j = 0; j < array->elements[array->length - 1].length; j++) {
        printf("%c", array->elements[array->length - 1].elements[j]);
    }
    printf("\n");
    for (size_t i = 0; i < array->length - 1; i++) {
        printf("%d, %d", array->elements[i].num_of_unknown, array->elements[i].num_of_damage);
        printf(" | ");
    }

    printf("%d, %d", array->elements[array->length - 1].num_of_unknown, array->elements[array->length - 1].num_of_damage);

    printf("\n");

}
