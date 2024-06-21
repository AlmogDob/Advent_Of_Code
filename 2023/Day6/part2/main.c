#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <inttypes.h>

#define MAXDIR 100
#define MAX_LEN_LINE (int)1e3
#define NUM_OF_RACES (int)1e1
#define NUM_OF_DISTANCES (int)5e5
#define dprintSTRING(expr) printf(#expr " = %s\n", expr)
#define dprintCHAR(expr) printf(#expr " = %c\n", expr)
#define dprintINT(expr) printf(#expr " = %d\n", expr)
#define dprintSIZET(expr) printf(#expr " = %zu\n", expr)

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

    char current_line[MAX_LEN_LINE], current_word[MAX_LEN_LINE], current_num[MAX_LEN_LINE] = {'\0'};

    int current_line_len, success = 1, i = 0, num_of_success = 0,
    mul_of_success = 1;

    size_t time, distances[NUM_OF_DISTANCES] = {0}, current_max_dist;

    while ((current_line_len = get_line(fp, current_line)) != -1) {
        get_word_and_cut(current_word, current_line);
        if (!strcmp(current_word, "Time")) {
            while ((success = get_word_and_cut(current_word, current_line))) {
                if (isdigit(current_word[0])) {
                    strcat(current_num, current_word);
                    sscanf(current_num, "%zu", &time);
                    dprintSIZET(time);
                }
            }
            success = 1;
        }
        i = 0;
        current_num[0] = '\0';
        if (!strcmp(current_word, "Distance")) {
            while ((success = get_word_and_cut(current_word, current_line))) {
                if (isdigit(current_word[0])) {
                    strcat(current_num, current_word);
                    sscanf(current_num, "%zu", &current_max_dist);
                    dprintSIZET(current_max_dist);
                }
            }
            success = 1;
        }
    }

    printf("-------------------------\n");
    printf("hi\n");
    for (size_t j = 1; j < time; j++) {
        if (j*(time - j) > current_max_dist) {
            num_of_success += 1;
        }
    }
    
    dprintINT(num_of_success);

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

