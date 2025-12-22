#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./Almog_String_Manipulation.h"
#include "./Almog_Dynamic_Array.h"

int main(void)
{
    FILE *fp = fopen("./test_input.txt", "rt");
    // FILE *fp = fopen("./input.txt", "rt");

    char line[ASM_MAX_LEN] = {'\0'};

    while ((asm_get_line(fp, line)) != -1) {
        asm_dprintSTRING(line);
    }

    return 0;
}

