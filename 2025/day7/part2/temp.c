#define ASM_MAX_LEN (int)1e5
#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./Almog_String_Manipulation.h"
#include "./Almog_Dynamic_Array.h"

int offset2d(int i, int j, int nj)
{
    return i * nj + j;
}

void print_char_diagram(char *diagram, int ni, int nj)
{
    for (int i = 0; i < ni; i++) {
        for (int j = 0; j < nj; j++) {
            printf("%c", diagram[offset2d(i, j, nj)]);
        }
        printf("\n");
    }
    printf("\n");
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

void print_size_t_diagram(size_t *diagram, int ni, int nj)
{
    for (int i = 0; i < ni; i++) {
        for (int j = 0; j < nj; j++) {
            printf("% 13zu ", diagram[offset2d(i, j, nj)]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(void)
{
    // FILE *fp = fopen("./test_input.txt", "rt");
    FILE *fp = fopen("./input.txt", "rt");

    char line[ASM_MAX_LEN] = {'\0'};
    char char_diagram[ASM_MAX_LEN] = {'\0'};
    size_t size_t_diagram[ASM_MAX_LEN];
	asm_memset(size_t_diagram, 0, sizeof(size_t_diagram));

    int ni = 0;
    while ((asm_get_line(fp, line)) != -1) {
        asm_strncat(char_diagram, line, ASM_MAX_LEN);
        ni++;
    }
    int nj = asm_length(char_diagram) / ni;


    for (int j = 0; j < nj; j++) {
        if (char_diagram[offset2d(0, j, nj)] == 'S') {
            size_t_diagram[offset2d(1, j, nj)] = 1;
        }
    }
    for (int i = 1; i < ni; i++) {
        for (int j = 0; j < nj; j++) {
            if (size_t_diagram[offset2d(i-1, j, nj)] != 0 && char_diagram[offset2d(i, j, nj)] == '.') {
                size_t_diagram[offset2d(i, j, nj)] += size_t_diagram[offset2d(i-1, j, nj)];
            }
            if (size_t_diagram[offset2d(i-1, j, nj)] != 0 && char_diagram[offset2d(i, j, nj)] == '^') {
                size_t_diagram[offset2d(i, j-1, nj)] += size_t_diagram[offset2d(i-1, j, nj)];
                size_t_diagram[offset2d(i, j+1, nj)] += size_t_diagram[offset2d(i-1, j, nj)];
            }
        }
    }


    for (int i = 0; i < ni; i++) {
        update_row(char_diagram, i, ni, nj);
    }

    print_char_diagram(char_diagram, ni, nj);
    print_size_t_diagram(size_t_diagram, ni, nj);

    size_t sum = 0;
    for (int j = 0; j < nj; j++) {
        sum += size_t_diagram[offset2d(ni-1, j, nj)];
    }

    asm_dprintSIZE_T(sum);



    return 0;
}

