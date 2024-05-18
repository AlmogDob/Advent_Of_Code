#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXDIR 100
#define MAXLINE 500
#define MAXRED 12
#define MAXGREEN 13
#define MAXBLUE 14
#define dprintSTRING(expr) printf(#expr " = %s\n", expr)
#define dprintCHAR(expr) printf(#expr " = %c\n", expr)
#define dprintINT(expr) printf(#expr " = %d\n", expr)

int get_line(FILE *fp, char *dst);
int length(char *str);
void copy_arry_by_indesies(char *target, int start, int end, char *src);
int get_next_word_from_line(char *dst, char *src);
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

    int current_line_length, success, game_ID_sum = 0,
    max_num_of_red, max_num_of_green, max_num_of_blue,
    current_num = 0, current_ID = 0;
    char dst[MAXLINE], str[MAXLINE]; 

    while ((current_line_length = get_line(fp, str)) != -1) {
        max_num_of_red = 0;     
        max_num_of_green = 0;     
        max_num_of_blue = 0;     

        success = get_word_and_cut(dst, str);

        if (strcmp(dst, "Game") == 0) {
            success = get_word_and_cut(dst, str);
            current_ID = atoi(dst);          
        }
        
        while (success) {

            success = get_word_and_cut(dst, str);
            if (strcmp(dst, ":") == 0) {
                ;
            }
            
            if ((current_num = atoi(dst))) {
                success = get_word_and_cut(dst, str);
                if (strcmp(dst, "red") == 0) {
                    /*test*/
                    // dprintSTRING(dst);
                    /*test*/
                    if (current_num > max_num_of_red) {
                        max_num_of_red = current_num;
                    }
                }

                if (strcmp(dst, "green") == 0) {
                    /*test*/
                    // dprintSTRING(dst);
                    /*test*/
                    if (current_num > max_num_of_green) {
                        max_num_of_green = current_num;
                    }
                }
                    
                if (strcmp(dst, "blue") == 0) {
                    /*test*/
                    // dprintSTRING(dst);
                    /*test*/
                    if (current_num > max_num_of_blue) {
                        max_num_of_blue = current_num;
                    }
                }
            }
            /*test*/
            // dprintSTRING(dst);
            // dprintSTRING(str);
            /*test*/
        }

        if (max_num_of_red <= MAXRED &&
            max_num_of_green <= MAXGREEN &&
            max_num_of_blue <= MAXBLUE) {
                game_ID_sum += current_ID;
        }
    }

    dprintINT(game_ID_sum);
    return 0;
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