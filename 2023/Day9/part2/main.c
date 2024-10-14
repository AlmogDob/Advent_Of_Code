#include <string.h>
#define ALMOG_AOC_IMPLEMENTATION
#include "Almog_AoC.h"

typedef struct {
    size_t length;
    size_t capacity;
    int* elements;
} ada_int_array;

typedef struct {
    size_t length;
    size_t capacity;
    ada_int_array* elements;
} ada_array_of_ada_int_array;

typedef struct {
    size_t length;
    size_t capacity;
    ada_array_of_ada_int_array *elements;
} ada_array_of_arrays_of_ada_int_array;

void print_int_array(ada_int_array history);
void print_list_of_int_array(ada_array_of_ada_int_array *hl);
void print_list_of_list_of_int_array(ada_array_of_arrays_of_ada_int_array *l);
ada_int_array get_diff_sequence(ada_int_array a);
int all_zeros(ada_int_array array);
ada_array_of_ada_int_array create_diff_sequences_of_history(ada_int_array history);
void extrapolate_next_value(ada_array_of_arrays_of_ada_int_array *a);
void extrapolate_previous_value(ada_array_of_arrays_of_ada_int_array *a);
void add_last_value_according_to_next_seq(ada_array_of_ada_int_array *a, size_t layar);
void add_first_value_according_to_next_seq(ada_array_of_ada_int_array *a, size_t layar);
int sum_last_values(ada_array_of_arrays_of_ada_int_array *a);
int sum_first_values(ada_array_of_arrays_of_ada_int_array *a);

int main(int argc, char const *argv[])
{
    char input_dir[MAXDIR];
    FILE *fp;

    if (--argc != 1) {
        fprintf(stderr, "ERROR: not right usage\n");
        return -1;
    }
    strncpy(input_dir, (*(++argv)), MAXDIR);

    fp = fopen(input_dir, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: couldn't find file path\n");
        return -1;
    }

/*------------------------------------------------------------*/

    int line_len, current_num; 
    char current_line[MAX_LEN_LINE], current_word[MAX_LEN_LINE];
    ada_int_array current_history;
    ada_array_of_ada_int_array historys, sequences;
    ada_array_of_arrays_of_ada_int_array OASIS_report;

    ada_array(ada_int_array, historys);
    ada_array(ada_int_array, sequences);

    ada_array(ada_array_of_ada_int_array, OASIS_report);

    while ((line_len = aaoc_get_line(fp, current_line)) != -1) {
        ada_array(int, current_history);
        while (aaoc_get_word_and_cut(current_word, current_line) != 0) {
            current_num = atoi(current_word);
            ada_appand(int, current_history, current_num);
        }
        ada_appand(ada_int_array, historys, current_history);
    }

    print_list_of_int_array(&historys);

    for (size_t i = 0; i < historys.length; i++) {
        sequences = create_diff_sequences_of_history(historys.elements[i]);
        ada_appand(ada_array_of_ada_int_array, OASIS_report, sequences);
    }

    extrapolate_previous_value(&OASIS_report);

    print_list_of_list_of_int_array(&OASIS_report);
    
    dprintINT(sum_first_values(&OASIS_report));

    return 0;
}

void print_int_array(ada_int_array history)
{
    for (size_t i = 0; i < history.length; i++) {
        printf("%d ", history.elements[i]);
    }
    printf("\n");
}

void print_list_of_int_array(ada_array_of_ada_int_array *hl)
{
    printf("-----------------------------\n");
    for (size_t i = 0; i < hl->length; i++) {
        printf("---------------\n");
        printf("%zu:\n", i+1);
        print_int_array(hl->elements[i]);
        printf("---------------\n");
    }
    printf("-----------------------------\n");
    printf("\n");
}

void print_list_of_list_of_int_array(ada_array_of_arrays_of_ada_int_array *l)
{
    printf("#########################################\n");
    for (size_t i = 0; i < l->length; i++) {
        printf("    ~%zu~    \n", i+1);
        print_list_of_int_array(&(l->elements[i]));
    }
    printf("#########################################\n");
}

ada_int_array get_diff_sequence(ada_int_array a)
{
    ada_int_array diff_seq;
    ada_array(int, diff_seq);

    for (size_t i = 0; i < a.length-1; i++) {
        ada_appand(int, diff_seq, a.elements[i+1] - a.elements[i]);
    }

    return diff_seq;
}

int all_zeros(ada_int_array array)
{
    for (size_t i = 0; i < array.length; i++) {
        if (array.elements[i] != 0) {
            return 0;
        }
    }
    return 1;
}

ada_array_of_ada_int_array create_diff_sequences_of_history(ada_int_array history)
{
    ada_array_of_ada_int_array sequences;
    ada_array(ada_int_array, sequences);

    ada_appand(ada_int_array, sequences, history);
    int i = 0;
    while (!all_zeros(sequences.elements[i])) {
        ada_appand(ada_int_array, sequences, get_diff_sequence(sequences.elements[i]));
        i++;
    }
    return sequences;
}

void extrapolate_next_value(ada_array_of_arrays_of_ada_int_array *a)
{
    for (size_t i = 0; i < a->length; i++) {
        ada_appand(int, a->elements[i].elements[a->elements[i].length-1], 0);
        for (int j = (int)a->elements[i].length - 2; j >= 0; j--) {
            add_last_value_according_to_next_seq(&(a->elements[i]), j);
        }
    }
}

void extrapolate_previous_value(ada_array_of_arrays_of_ada_int_array *a)
{
    for (int i = 0; i < (int)a->length; i++) {
        ada_insert(int, a->elements[i].elements[a->elements[i].length-1], 0, 0);
        for (int j = (int)a->elements[i].length - 2; j >= 0; j--) {
            add_first_value_according_to_next_seq(&(a->elements[i]), j);
        }
    }

}

void add_last_value_according_to_next_seq(ada_array_of_ada_int_array *a, size_t layar)
{
    int next_layar_last_value = a->elements[layar+1].elements[a->elements[layar+1].length-1];
    int current_layar_last_value = a->elements[layar].elements[a->elements[layar].length-1];

    ada_appand(int, a->elements[layar], current_layar_last_value + next_layar_last_value);
}

void add_first_value_according_to_next_seq(ada_array_of_ada_int_array *a, size_t layar)
{
    int current_layar_first_value = a->elements[layar].elements[0];
    int next_layar_first_value = a->elements[layar+1].elements[0];

    ada_insert(int, a->elements[layar], current_layar_first_value - next_layar_first_value, 0);
}

int sum_last_values(ada_array_of_arrays_of_ada_int_array *a)
{
    int sum = 0;

    for (size_t i = 0; i < a->length; i++) {
        sum += a->elements[i].elements[0].elements[a->elements[i].elements[0].length-1];
    }
    return sum;
}

int sum_first_values(ada_array_of_arrays_of_ada_int_array *a)
{
    int sum = 0;

    for (size_t i = 0; i < a->length; i++) {
        sum += a->elements[i].elements[0].elements[0];
    }
    return sum;

}
