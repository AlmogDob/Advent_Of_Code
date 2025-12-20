#define ASM_MAX_LEN (int)1e5

#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./Almog_String_Manipulation.h"

struct range {
    size_t upper;
    size_t lower;
};

bool check_valid_ID(struct range *list, size_t ID)
{
    for (int i = 0; list[i].lower != 0; i++) {
        if (ID >= list[i].lower && ID <= list[i].upper) {
            return true;
        }
    }

    return false;
}

int main(void)
{
    // FILE *fp = fopen("./test_input.txt", "rt");
    FILE *fp = fopen("./input.txt", "rt");

    char line[ASM_MAX_LEN] = {'\0'};
    char word[ASM_MAX_LEN] = {'\0'};
    struct range list[ASM_MAX_LEN] = {0};
    size_t IDs[ASM_MAX_LEN] = {0};

    int len = 0, i = 0;
    bool len_was_zero = false;

    int count = 0;
    while ((len = asm_get_line(fp, line)) != -1) {
        if (len == 0) {
            len_was_zero = true;
        }

        if (!len_was_zero) {
            asm_get_word_and_cut(word, line, '-', 0);
            list[i].lower = asm_str2size_t(word, NULL, 10);
            list[i].upper = asm_str2size_t(line, NULL, 10);
            i++;
        }

        if (len_was_zero) {

            if (check_valid_ID(list, asm_str2size_t(line, NULL, 10))) {
                count++;
            };
        }
    }

    asm_dprintINT(count);


    return 0;
}

