#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./Almog_String_Manipulation.h"

size_t get_highest_jolts(char *s) 
{
    size_t len = asm_length(s);

    size_t highest_jolts = 0;
    char window[3]; window[2] = '\0';
    for (size_t i = 0; i < len-1; i++) {
        for (size_t j = i+1; j < len; j++) {
            window[0] = s[i];
            window[1] = s[j];
            size_t current = atoi(window);
            if (highest_jolts < current) {
                highest_jolts = current;
            }
        }
    }

    return highest_jolts;
}

int main(void)
{
    // FILE *fp = fopen("./test_input.txt", "rt");
    FILE *fp = fopen("./input.txt", "rt");

    char line[ASM_MAX_LEN_LINE];

    int sum = 0;
    while (asm_get_line(fp, line) != -1) {
        // asm_dprintSTRING(line);
        sum += get_highest_jolts(line);
    }

    asm_dprintINT(sum);
    

    return 0;
}

