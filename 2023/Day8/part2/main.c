#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "Almog_Dynamic_Array.h"

#define MAXDIR 100
#define MAX_LEN_LINE (int)1e3
#define NUM_OF_RACES (int)1e1
#define NUM_OF_DISTANCES (int)5e2
#define dprintSTRING(expr) printf(#expr " = %s\n", expr)
#define dprintCHAR(expr) printf(#expr " = %c\n", expr)
#define dprintINT(expr) printf(#expr " = %d\n", expr)

#define ADRESS_LEN 3

typedef struct {
    char name[ADRESS_LEN+1];
    char right[ADRESS_LEN+1];
    char left[ADRESS_LEN+1];
} Node;

typedef struct {
    size_t length;
    size_t capacity;
    Node* elements;
} ada_node_array;

typedef struct {
    size_t length;
    size_t capacity;
    char* elements;
} ada_char_array;

typedef struct {
    char start_node[ADRESS_LEN+1];
    char current_node[ADRESS_LEN+1];
    int direction_index;
} Path;

typedef struct {
    size_t length;
    size_t capacity;
    Path* elements;
} ada_path_array;

int get_line(FILE *fp, char *dst);
int length(char *str);
int get_next_word_from_line(char *dst, char *src);
void copy_arry_by_indesies(char *target, int start, int end, char *src);
int get_word_and_cut(char *dst, char *src);
void print_node(Node n);
void print_network(ada_node_array *n);
void print_directions(ada_char_array *d);
void get_next_node_name(char *name, Node node, char direction);
int get_index_of_node(ada_node_array *network, char *name);
int walk_network(ada_node_array *network, ada_char_array *directions);

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
    ada_node_array network;
    ada_char_array directions;
    Node current_node;
    char current_word[MAX_LEN_LINE], current_line[MAX_LEN_LINE];
    int line_len, i = 0;

    ada_array(Node, network);
    ada_array(char, directions);

    get_line(fp, current_line);
    while (current_line[i] != '\0') {
        ada_appand(char, directions, current_line[i]);
        i++;
    }
    

    get_line(fp, current_line);

    while ((line_len = get_line(fp, current_line)) != -1) {

        get_word_and_cut(current_word, current_line);
        strncpy(current_node.name, current_word, ADRESS_LEN+1);

        get_word_and_cut(current_word, current_line);
        get_word_and_cut(current_word, current_line);
        copy_arry_by_indesies(current_word, 1, ADRESS_LEN+1, current_word);
        strncpy(current_node.left, current_word, ADRESS_LEN+1);

        get_word_and_cut(current_word, current_line);
        get_word_and_cut(current_word, current_line);
        copy_arry_by_indesies(current_word, 0, 3, current_word);
        strncpy(current_node.right, current_word, ADRESS_LEN+1);

        ada_appand(Node, network, current_node);
    }

    print_directions(&directions);
    // print_network(&network);
    

    dprintINT(walk_network(&network, &directions)-1);
    
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
    copy_arry_by_indesies(src, last_pos, length(src), src);
    return 1;
}

void print_node(Node n)
{
    printf("%s = (%s, %s)\n", n.name, n.left, n.right);
}

void print_network(ada_node_array *n)
{
    for (size_t i = 0; i < n->length; i++) {
        print_node(n->elements[i]);
    }
}

void print_directions(ada_char_array *d)
{
    for (size_t i = 0; i < d->length; i++) {
        printf("%c",d->elements[i]);
    }
    printf("\n");
}

void get_next_node_name(char *name, Node node, char direction)
{
    if (direction == 'L') {
        strncpy(name, node.left,ADRESS_LEN+1);
    }
    if (direction == 'R') {
        strncpy(name, node.right,ADRESS_LEN+1);
    }
}

int get_index_of_node(ada_node_array *network, char *name)
{
    for (size_t i = 0; i < network->length; i++) {
        if (!strncmp(name, network->elements[i].name, ADRESS_LEN+1)) {
            return i;
        }
    }
    return -1;
}

int walk_network(ada_node_array *network, ada_char_array *directions)
{
    int step = 0, next_node_index, start_index;
    char current_node_name[ADRESS_LEN+1] = {0}, next_node_name[ADRESS_LEN+1] = {0};

    start_index = get_index_of_node(network, "AAA");
    get_next_node_name(next_node_name, network->elements[start_index], directions->elements[0]);
    step++;

    while (strncmp(current_node_name, "ZZZ", ADRESS_LEN+1)) {
        if (step > 1000000) {
            return -1;
        }

        // dprintSTRING(next_node_name);
        next_node_index = get_index_of_node(network, next_node_name);
        print_node(network->elements[next_node_index]);

        strncpy(current_node_name, network->elements[next_node_index].name, ADRESS_LEN+1);
        dprintSTRING(current_node_name);
        dprintCHAR(directions->elements[step%(directions->length)]);
        next_node_index = get_index_of_node(network, current_node_name);
        get_next_node_name(next_node_name,
                           network->elements[next_node_index],
                           directions->elements[step%(directions->length)]);
        step++;

    }
    return step;
}
