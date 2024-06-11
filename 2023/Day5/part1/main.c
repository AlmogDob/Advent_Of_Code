#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN_LINE (int)1e3
#define MAX_NUM_OF_NUMBERS (int)1e1
#define MAXDIR 100
#define dprintSTRING(expr) printf(#expr " = %s\n", expr)
#define dprintCHAR(expr) printf(#expr " = %c\n", expr)
#define dprintINT(expr) printf(#expr " = %d\n", expr)
#define dprintL(expr) printf(#expr " = %ld\n", expr)
#define dprintUINT(expr) printf(#expr " = %u\n", expr)

int get_line(FILE *fp, char *dst);
int length(char *str);
int get_next_word_from_line(char *dst, char *src);
void copy_arry_by_indesies(char *target, int start, int end, char *src);
int get_word_and_cut(char *dst, char *src);

int main(int argc, char const *argv[])
{
    char input_dir[MAXDIR];
    FILE *fp;

    if (--argc != 1) {
        fprintf(stderr, "ERROR: not right usage\n");
        return -1;
    }
    strncpy(input_dir, (*(++argv)), MAXDIR);

    fp = fopen(input_dir, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: couldn't find file path\n");
        return -1;
    }

/*------------------------------------------------------------*/

    int current_line_len, i;

    long seed_numbers[MAX_NUM_OF_NUMBERS] = {0},
    soil_numbers[MAX_NUM_OF_NUMBERS] = {0},
    destination_range_start, source_range_start, range_length,
    current_number;

    char current_line[MAX_LEN_LINE], current_word[MAX_LEN_LINE];

    while((current_line_len = get_line(fp, current_line)) != -1) {
        get_word_and_cut(current_word, current_line);
        if (!strcmp(current_word, "seeds")) {
            get_word_and_cut(current_word, current_line);
            i = 0;
            while (get_word_and_cut(current_word, current_line)) {
                if (isdigit(current_word[0])) {
                    seed_numbers[i++] = atol(current_word);
                }
            }
        }
        if (!strcmp(current_word, "seed-to-soil")) {
            get_line(fp, current_line);
            while (strcmp(current_line, "")) {
                get_word_and_cut(current_word, current_line);
                destination_range_start = atol(current_word);
                get_word_and_cut(current_word, current_line);
                source_range_start = atol(current_word);
                get_word_and_cut(current_word, current_line);
                range_length = atol(current_word);
                dprintL(destination_range_start);
                dprintL(source_range_start);
                dprintL(range_length);
                for(i = 0; i < MAX_NUM_OF_NUMBERS; i++) {
                    current_number = seed_numbers[i];
                    if (current_number >= destination_range_start &&
                        current_number < destination_range_start + range_length) {
                            soil_numbers[i] = (destination_range_start - source_range_start) + current_number;
                        }
                    else {
                        soil_numbers[i] = current_number;
                    }
                }

                get_line(fp, current_line);
            }
        }

    }

    for (int i = 0; i < MAX_NUM_OF_NUMBERS; i++) {
        dprintL(seed_numbers[i]);
        dprintL(soil_numbers[i]);
    }
    
    return 0;
}

int get_line(FILE *fp, char *dst)
{
    int i = 0;
    char c;

    while ((c = fgetc(fp)) != '\n' && c != EOF) {
        dst[i] = c;
        i++;
        if (i >= MAX_LEN_LINE) {
            fprintf(stderr, "ERROR: line too long\n");
            exit(1);
        }
    }
    dst[i] = '\0';
    if (c == EOF) {
        return -1;
    }
    return i;
}

int length(char *str)
{
    char c;
    int i = 0;

    while ((c = str[i]) != '\0') {
        i++;
    }
    return i++;
}

int get_next_word_from_line(char *dst, char *src)
{
    int i = 0, j = 0;
    char c;

    while (isspace((c = src[i]))) {
        i++;
    }

    while ((c = src[i]) != ' ' &&
                      c != ';' &&
                      c != ':' &&
                      c != ',' &&
                      c != '\n'&&
                      c != '\0') {
                        dst[j] = src[i];
                        i++;
                        j++;
    }

    if ((c == ' ' ||
        c == ';' ||
        c == ':' ||
        c == ',' ||
        c == '\n'||
        c == '\0') && i == 0) {
            dst[j++] = c;
            i++;
    }

    dst[j] = '\0';

    /*test*/
    // dprintCHAR(src[i]);
    /*test*/
    if (j == 0) {
        return -1;
    }
    return i;

}

void copy_arry_by_indesies(char *target, int start, int end, char *src)
{
    int j = 0;
    for (int i = start; i < end; i++) {
        target[j] = src[i];
        j++;
    }
    target[j] = '\0';
}

int get_word_and_cut(char *dst, char *src)
{
    int last_pos;

    if (src[0] == '\0') {
        return 0;
    }
    last_pos = get_next_word_from_line(dst, src);
    if (last_pos == -1) {
        return 0;
    }
    /*test*/
    // dprintSTRING(src);
    // dprintINT(last_pos);
    /*test*/
    copy_arry_by_indesies(src, last_pos, length(src), src);
    return 1;
}

