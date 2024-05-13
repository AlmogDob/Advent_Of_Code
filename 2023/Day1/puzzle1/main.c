#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXDIR 100
#define MAXLINE 100

int get_calibration_values(char *str);
int get_line(FILE *fp, char *dst);

int main(int argc, char const *argv[])
{
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
    char c;
    int i = 0;
    int ans;

    while ((c = str[i]) != '\0') {
        if (isdigit(c) ) {
            if (first == '\0') {
            first = str[i];
            } else {
                last = str[i];
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
