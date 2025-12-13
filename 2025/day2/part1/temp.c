#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./Almog_String_Manipulation.h"

#include <math.h>

bool check_valid_ID(char *ID) {
    if (ID[0] == 0) return false;
    int len = asm_length(ID);
    if (len%2 == 1) return true;
    int window_size = len/2;
    char left[ASM_MAX_LEN_LINE], right[ASM_MAX_LEN_LINE];

    asm_copy_array_by_indesies(left , 0, window_size, ID);
    asm_copy_array_by_indesies(right, window_size, len+1, ID);

    return !asm_strncmp(left, right, len/2);
}

void increment_digit(char *ID, size_t index) {
    assert(asm_length(ID) >= index);

    if (index == 0) {
        if (ID[index] != '9') {
            ID[index]++;
        } else {
            ID[index] = '0';
            asm_left_pad(ID, 1);
            ID[0] = '1';
        }
    } else {
        if (ID[index] != '9') {
            ID[index]++;
        } else {
            ID[index] = '0';
            increment_digit(ID, index-1);
        }
    }
}

void increment_ID(char *ID) {
    increment_digit(ID, asm_length(ID)-1);
}

size_t calc_diff_IDs(char *ID1, char *ID2)
{
    int difference_index = 0;
    for (int i = 0; i < (int)asm_min(asm_length(ID1), asm_length(ID2)); i++) {
        if (ID1[i] != ID2[i]) {
            difference_index = i;
            break;
        }
    }

    char diff_ID1_str[ASM_MAX_LEN_LINE], diff_ID2_str[ASM_MAX_LEN_LINE];
    asm_copy_array_by_indesies(diff_ID1_str, difference_index, asm_length(ID1), ID1);
    asm_copy_array_by_indesies(diff_ID2_str, difference_index, asm_length(ID2), ID2);

    int diff_ID1_num = atoi(diff_ID1_str);
    int diff_ID2_num = atoi(diff_ID2_str);

    return diff_ID2_num - diff_ID1_num;
}

size_t get_half_ID(char *ID) {
    assert(ID[0] != 0);
    size_t len = asm_length(ID);
    assert(len%2 == 0);

    char half[ASM_MAX_LEN_LINE];
    asm_copy_array_by_indesies(half , 0, len/2, ID);

    return (size_t)atoi(half);
}

#define print_ID(ID) printf("%s: %d\n", ID, check_valid_ID(ID))

int main(void)
{
    // FILE *fp = fopen("./test_input.txt", "rt");
    FILE *fp = fopen("./input.txt", "rt");

    char line[ASM_MAX_LEN_LINE], range_str[ASM_MAX_LEN_LINE];
    char start_str[ASM_MAX_LEN_LINE], end_str[ASM_MAX_LEN_LINE], temp[ASM_MAX_LEN_LINE]; 
    asm_get_line(fp, line);

    asm_dprintSTRING(line);
    printf("\n");

    size_t sum = 0;

    while (asm_get_word_and_cut(range_str, line, ',', 0)) {
        asm_copy_array_by_indesies(end_str, 0, ASM_MAX_LEN_LINE, range_str);
        asm_get_word_and_cut(start_str, end_str, '-', 0);
        asm_copy_array_by_indesies(temp, 0, asm_length(start_str), start_str);

        // print_ID(start_str);
        // print_ID(end_str);
        // printf("\n");

        if (check_valid_ID(start_str) == 0) sum += get_half_ID(start_str) * (size_t)pow(10, asm_length(start_str)/2) + get_half_ID(start_str);
        size_t diff = calc_diff_IDs(start_str, end_str);
        for (size_t i = 0; i < diff-1; i++) {
            increment_ID(temp);
            if (check_valid_ID(temp) == 0) sum += get_half_ID(temp) * (size_t)pow(10, asm_length(temp)/2) + get_half_ID(temp);
        }
        if (check_valid_ID(end_str) == 0) sum += get_half_ID(end_str) * (size_t)pow(10, asm_length(end_str)/2) + get_half_ID(end_str);
    }

    asm_dprintSIZE_T(sum);


    return 0;
}

