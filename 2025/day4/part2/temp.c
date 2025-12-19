#define ASM_MAX_LEN (int)1e5

#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./Almog_String_Manipulation.h"

int offset_2d(int i, int j, int nj)
{
    return i * nj + j;
}

bool accessible(char *map, int i, int j, int ni, int nj)
{
    int count = 0;
    for (int index = -1; index <= 1; index++) {
        for (int jndex = -1; jndex <= 1; jndex++) {
            if (index == 0 && jndex == 0) continue;
            int y = i + index;
            int x = j + jndex;
            if (x < 0 || x >= nj || y < 0 || y >= ni) continue;
            char current = map[offset_2d(y, x, nj)];
            if (current == '@') count++;
        }
    }

    return count < 4;
}

int remove_rolls_of_paper(char *map, int ni, int nj) {
    int count = 0;
    for (int index = 0; index < ni; index++) {
        for (int jndex = 0; jndex < nj; jndex++) {
            if (map[offset_2d(index, jndex, nj)] == '@') {
                if (accessible(map, index, jndex, ni, nj)) {
                    count++;
                    map[offset_2d(index, jndex, nj)] = 'x';
                }
            } 
        }
    }

    return count;
}

int main(void)
{
    // FILE *fp = fopen("./test_input.txt", "rt");
    FILE *fp = fopen("./input.txt", "rt");

    char line[ASM_MAX_LEN] = {'\0'};
    char map[ASM_MAX_LEN] = {'\0'};
    int stride = 0;
    while (asm_get_line(fp, line) != -1) {
        stride = asm_strncat(map, line, 0);
    }
    int nj = stride;
    int ni = (int)asm_length(map) / stride;
    asm_dprintINT(ni);
    asm_dprintINT(nj);

    int count = 0;
    for (int i = 0; i < 1000; i++) {
        count += remove_rolls_of_paper(map, ni, nj);
    }

    asm_dprintINT(count);

    return 0;
}

