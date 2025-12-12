#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./Almog_String_Manipulation.h"

int main(void)
{
    // FILE *fp = fopen("./test_input1.txt", "rt");
    // FILE *fp = fopen("./test_input.txt", "rt");
    FILE *fp = fopen("./input.txt", "rt");

    char line[ASM_MAX_LEN_LINE], the_num[ASM_MAX_LEN_LINE]; 

    int current_dial = 50; 
    int count = 0;
    int max_lim = 99;
    int min_lim = 0;

    while (asm_get_line(fp, line) != -1) {
        int sign = 0;
        if (line[0] == 'L') sign = -1; 
        if (line[0] == 'R') sign = 1; 
        
        if (sign == 0) continue;

        {
            int i = 1, j = 0;
            while (isdigit(line[i])) {
                the_num[j++] = line[i++];
            }
            the_num[j] = '\0';
        }
        
        int steps = atoi(the_num);
        
        for (int k = 0; k < steps; k++) {
            if (sign == 1) {
                current_dial++;
                if (current_dial > max_lim) current_dial = min_lim;
            } else {
                current_dial--;
                if (current_dial < 0) current_dial = max_lim;
            }

            if (current_dial == 0) {
                count++;
            }
        }
    }

    asm_dprintINT(count);

    return 0;
}