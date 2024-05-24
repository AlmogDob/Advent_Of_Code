#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN_LINE (int)1e3
#define MAXDIR 100
#define dprintSTRING(expr) printf(#expr " = %s\n", expr)
#define dprintCHAR(expr) printf(#expr " = %c\n", expr)
#define dprintINT(expr) printf(#expr " = %d\n", expr)
#define dprintUINT(expr) printf(#expr " = %u\n", expr)

int get_line(FILE *fp, char *dst);
int length(char *str);
int get_word_and_cut(char *dst, char *src);
int get_next_word_from_line(char *dst, char *src);
void copy_arry_by_indesies(char *target, int start, int end, char *src);
void get_number_of_numbers(int *number_of_winnig_numbers, int *number_of_my_numbers, char *src);
int get_number_of_matches(int *winnig_numbers,int number_of_winnig_numbers, int *my_numbers, int number_of_my_numbers);

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
    
    char current_line[MAX_LEN_LINE], current_word[MAX_LEN_LINE];
    int number_of_winnig_numbers = 0, number_of_my_numbers = 0,
    current_line_len, success = 1, *winnig_numbers, *my_numbers,
    current_number = 0, number_index = 0, number_of_matches = 0,
    sum_of_worth = 0;
    unsigned int current_worth = 1;

    while ((current_line_len = get_line(fp, current_line)) != -1) {

        get_number_of_numbers(&number_of_winnig_numbers, &number_of_my_numbers, current_line);

        winnig_numbers = (int *)malloc(sizeof(int) * number_of_winnig_numbers);
        my_numbers = (int *)malloc(sizeof(int) * number_of_my_numbers);

        while (success) {
            success = get_word_and_cut(current_word, current_line);

            if (!strcmp(current_word, ":")) {
                while (strcmp(current_word, "|")) {
                    success = get_word_and_cut(current_word, current_line);
                    current_number = atoi(current_word);

                    if(current_number) {
                        winnig_numbers[number_index++] = current_number;
                    }
                }
                number_index = 0;
                
                while (success) {
                    success = get_word_and_cut(current_word, current_line);
                    current_number = atoi(current_word);

                    if(current_number) {
                        my_numbers[number_index++] = current_number;
                    }
                }
                number_index = 0;
            }
        }

        number_of_matches = get_number_of_matches(winnig_numbers, number_of_winnig_numbers, my_numbers, number_of_my_numbers);
        current_worth = number_of_matches? current_worth<<(number_of_matches-1) : 0;
        sum_of_worth += current_worth;
    
        free(my_numbers);
        number_of_my_numbers = 0;
        free(winnig_numbers);
        number_of_winnig_numbers = 0;

        number_of_matches = 0;
        success = 1;
        current_worth = 1;

    }
    dprintINT(sum_of_worth);    

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
            return -1;
        }
    }
    dst[i] = '\0';
    if (i == 0) {
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
    if (src[i] == '\0') {
        return -1;
    }
    return i;

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

void copy_arry_by_indesies(char *target, int start, int end, char *src)
{
    int j = 0;
    for (int i = start; i < end; i++) {
        target[j] = src[i];
        j++;
    }
    target[j] = '\0';
}

void get_number_of_numbers(int *number_of_winnig_numbers, int *number_of_my_numbers, char *src)
{
    char copy[MAX_LEN_LINE], current_word[MAX_LEN_LINE];
    int success = 1;

    strcpy(copy, src);
    while (success) {
        success = get_word_and_cut(current_word, copy);
        if (!strcmp(current_word, ":")) {
            while (strcmp(current_word, "|") && success) {
                (*number_of_winnig_numbers)++;
                success = get_word_and_cut(current_word, copy);
            }
            while (success) {
                (*number_of_my_numbers)++;
                success = get_word_and_cut(current_word, copy);
            }
        }
    }
    *number_of_winnig_numbers -= 1;
}

int get_number_of_matches(int *winnig_numbers,int number_of_winnig_numbers, int *my_numbers, int number_of_my_numbers)
{
    int result = 0;

    for (int i = 0; i < number_of_my_numbers; i++) {
        for (int j = 0; j < number_of_winnig_numbers; j++) {
            if (*(my_numbers+i) == *(winnig_numbers+j)) {
                result++;
            }
        }
    }

    return result;
}
