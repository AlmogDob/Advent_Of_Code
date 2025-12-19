#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./Almog_String_Manipulation.h"

#include <assert.h>
#include <stdlib.h>
#include <ctype.h>

size_t get_index_highest_char(char *s)
{
    size_t len = asm_length(s);
    char highest_char = 0;
    size_t index_highest_char = len;
    for (size_t i = 0; i < len; i++) {
        if (s[i] > highest_char) {
            highest_char = s[i];
            index_highest_char = i;
        }
    }

    return index_highest_char;
}

size_t get_highest_jolts(char *s) 
{
    size_t len = asm_length(s);
    if (len < 12) return 0;

    char temp_s[len+1], output_str[12];

    for (int i = 11; i >= 0; i--) {
        len = asm_length(s);
        asm_copy_array_by_indesies(temp_s, 0, len-i, s);
        size_t index = get_index_highest_char(temp_s);
        output_str[11-i] = s[index];
        s += index + 1;
    }

    return strtoull(output_str, NULL, 10);
}

int main(void)
{
    // FILE *fp = fopen("./test_input.txt", "rt");
    FILE *fp = fopen("./input.txt", "rt");

    char line[ASM_MAX_LEN_LINE];

    size_t sum = 0;
    while (asm_get_line(fp, line) != -1) {
        // asm_dprintSTRING(line);
        sum += get_highest_jolts(line);
    }

    asm_dprintSIZE_T(sum);

    

    return 0;
}

