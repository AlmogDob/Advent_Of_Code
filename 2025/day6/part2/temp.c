#define ASM_MAX_LEN (int)1e4
#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./Almog_String_Manipulation.h"
#include "./Almog_Dynamic_Array.h"

#define MAX_LEN_WORD 50

typedef char Word[MAX_LEN_WORD];

int offset2d(int i, int j, int nj)
{
    return i * nj + j;
}

int main(void)
{
    // FILE *fp = fopen("./test_input.txt", "rt");
    FILE *fp = fopen("./input.txt", "rt");

    char line[ASM_MAX_LEN] = {'\0'};
    char next_word[ASM_MAX_LEN] = {'\0'};
    Word equations[ASM_MAX_LEN * 4] = {'\0'};

    int i = 0, stride = 0;
    while ((asm_get_line(fp, line)) != -1) {
        stride = 0;
        while (asm_get_word_and_cut(next_word, line, ' ', 0)) {
            asm_strncat(equations[i++], next_word, MAX_LEN_WORD);
            stride++;
        }
    }
    int num_of_eq = i;
    int nj = stride;
    int ni = num_of_eq / nj;

    // for (int j = 0; j < num_of_eq; j++) {
    //     asm_dprintSTRING(equations[j]);
    // }
    asm_dprintINT(ni);
    asm_dprintINT(nj);
    size_t sum = 0;
    for (int j = 0; j < nj; j++) {
        size_t ans = 0;
        if (equations[offset2d(4, j, nj)][0] == '*') {
            ans = 1;
            for (int i = 0; i < 4; i++) {
                ans *= asm_str2size_t(equations[offset2d(i, j, nj)], NULL, 10);
            }
        } else if (equations[offset2d(4, j, nj)][0] == '+') {
            ans = 0;
            for (int i = 0; i < 4; i++) {
                ans += asm_str2size_t(equations[offset2d(i, j, nj)], NULL, 10);
            }
        }
        sum += ans;
    }

    asm_dprintSIZE_T(sum);

    return 0;
}

