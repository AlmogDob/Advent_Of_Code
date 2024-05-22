#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LEN_LINE (int)1e5
#define MAXDIR 100
#define dprintSTRING(expr) printf(#expr " = %s\n", expr)
#define dprintCHAR(expr) printf(#expr " = %c\n", expr)
#define dprintINT(expr) printf(#expr " = %d\n", expr)

int get_line(FILE *fp, char *dst);
int length(char *str);
int get_curret_number(char *input, int index);
int check_if_gear(char *input, int index);
int get_gear_ratio(char *input, int index);

int stride;

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
    char str[MAX_LEN_LINE], input[MAX_LEN_LINE];
    int len, i = 0, is_gear = 0, sum_of_gear_ratio = 0;
    char c;

    len = stride = get_line(fp, str);
    strcat(input, str);

    while (len != -1) {
        len = get_line(fp, str);
        strcat(input, str);
    }

    while ((c = input[i]) != '\0') {
        if (c == '*') {
            is_gear = check_if_gear(input, i);
        }

        if (is_gear) {
            sum_of_gear_ratio += get_gear_ratio(input, i);
            /*test*/
            // dprintINT(i);
            /*test*/
            is_gear = 0;
        }
        i++;
    }

    dprintINT(sum_of_gear_ratio);
    
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

int get_curret_number(char *input, int index)
{
    char c, number_str[MAX_LEN_LINE];
    int i = index, j = 0;
    
    number_str[0] = '\0';
    
    while (isdigit(c = input[i]) && i % stride >= 0) {
        i--;
    }

    while (isdigit(input[++i]) && i % stride < stride - 1) {
        number_str[j++] = input[i];
    } 
    if (isdigit(input[i]) && i % stride == stride - 1) {
        number_str[j++] = input[i];
    }
    number_str[j] = '\0';

    return atoi(number_str);
    
}

int check_if_gear(char *input, int index)
{
    int rows = length(input) / stride;
    int cols = stride;
    int i, j, temp_index;
    int current_num, num1 = -1, num2 = -1;

    temp_index = index + 1;
    if (isdigit(input[temp_index])) {
        j = temp_index % stride;
        i = temp_index / stride;
        if (i >= 0 && i < rows && j >= 0 && j < cols) {
            current_num = get_curret_number(input, temp_index);
            if (num1 == -1) {
                num1 = current_num;
            }
            if (current_num != num1 && num1 != -1 && num2 == -1) {
                num2 = current_num;
            }
            if (current_num != num1 && num1 != -1 && current_num != num2 && num2 != -1) {
                return 0;
            }
        }
    }
    
    temp_index = index - 1;
    if (isdigit(input[temp_index])) {
        j = temp_index % stride;
        i = temp_index / stride;
        if (i >= 0 && i < rows && j >= 0 && j < cols) {
            current_num = get_curret_number(input, temp_index);
            if (num1 == -1) {
                num1 = current_num;
            }
            if (current_num != num1 && num1 != -1 && num2 == -1) {
                num2 = current_num;
            }
            if (current_num != num1 && num1 != -1 && current_num != num2 && num2 != -1) {
                return 0;
            }
        }
    }
    
    temp_index = index - stride;
    if (isdigit(input[temp_index])) {
        j = temp_index % stride;
        i = temp_index / stride;
        if (i >= 0 && i < rows && j >= 0 && j < cols) {
            current_num = get_curret_number(input, temp_index);
            if (num1 == -1) {
                num1 = current_num;
            }
            if (current_num != num1 && num1 != -1 && num2 == -1) {
                num2 = current_num;
            }
            if (current_num != num1 && num1 != -1 && current_num != num2 && num2 != -1) {
                return 0;
            }
        }
    }
    
    temp_index = index - stride + 1;
    if (isdigit(input[temp_index])) {
        j = temp_index % stride;
        i = temp_index / stride;
        if (i >= 0 && i < rows && j >= 0 && j < cols) {
            current_num = get_curret_number(input, temp_index);
            if (num1 == -1) {
                num1 = current_num;
            }
            if (current_num != num1 && num1 != -1 && num2 == -1) {
                num2 = current_num;
            }
            if (current_num != num1 && num1 != -1 && current_num != num2 && num2 != -1) {
                return 0;
            }
        }
    }
    
    temp_index = index - stride - 1;
    if (isdigit(input[temp_index])) {
        j = temp_index % stride;
        i = temp_index / stride;
        if (i >= 0 && i < rows && j >= 0 && j < cols) {
            current_num = get_curret_number(input, temp_index);
            if (num1 == -1) {
                num1 = current_num;
            }
            if (current_num != num1 && num1 != -1 && num2 == -1) {
                num2 = current_num;
            }
            if (current_num != num1 && num1 != -1 && current_num != num2 && num2 != -1) {
                return 0;
            }
        }
    }
    
    temp_index = index + stride;
    if (isdigit(input[temp_index])) {
        j = temp_index % stride;
        i = temp_index / stride;
        if (i >= 0 && i < rows && j >= 0 && j < cols) {
            current_num = get_curret_number(input, temp_index);
            if (num1 == -1) {
                num1 = current_num;
            }
            if (current_num != num1 && num1 != -1 && num2 == -1) {
                num2 = current_num;
            }
            if (current_num != num1 && num1 != -1 && current_num != num2 && num2 != -1) {
                return 0;
            }
        }
    }
    
    temp_index = index + stride + 1;
    if (isdigit(input[temp_index])) {
        j = temp_index % stride;
        i = temp_index / stride;
        if (i >= 0 && i < rows && j >= 0 && j < cols) {
            current_num = get_curret_number(input, temp_index);
            if (num1 == -1) {
                num1 = current_num;
            }
            if (current_num != num1 && num1 != -1 && num2 == -1) {
                num2 = current_num;
            }
            if (current_num != num1 && num1 != -1 && current_num != num2 && num2 != -1) {
                return 0;
            }
        }
    }
    
    temp_index = index + stride - 1;
    if (isdigit(input[temp_index])) {
        j = temp_index % stride;
        i = temp_index / stride;
        if (i >= 0 && i < rows && j >= 0 && j < cols) {
            current_num = get_curret_number(input, temp_index);
            if (num1 == -1) {
                num1 = current_num;
            }
            if (current_num != num1 && num1 != -1 && num2 == -1) {
                num2 = current_num;
            }
            if (current_num != num1 && num1 != -1 && current_num != num2 && num2 != -1) {
                return 0;
            }
        }
    }

    if (num1 != -1 && num2 != -1) {
        return 1;
    } else {
        return 0;
    }
}

int get_gear_ratio(char *input, int index)
{
    int rows = length(input) / stride;
    int cols = stride;
    int i, j, temp_index;
    int current_num, num1 = -1, num2 = -1;

    temp_index = index + 1;
    if (isdigit(input[temp_index])) {
        j = temp_index % stride;
        i = temp_index / stride;
        if (i >= 0 && i < rows && j >= 0 && j < cols) {
            current_num = get_curret_number(input, temp_index);
            if (num1 == -1) {
                num1 = current_num;
            }
            if (current_num != num1 && num1 != -1 && num2 == -1) {
                num2 = current_num;
            }
            if (current_num != num1 && num1 != -1 && current_num != num2 && num2 != -1) {
                return 0;
            }
        }
    }
    
    temp_index = index - 1;
    if (isdigit(input[temp_index])) {
        j = temp_index % stride;
        i = temp_index / stride;
        if (i >= 0 && i < rows && j >= 0 && j < cols) {
            current_num = get_curret_number(input, temp_index);
            if (num1 == -1) {
                num1 = current_num;
            }
            if (current_num != num1 && num1 != -1 && num2 == -1) {
                num2 = current_num;
            }
            if (current_num != num1 && num1 != -1 && current_num != num2 && num2 != -1) {
                return 0;
            }
        }
    }
    
    temp_index = index - stride;
    if (isdigit(input[temp_index])) {
        j = temp_index % stride;
        i = temp_index / stride;
        if (i >= 0 && i < rows && j >= 0 && j < cols) {
            current_num = get_curret_number(input, temp_index);
            if (num1 == -1) {
                num1 = current_num;
            }
            if (current_num != num1 && num1 != -1 && num2 == -1) {
                num2 = current_num;
            }
            if (current_num != num1 && num1 != -1 && current_num != num2 && num2 != -1) {
                return 0;
            }
        }
    }
    
    temp_index = index - stride + 1;
    if (isdigit(input[temp_index])) {
        j = temp_index % stride;
        i = temp_index / stride;
        if (i >= 0 && i < rows && j >= 0 && j < cols) {
            current_num = get_curret_number(input, temp_index);
            if (num1 == -1) {
                num1 = current_num;
            }
            if (current_num != num1 && num1 != -1 && num2 == -1) {
                num2 = current_num;
            }
            if (current_num != num1 && num1 != -1 && current_num != num2 && num2 != -1) {
                return 0;
            }
        }
    }
    
    temp_index = index - stride - 1;
    if (isdigit(input[temp_index])) {
        j = temp_index % stride;
        i = temp_index / stride;
        if (i >= 0 && i < rows && j >= 0 && j < cols) {
            current_num = get_curret_number(input, temp_index);
            if (num1 == -1) {
                num1 = current_num;
            }
            if (current_num != num1 && num1 != -1 && num2 == -1) {
                num2 = current_num;
            }
            if (current_num != num1 && num1 != -1 && current_num != num2 && num2 != -1) {
                return 0;
            }
        }
    }
    
    temp_index = index + stride;
    if (isdigit(input[temp_index])) {
        j = temp_index % stride;
        i = temp_index / stride;
        if (i >= 0 && i < rows && j >= 0 && j < cols) {
            current_num = get_curret_number(input, temp_index);
            if (num1 == -1) {
                num1 = current_num;
            }
            if (current_num != num1 && num1 != -1 && num2 == -1) {
                num2 = current_num;
            }
            if (current_num != num1 && num1 != -1 && current_num != num2 && num2 != -1) {
                return 0;
            }
        }
    }
    
    temp_index = index + stride + 1;
    if (isdigit(input[temp_index])) {
        j = temp_index % stride;
        i = temp_index / stride;
        if (i >= 0 && i < rows && j >= 0 && j < cols) {
            current_num = get_curret_number(input, temp_index);
            if (num1 == -1) {
                num1 = current_num;
            }
            if (current_num != num1 && num1 != -1 && num2 == -1) {
                num2 = current_num;
            }
            if (current_num != num1 && num1 != -1 && current_num != num2 && num2 != -1) {
                return 0;
            }
        }
    }
    
    temp_index = index + stride - 1;
    if (isdigit(input[temp_index])) {
        j = temp_index % stride;
        i = temp_index / stride;
        if (i >= 0 && i < rows && j >= 0 && j < cols) {
            current_num = get_curret_number(input, temp_index);
            if (num1 == -1) {
                num1 = current_num;
            }
            if (current_num != num1 && num1 != -1 && num2 == -1) {
                num2 = current_num;
            }
            if (current_num != num1 && num1 != -1 && current_num != num2 && num2 != -1) {
                return 0;
            }
        }
    }

    if (num1 != -1 && num2 != -1) {
        /*test*/
        // dprintINT(num1);
        // dprintINT(num2);
        /*test*/
        return num1 * num2;
    } else {
        return 0;
    }
}
