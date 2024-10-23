#include <string.h>
#define ALMOG_AOC_IMPLEMENTATION
#include "Almog_AoC.h"
#include <stdbool.h>

int main(int argc, char const *argv[])
{
    char input_dir[MAXDIR];
    FILE *fp;

    if (--argc != 1) {
        fprintf(stderr, "%s:%d: [ERROR]: not right usage\n", __FILE__, __LINE__);
        return -1;
    }
    strncpy(input_dir, (*(++argv)), MAXDIR);

    fp = fopen(input_dir, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: couldn't find file path\n");
        return -1;
    }

/*------------------------------------------------------------*/
    int line_len;
    char current_line[MAX_LEN_LINE];

    while ((line_len = aaoc_get_line(fp, current_line)) != -1) {
        dprintSTRING(current_line);
    }


    return 0;
}

