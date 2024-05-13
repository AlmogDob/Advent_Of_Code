#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXDIR 100
#define MAXLINE 100
#define MAX_DIGIT_LEN 5
#define NUM_OF_DIGIT 9
#define dprintSTRING(expr) printf(#expr " = %s\n", expr)
#define dprintCHAR(expr) printf(#expr " = %c\n", expr)
#define dprintINT(expr) printf(#expr " = %d\n", expr)

int get_calibration_values(char *str);
int get_line(FILE *fp, char *dst);
int is_digit_as_word(char *str);
int length(char *str);
void copy_arry_by_indesies(char *target, int start, int end, char *src);

char *digits[9];

int main(int argc, char const *argv[])
{
    for (int i = 0; i < NUM_OF_DIGIT; i++) {
        digits[i] = malloc(sizeof(char) * (MAX_DIGIT_LEN+1));
    }

    digits[0] = "one";
    digits[1] = "two";
    digits[2] = "three";
    digits[3] = "four";
    digits[4] = "five";
    digits[5] = "six";
    digits[6] = "seven";
    digits[7] = "eight";
    digits[8] = "nine";

    char input_dir[MAXDIR];
    FILE *fp;
    int len, ans, sum = 0;
    char str[MAXLINE];

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

    while ((len = get_line(fp, str)) != -1) {
        ans = get_calibration_values(str);
        /*test*/
        // printf("line: '%s'\nans = %d\n", str, ans);
        /*test*/
        sum += ans;
    }
    printf("sum = %d\n", sum);

    return 0;
}

int get_calibration_values(char *str)
{
    char first = '\0';  /* is ampty*/
    char last = '\0';
    char res[3];
    char c, str_to_test[MAXLINE];
    int i = 0, ans, start = 0, end = 0, last_start;

    while ((c = str[i]) != '\0') {
        end++;
        if (isdigit(c) ) {
            if (first == '\0') {
                first = str[i];
            } else {
                last = str[i];
            }
        }

        if (end - start >= MAX_DIGIT_LEN || str[end+1] == '\0') {
            last_start = start;
            while (end - start >= 1) {
                copy_arry_by_indesies(str_to_test, start, end, str);
                if (is_digit_as_word(str_to_test)) {
                    if (first == '\0') {
                        first = '0' + is_digit_as_word(str_to_test);
                    } else {
                        last = '0' + is_digit_as_word(str_to_test);
                    }
                }
                // printf("str_to_test: %s,\tstart: %d,\tend:%d\n", str_to_test, start, end);
                start++;    
            }
            start = last_start;
        }
        if (end - start >= MAX_DIGIT_LEN-1 || str[end+1] == '\0') {
            last_start = start;
            while (end - start >= 1) {
                copy_arry_by_indesies(str_to_test, start, end, str);
                if (is_digit_as_word(str_to_test)) {
                    if (first == '\0') {
                        first = '0' + is_digit_as_word(str_to_test);
                    } else {
                        last = '0' + is_digit_as_word(str_to_test);
                    }
                }
                // printf("str_to_test: %s,\tstart: %d,\tend:%d\n", str_to_test, start, end);
                start++;    
            }
            start = last_start;
        }
        
        copy_arry_by_indesies(str_to_test, start, end, str);
        // printf("str_to_test: %s,\tstart: %d,\tend:%d\n", str_to_test, start, end);
        if (is_digit_as_word(str_to_test)) {
            if (first == '\0') {
                first = '0' + is_digit_as_word(str_to_test);
            } else {
                last = '0' + is_digit_as_word(str_to_test);
            }
        }
        i++;
    }
    
    res[0] = first; 
    if (last == '\0') {
        res[1] = first;
    } else {
        res[1] = last;
    }
    res[2] = '\0';
    ans = atoi(res);
    return ans;
}

int get_line(FILE *fp, char *dst)
{
    int i = 0;
    char c;
    
    while ((c = fgetc(fp)) != '\n' && c != EOF) {
        dst[i] = c;
        i++;
        if (i >= MAXLINE) {
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

int is_digit_as_word(char *str)
{
    for (int i = 0; i < NUM_OF_DIGIT; i++) {
        if (strncmp(digits[i], str, length(digits[i])>length(str)?length(digits[i]):length(str)) == 0) {
            return i+1;
        } 
    }
    return 0;
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

void copy_arry_by_indesies(char *target, int start, int end, char *src)
{
    int j = 0;
    for (int i = start; i < end; i++) {
        target[j] = src[i];
        j++;
    }
    target[j] = '\0';
}
