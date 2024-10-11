#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <inttypes.h>
#include "Almog_Dynamic_Array.h"

enum hand_type{
    high_card,
    one_pair,
    two_pair, 
    three_of_a_kind,
    full_house,
    four_of_a_kind,
    five_of_a_kind
};

typedef struct {
    char cards[6];
    size_t bid;
    int type;
    int ordered_card[5]; /* 14=A, 13=k, 12=Q, 11=J...*/
    int histogram[13];
    size_t profit;
} hand;

typedef struct {
    size_t length;
    size_t capacity;
    hand* elements;
} ada_hand_array;


#define MAXDIR 100
#define MAX_LEN_LINE (int)1e3
#define NUM_OF_RACES (int)1e1
#define NUM_OF_DISTANCES (int)5e2
#define dprintSTRING(expr) printf(#expr " = %s\n", expr)
#define dprintCHAR(expr) printf(#expr " = %c\n", expr)
#define dprintINT(expr) printf(#expr " = %d\n", expr)

int get_line(FILE *fp, char *dst);
int length(char *str);
int get_next_word_from_line(char *dst, char *src);
void copy_arry_by_indesies(char *target, int start, int end, char *src);
int get_word_and_cut(char *dst, char *src);
void print_hand(hand h);
void print_game(ada_hand_array game);
void set_type_high_card(hand *h);
int commper_hands(hand h1, hand h2);
void my_qsort_hands(hand v[], int left, int right);
void swap_hands(hand v[], int i, int j);
void my_qsort_int(int v[], int left, int right);
void swap_int(int v[], int i, int j);

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
    ada_hand_array game;
    ada_array(hand, game);

    int line_len;
    size_t result = 0; 
    char current_line[MAX_LEN_LINE], current_word[MAX_LEN_LINE];
    hand current_hand;

    while ((line_len = get_line(fp, current_line)) != -1) {
        get_word_and_cut(current_hand.cards, current_line);
        get_word_and_cut(current_word, current_line);
        sscanf(current_word, "%zu", &current_hand.bid);

        ada_appand(hand, game, current_hand);
    }
    
    for (int i = 0; i < (int)game.length; i++) {
        set_type_high_card(&game.elements[i]);
    }

    my_qsort_hands(game.elements, 0, game.length-1);

    for (size_t i = 0; i < game.length; i++) {
        game.elements[i].profit = game.elements[i].bid * (i+1);
        result += game.elements[i].profit;
    } 
    print_game(game);

    // print_hand(game.elements[0]);
    // print_hand(game.elements[1]);
    // dprintINT(commper_hands(game.elements[0], game.elements[1]));


    printf("%zu\n", result);
    
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

void print_hand(hand h)
{
    printf("cards:        %s\nbid:          %zu\ntype:         %d\n", h.cards, h.bid, h.type);
    printf("orderd hand: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", h.ordered_card[i]);
    }
    printf("\n");
    printf("histogram: ");
    for (int i = 0; i < 13; i++) {
        printf("%d ", h.histogram[i]);
    }
    printf("\n           ");
    for (int i = 2; i < 10; i++) {
        printf("%d ",i);
    }
    printf("\n");
    printf("profit: %zu\n", h.profit);
}

void print_game(ada_hand_array game)
{
    for (int i = 0; i < (int)game.length; i++) {
        printf("hand-%d\n", i+1);
        printf("-------\n");
        print_hand(game.elements[i]);
        printf("\n");
    }
}

void set_type_high_card(hand *h)
{
    /* 2, 3, 4, 5, 6, 7, 8, 9, T, J, Q, K, A */
    int histogram[13] = {0};
    int first_high = 0, second_high = 0, low = 5;

    for (int i = 0; i < (int)length(h->cards); i++){
        if (h->cards[i] == 'A') {
            histogram[13-1] += 1;
        } else if (isdigit(h->cards[i])) {
            histogram[h->cards[i] - '0' - 2] += 1;
        } else if (h->cards[i] == 'K') {
            histogram[12 - 1] += 1;
        } else if (h->cards[i] == 'Q') {
            histogram[11 - 1] += 1;
        } else if (h->cards[i] == 'J') {
            histogram[10 - 1] += 1;
        } else if (h->cards[i] == 'T') {
            histogram[9 - 1] += 1;
        }
    }
    for (int i = 0; i < 13; i++){
        if (histogram[i] >= first_high) {
            if (histogram[i] == first_high) {
                second_high = histogram[i];
            } else {
                first_high = histogram[i];
            }
        }
        if (histogram[i] > second_high && histogram[i] < first_high) {
            second_high = histogram[i];
        }
        if (histogram[i] != 0 && histogram[i] < low) {
            low = histogram[i];
        }
    }

    for (int i = 0; i < 13; i++) {
        h->histogram[i] = histogram[i];
    }

    if (first_high == 5) {
        h->type = five_of_a_kind;
    } else if (first_high == 4) {   
        h->type = four_of_a_kind;
    } else if (first_high == 3 && second_high == 2) {   
        h->type = full_house;
    } else if (first_high == 3 && second_high < 2) {    
        h->type = three_of_a_kind;
    } else if (first_high == 2 && second_high == 2) {   
        h->type = two_pair;
    } else if (first_high == 2 && second_high < 2) {    
        h->type = one_pair;
    } else {    
        h->type = high_card;
    }

    for (int i = 0; i < 5; i++) {
        if (h->cards[i] == 'A') {
            h->ordered_card[i] = 14;
        } else if (h->cards[i] == 'K') {
            h->ordered_card[i] = 13;
        } else if (h->cards[i] == 'Q') {
            h->ordered_card[i] = 12;
        } else if (h->cards[i] == 'J') {
            h->ordered_card[i] = 11;
        } else if (h->cards[i] == 'T') {
            h->ordered_card[i] = 10;
        } else {
            h->ordered_card[i] = h->cards[i] - '0';
        }
    }

    my_qsort_int(h->ordered_card, 0, 4);

}

int commper_hands(hand h1, hand h2)
{
    if (h1.type > h2.type) {
        return 1;
    } else if (h2.type > h1.type) {
        return 2;
    } else if (h1.type == high_card) {
        for (int i = 4; i >= 0; i--) {
            // printf("%d, %d\n", h1.ordered_card[i], h2.ordered_card[i]);
            if (h1.ordered_card[i] > h2.ordered_card[i]) {
                return 1;
            }
            if (h2.ordered_card[i] > h1.ordered_card[i]) {
                return 2;
            }
        }
    } else {
        for (int i = 0; i < 5; i++) {
            if (h1.cards[i] == h2.cards[i]) {
                continue;
            }
            if (h1.cards[i] == 'A') {
                return 1;
            }
            if (h2.cards[i] == 'A') {
                return 2;
            }
            if (h1.cards[i] == 'K') {
                return 1;
            }
            if (h2.cards[i] == 'K') {
                return 2;
            }
            if (h1.cards[i] == 'Q') {
                return 1;
            }
            if (h2.cards[i] == 'Q') {
                return 2;
            }
            if (h1.cards[i] == 'J') {
                return 1;
            }
            if (h2.cards[i] == 'J') {
                return 2;
            }
            if (h1.cards[i] == 'T') {
                return 1;
            }
            if (h2.cards[i] == 'T') {
                return 2;
            }
            if (atoi(&(h1.cards[i])) > atoi(&(h2.cards[i]))) {
                // dprintCHAR(h1.cards[i]);
                // dprintCHAR(h2.cards[i]);
                return 1;
            }
            if (atoi(&(h2.cards[i])) > atoi(&(h1.cards[i]))) {
                return 2;
            }
        }
    }
    return 0;
}

/* qsort: sort v[left]...v[right] int increasing order */
void my_qsort_hands(hand v[], int left, int right)
{
    int i, last;

    if (left >= right)                  /* do nothing if array contains */
        return;                         /* fewer than two elements */
    swap_hands(v, left, (left + right) / 2);  /* move partition elem */
    last = left;                        /* to v[0] */
    for (i = left + 1; i <= right; i++) /* partition */
        if (commper_hands(v[i], v[left])  == 2)  
            swap_hands(v, ++last, i);
    swap_hands(v, left, last); /* restore partition elem */
    my_qsort_hands(v, left, last - 1);
    my_qsort_hands(v, last + 1, right);
}

/* swap_hands: interchange v[i] and v[j] */
void swap_hands(hand v[], int i, int j)
{
    hand temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

/* qsort: sort v[left]...v[right] int increasing order */
void my_qsort_int(int v[], int left, int right)
{
    int i, last;

    if (left >= right)                  /* do nothing if array contains */
        return;                         /* fewer than two elements */
    swap_int(v, left, (left + right) / 2);  /* move partition elem */
    last = left;                        /* to v[0] */
    for (i = left + 1; i <= right; i++) /* partition */
        if (v[i] < v[left])  
            swap_int(v, ++last, i);
    swap_int(v, left, last); /* restore partition elem */
    my_qsort_int(v, left, last - 1);
    my_qsort_int(v, last + 1, right);
}

/* swap: interchange v[i] and v[j] */
void swap_int(int v[], int i, int j)
{
    int temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}