#define NUM_OF_ROWS 5
#define ASM_MAX_LEN (int)1e4 * NUM_OF_ROWS
#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./Almog_String_Manipulation.h"
#include "./Almog_Dynamic_Array.h"


int offset2d(int i, int j, int nj)
{
    return i * nj + j;
}

bool column_is_empty(char *equations, int j, int nj)
{
    for (int i = 0; i < NUM_OF_ROWS; i++) {
        if (equations[offset2d(i, j, nj)] != ' ') {
            return false;
        }
    }

    return true;
}

size_t calc_problem(char *equations, int previous_jndex, int current_jndex, int nj)
{
    size_t ans = 0;
    if (equations[offset2d(NUM_OF_ROWS-1, previous_jndex, nj)] == '*') {
        ans = 1;
    }
    for (int j = previous_jndex; j < current_jndex; j++) {
        char word[NUM_OF_ROWS] = {'\0'};
        for (int i = 0; i < NUM_OF_ROWS-1; i++) {
            word[i] = equations[offset2d(i, j, nj)];
        }
        if (equations[offset2d(NUM_OF_ROWS-1, previous_jndex, nj)] == '*') {
            ans *= asm_str2size_t(word, NULL, 10);
        } else {
            ans += asm_str2size_t(word, NULL, 10);
        }
    }

    return ans;
}

int main(void)
{
    // FILE *fp = fopen("./test_input.txt", "rt");
    FILE *fp = fopen("./input.txt", "rt");

    char line[ASM_MAX_LEN] = {'\0'};
    char equations[ASM_MAX_LEN] = {'\0'};

    while ((asm_get_line(fp, line)) != -1) {
        asm_strncat(equations, line, ASM_MAX_LEN);
    }

    int ni = NUM_OF_ROWS; 
    int nj = asm_length(equations) / ni;

    size_t sum = 0;

    for (int current_jndex = 0, previous_jndex = 0; current_jndex < nj; current_jndex++) {
        size_t ans = 0;
        if (column_is_empty(equations, current_jndex, nj)) {
            ans = calc_problem(equations, previous_jndex, current_jndex, nj);
            previous_jndex = ++current_jndex;
        } else if (current_jndex == nj -1) {
            ans = calc_problem(equations, previous_jndex, ++current_jndex, nj);
        }
        sum += ans;
    }
    
    asm_dprintSIZE_T(sum);

    return 0;
}

