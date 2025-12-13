#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./Almog_String_Manipulation.h"

#include <math.h>

bool ID_made_from_str(char *ID, char *str)
{
    size_t ID_len = asm_length(ID);
    size_t str_len = asm_length(str);

    if (ID_len % str_len != 0) return false;
    for (size_t i = 0; i < ID_len; i += str_len) {
        char win[ASM_MAX_LEN_LINE];
        asm_copy_array_by_indesies(win , i, i+str_len, ID);
        if (!asm_strncmp(win, str, str_len)) return false; 
    }
    return true;
}

bool check_valid_ID(char *ID) {
    size_t len = asm_length(ID);

    bool valid = true;
    for (size_t win_size = 1; win_size <= len/2; win_size++) {
        for (int i = 0; i <= len-win_size; i += win_size) {
            char win[ASM_MAX_LEN_LINE];
            asm_copy_array_by_indesies(win , i, i+win_size, ID);
            if (ID_made_from_str(ID, win)) valid = false;
        }
    }
    return valid;
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

        if (check_valid_ID(start_str) == 0) sum += strtoull(start_str, NULL, 10);
        size_t diff = calc_diff_IDs(start_str, end_str);
        for (size_t i = 0; i < diff-1; i++) {
            increment_ID(temp);
            if (check_valid_ID(temp) == 0) sum += strtoull(temp, NULL, 10);
        }
        if (check_valid_ID(end_str) == 0) sum += strtoull(end_str, NULL, 10);
    }

    asm_dprintSIZE_T(sum);


    return 0;
}

