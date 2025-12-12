#include <stdio.h>
#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./Almog_String_Manipulation.h"

int main(void)
{
    FILE *fp = fopen("./test_input.txt", "rt");

    char line[ASM_MAX_LEN_LINE]; 

    asm_get_line(fp, line);

    asm_dprintSTRING(line);


    return 0;
}
