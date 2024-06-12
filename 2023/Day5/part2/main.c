
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <inttypes.h>

#define MAX_LEN_LINE (int)1e3
#define MAX_NUM_OF_NUMBERS (int)1e6
#define MAXDIR 100
#define dprintSTRING(expr) printf(#expr " = %s\n", expr)
#define dprintCHAR(expr) printf(#expr " = %c\n", expr)
#define dprintINT(expr) printf(#expr " = %d\n", expr)
#define dprintUL(expr) printf(#expr " = %lu\n", expr)
#define dprintUINT(expr) printf(#expr " = %u\n", expr)

int get_line(FILE *fp, char *dst);
int length(char *str);
int get_next_word_from_line(char *dst, char *src);
void copy_arry_by_indesies(char *target, int start, int end, char *src);
int get_word_and_cut(char *dst, char *src);
void populat(FILE *fp, uint64_t *current_list, uint64_t *previus_list);
// void propaget(FILE *fp, uint64_t *seed_numbers, uint64_t *soil_numbers,
//               uint64_t *fertilize_numbers, uint64_t *water_numbers,
//               uint64_t *light_numbers, uint64_t *temperature_numbers,
//               uint64_t *humidity_numbers, uint64_t *location_numbers);

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

    uint64_t seed_numbers[MAX_NUM_OF_NUMBERS] = {0},
    soil_numbers[MAX_NUM_OF_NUMBERS] = {0},
    fertilize_numbers[MAX_NUM_OF_NUMBERS] = {0},
    water_numbers[MAX_NUM_OF_NUMBERS] = {0},
    light_numbers[MAX_NUM_OF_NUMBERS] = {0},
    temperature_numbers[MAX_NUM_OF_NUMBERS] = {0},
    humidity_numbers[MAX_NUM_OF_NUMBERS] = {0},
    location_numbers[MAX_NUM_OF_NUMBERS] = {0},
    smallest_location, start, length;

    char current_line[MAX_LEN_LINE], current_word[MAX_LEN_LINE];

    while((current_line_len = get_line(fp, current_line)) != -1) {
        get_word_and_cut(current_word, current_line);
        if (!strcmp(current_word, "seeds")) {
            get_word_and_cut(current_word, current_line);
            sscanf(current_line, "%lu %lu", &start, &length);
            dprintUL(start);
            dprintUL(length);
            dprintSTRING(current_line);
        }
        
        if (!strcmp(current_word, "seed-to-soil")) {
            populat(fp, soil_numbers, seed_numbers);
        }

        if (!strcmp(current_word, "soil-to-fertilizer")) {
            populat(fp, fertilize_numbers, soil_numbers);
        }

        if (!strcmp(current_word, "fertilizer-to-water")) {
            populat(fp, water_numbers, fertilize_numbers);
        }

        if (!strcmp(current_word, "water-to-light")) {
            populat(fp, light_numbers, water_numbers);
        }

        if (!strcmp(current_word, "light-to-temperature")) {
            populat(fp, temperature_numbers, light_numbers);
        }

        if (!strcmp(current_word, "temperature-to-humidity")) {
            populat(fp, humidity_numbers, temperature_numbers);
        }

        if (!strcmp(current_word, "humidity-to-location")) {
            populat(fp, location_numbers, humidity_numbers);
        }
    }

    i = 0;
    smallest_location = location_numbers[0];
    while (seed_numbers[i] != 0) {
        // dprintUL(seed_numbers[i]);
        // dprintUL(soil_numbers[i]);
        // dprintUL(fertilize_numbers[i]);
        // dprintUL(water_numbers[i]);
        // dprintUL(light_numbers[i]);
        // dprintUL(temperature_numbers[i]);
        // dprintUL(humidity_numbers[i]);
        // dprintUL(location_numbers[i]);
        if (smallest_location > location_numbers[i]) {
            smallest_location = location_numbers[i];
        }
        i++;
    }

    dprintUL(smallest_location);
    
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

void populat(FILE *fp, uint64_t *current_list, uint64_t *previus_list)
{
    char current_line[MAX_LEN_LINE];
    uint64_t destination_range_start, source_range_start, range_length,
    current_number;

    get_line(fp, current_line);
    while (strcmp(current_line, "")) {
        sscanf(current_line, "%lu %lu %lu", &destination_range_start, &source_range_start, &range_length);
        // get_word_and_cut(current_word, current_line);
        // destination_range_start = atol(current_word);
        // get_word_and_cut(current_word, current_line);
        // source_range_start = atol(current_word);
        // get_word_and_cut(current_word, current_line);
        // range_length = atol(current_word);
        for(int i = 0; i < MAX_NUM_OF_NUMBERS; i++) {
            current_number = previus_list[i];
            if (current_number >= source_range_start &&
                current_number < (source_range_start + range_length)) {
                    current_list[i] = (current_number - source_range_start) + destination_range_start;
                }
            else if (current_list[i] == 0) {
                current_list[i] = current_number;
            }
        }
        get_line(fp, current_line);
    }

}

// void propaget(FILE *fp, uint64_t *seed_numbers, uint64_t *soil_numbers,
//               uint64_t *fertilize_numbers, uint64_t *water_numbers,
//               uint64_t *light_numbers, uint64_t *temperature_numbers,
//               uint64_t *humidity_numbers, uint64_t *location_numbers)
// {

// }
