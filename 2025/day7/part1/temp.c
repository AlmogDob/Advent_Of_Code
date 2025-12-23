#define ASM_MAX_LEN (int)1e5
#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./Almog_String_Manipulation.h"
#include "./Almog_Dynamic_Array.h"

int offset2d(int i, int j, int nj)
{
    return i * nj + j;
}

void print_diagram(char *diagram, int ni, int nj)
{
    for (int i = 0; i < ni; i++) {
        for (int j = 0; j < nj; j++) {
            printf("%c", diagram[offset2d(i, j, nj)]);
        }
        printf("\n");
    }
    printf("\n");
}

void update_row(char *diagram, int i, int ni, int nj) {
    if (i == 0 || i > ni-1) {
        return;
    }
    for (int j = 0; j < nj; j++) {
        if (diagram[offset2d(i-1, j, nj)] == 'S') {
            diagram[offset2d(i, j, nj)] = '|';
        }
        if (diagram[offset2d(i-1, j, nj)] == '|' && diagram[offset2d(i, j, nj)] != '^') {
            diagram[offset2d(i, j, nj)] = '|';
        }
        if ((diagram[offset2d(i, j, nj)] == '^') && (diagram[offset2d(i-1, j, nj)] == '|')) {
            diagram[offset2d(i, j-1, nj)] = '|';
            diagram[offset2d(i, j+1, nj)] = '|';
        }
    }
}

int count_splits(char *diagram, int ni, int nj)
{
    int count = 0;
    for (int i = 0; i < ni; i++) {
        for (int j = 0; j < nj; j++) {
            if (diagram[offset2d(i, j, nj)] == '^' && diagram[offset2d(i-1, j, nj)] == '|') {
                count++;
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
    char diagram[ASM_MAX_LEN] = {'\0'};

    int ni = 0;
    while ((asm_get_line(fp, line)) != -1) {
        asm_strncat(diagram, line, ASM_MAX_LEN);
        ni++;
    }
    int nj = asm_length(diagram) / ni;

    for (int i = 0; i < ni; i++) {
        update_row(diagram, i, ni, nj);
    }

    asm_dprintINT(count_splits(diagram, ni, nj));


    return 0;
}

