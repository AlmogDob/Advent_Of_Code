#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <inttypes.h>

#define MAX_LEN_LINE (int)1e3
#define MAX_NUM_OF_NUMBERS (int)1e4
#define MAXDIR 100
#define dprintSTRING(expr) printf(#expr " = %s\n", expr)
#define dprintCHAR(expr) printf(#expr " = %c\n", expr)
#define dprintINT(expr) printf(#expr " = %d\n", expr)
#define dprintUL(expr) printf(#expr " = %lu\n", expr)
#define dprintUINT(expr) printf(#expr " = %u\n", expr)
#define PRINT_RANGE(expr) printf(#expr " = (%lu, %lu)\n", expr.start, expr.end)
#define PRINT_MAP(expr) printf(#expr ":\nsource: (%lu, %lu)\ndestination: (%lu, %lu)\n", expr.source.start, expr.source.end, expr.destination.start, expr.destination.end)

typedef struct {
    uint64_t start;
    uint64_t end;
} Range;

typedef struct {
    Range source;
    Range destination;
} Map;

int get_line(FILE *fp, char *dst);
int length(char *str);
int get_next_word_from_line(char *dst, char *src);
void copy_arry_by_indesies(char *target, int start, int end, char *src);
int get_word_and_cut(char *dst, char *src);
void populat_maps(FILE *fp, Map *maps, int *number_of_maps);
void propegate_layer(Map* maps, int number_of_maps, uint64_t source, uint64_t *destination);
void creat_sub_ranges(Range *temp_ranges, Range *ranges, int number_ranges, Map *maps, int number_of_maps, int *number_of_temp_ranges);
void qsort_ranges(Range *v, int left, int right);
void swap_ranges(Range *v, int i, int j);

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
    Range seeds_ranges[MAX_NUM_OF_NUMBERS] = {0},
    seeds_temp_ranges[MAX_NUM_OF_NUMBERS] = {0},
    soil_ranges[MAX_NUM_OF_NUMBERS] = {0},
    soil_temp_ranges[MAX_NUM_OF_NUMBERS] = {0},
    fertilizer_ranges[MAX_NUM_OF_NUMBERS] = {0},
    fertilizer_temp_ranges[MAX_NUM_OF_NUMBERS] = {0},
    water_ranges[MAX_NUM_OF_NUMBERS] = {0},
    water_temp_ranges[MAX_NUM_OF_NUMBERS] = {0},
    light_ranges[MAX_NUM_OF_NUMBERS] = {0},
    light_temp_ranges[MAX_NUM_OF_NUMBERS] = {0},
    temperature_ranges[MAX_NUM_OF_NUMBERS] = {0},
    temperature_temp_ranges[MAX_NUM_OF_NUMBERS] = {0},
    humidity_ranges[MAX_NUM_OF_NUMBERS] = {0},
    humidity_temp_ranges[MAX_NUM_OF_NUMBERS] = {0},
    location_ranges[MAX_NUM_OF_NUMBERS] = {0},
    location_temp_ranges[MAX_NUM_OF_NUMBERS] = {0};
    
    Map seed_to_soil_maps[MAX_NUM_OF_NUMBERS] = {0},
    soil_to_fertilizer_maps[MAX_NUM_OF_NUMBERS] = {0},
    fertilizer_to_water_maps[MAX_NUM_OF_NUMBERS] = {0},
    water_to_light_maps[MAX_NUM_OF_NUMBERS] = {0},
    light_to_temperature_maps[MAX_NUM_OF_NUMBERS] = {0},
    temperature_to_humidity_maps[MAX_NUM_OF_NUMBERS] = {0},
    humidity_to_location_maps[MAX_NUM_OF_NUMBERS] = {0};

    int current_line_length, i, number_of_seed_to_soil_maps,
    number_of_soil_to_fertilizer_maps, number_of_fertilizer_to_water_maps,
    number_of_water_to_light_maps, number_of_light_to_temperature_maps,
    number_of_temperature_to_humidity_maps, number_of_humidity_to_location_maps;

    int number_of_seed_ranges, number_of_soil_ranges,
    number_of_fertilizer_ranges, number_of_water_ranges,
    number_of_light_ranges, number_of_temperature_ranges,
    number_of_humidity_ranges, number_of_location_ranges;

    int number_of_temp_seed_ranges, number_of_temp_soil_ranges,
    number_of_temp_fertilizer_ranges, number_of_temp_water_ranges,
    number_of_temp_light_ranges, number_of_temp_temperature_ranges,
    number_of_temp_humidity_ranges, number_of_temp_location_ranges;

    char current_line[MAX_LEN_LINE], current_word[MAX_LEN_LINE];

    uint64_t range_length, range_start;
    
    while ((current_line_length = get_line(fp, current_line)) != -1) {
        get_word_and_cut(current_word, current_line);
        if (!strcmp(current_word, "seeds")) {
            get_word_and_cut(current_word, current_line);
            i = 0;
            while (get_word_and_cut(current_word, current_line)) {
                sscanf(current_word, "%lu", &range_start);
                get_word_and_cut(current_word, current_line);
                sscanf(current_word, "%lu", &range_length);
                seeds_ranges[i].start = range_start;
                seeds_ranges[i].end = range_start + range_length-1;
                // PRINT_RANGE(seeds_ranges[i]);
                i++;
                number_of_seed_ranges = i;
            }
        }
        
        if (!strcmp(current_word, "seed-to-soil")) {
            populat_maps(fp, seed_to_soil_maps, &number_of_seed_to_soil_maps);
        }

        if (!strcmp(current_word, "soil-to-fertilizer")) {
            populat_maps(fp, soil_to_fertilizer_maps, &number_of_soil_to_fertilizer_maps);
        }

        if (!strcmp(current_word, "fertilizer-to-water")) {
            populat_maps(fp, fertilizer_to_water_maps, &number_of_fertilizer_to_water_maps);
        }

        if (!strcmp(current_word, "water-to-light")) {
            populat_maps(fp, water_to_light_maps, &number_of_water_to_light_maps);
        }

        if (!strcmp(current_word, "light-to-temperature")) {
            populat_maps(fp, light_to_temperature_maps, &number_of_light_to_temperature_maps);
        }

        if (!strcmp(current_word, "temperature-to-humidity")) {
            populat_maps(fp, temperature_to_humidity_maps, &number_of_temperature_to_humidity_maps);
        }

        if (!strcmp(current_word, "humidity-to-location")) {
            populat_maps(fp, humidity_to_location_maps, &number_of_humidity_to_location_maps);
        }
    }

    // for (i = 0; i < MAX_NUM_OF_NUMBERS; i++) {
    //     PRINT_MAP(seed_to_soil_maps[i]);
    // }
    // printf("\n");
    // for (i = 0; i < MAX_NUM_OF_NUMBERS; i++) {
    //     PRINT_MAP(soil_to_fertilizer_maps[i]);
    // }
    // printf("\n");
    // for (i = 0; i < MAX_NUM_OF_NUMBERS; i++) {
    //     PRINT_MAP(fertilizer_to_water_maps[i]);
    // }
    // printf("\n");
    // for (i = 0; i < MAX_NUM_OF_NUMBERS; i++) {
    //     PRINT_MAP(water_to_light_maps[i]);
    // }
    // printf("\n");
    // for (i = 0; i < MAX_NUM_OF_NUMBERS; i++) {
    //     PRINT_MAP(light_to_temperature_maps[i]);
    // }
    // printf("\n");
    // for (i = 0; i < MAX_NUM_OF_NUMBERS; i++) {
    //     PRINT_MAP(temperature_to_humidity_maps[i]);
    // }
    // printf("\n");
    // for (i = 0; i < MAX_NUM_OF_NUMBERS; i++) {
    //     PRINT_MAP(humidity_to_location_maps[i]);
    // }

    creat_sub_ranges(seeds_temp_ranges, seeds_ranges, number_of_seed_ranges,
                     seed_to_soil_maps, number_of_seed_to_soil_maps,
                     &number_of_temp_seed_ranges);
    for (int i = 0; i < number_of_temp_seed_ranges; i++) {
        propegate_layer(seed_to_soil_maps, number_of_seed_to_soil_maps,
                        seeds_temp_ranges[i].start, &soil_ranges[i].start);
        propegate_layer(seed_to_soil_maps, number_of_seed_to_soil_maps,
                        seeds_temp_ranges[i].end, &soil_ranges[i].end);
        
    }
    number_of_soil_ranges = number_of_temp_seed_ranges;
    for (int i = 0; i < number_of_soil_ranges; i++) {
        // PRINT_RANGE(soil_ranges[i]);
    }


    creat_sub_ranges(soil_temp_ranges, soil_ranges, number_of_soil_ranges,
                     soil_to_fertilizer_maps, number_of_soil_to_fertilizer_maps,
                     &number_of_temp_soil_ranges);
    for (int i = 0; i < number_of_temp_seed_ranges; i++) {
        propegate_layer(soil_to_fertilizer_maps, number_of_soil_to_fertilizer_maps,
                        soil_temp_ranges[i].start, &fertilizer_ranges[i].start);
        propegate_layer(soil_to_fertilizer_maps, number_of_soil_to_fertilizer_maps,
                        soil_temp_ranges[i].end, &fertilizer_ranges[i].end);
    }
    number_of_fertilizer_ranges = number_of_temp_soil_ranges;
    for (int i = 0; i < number_of_fertilizer_ranges; i++) {
        // PRINT_RANGE(fertilizer_ranges[i]);
    }


    creat_sub_ranges(fertilizer_temp_ranges, fertilizer_ranges, number_of_fertilizer_ranges,
                     fertilizer_to_water_maps, number_of_fertilizer_to_water_maps,
                     &number_of_temp_fertilizer_ranges);
    for (int i = 0; i < number_of_temp_fertilizer_ranges; i++) {
        propegate_layer(fertilizer_to_water_maps, number_of_fertilizer_to_water_maps,
                        fertilizer_temp_ranges[i].start, &water_ranges[i].start);
        propegate_layer(fertilizer_to_water_maps, number_of_fertilizer_to_water_maps,
                        fertilizer_temp_ranges[i].end, &water_ranges[i].end);
    }
    number_of_water_ranges = number_of_temp_fertilizer_ranges;
    for (int i = 0; i < number_of_water_ranges; i++) {
        // PRINT_RANGE(water_ranges[i]);
    }
    

    creat_sub_ranges(water_temp_ranges, water_ranges, number_of_water_ranges,
                     water_to_light_maps, number_of_water_to_light_maps,
                     &number_of_temp_water_ranges);
    for (int i = 0; i < number_of_temp_water_ranges; i++) {
        propegate_layer(water_to_light_maps, number_of_water_to_light_maps,
                        water_temp_ranges[i].start, &light_ranges[i].start);
        propegate_layer(water_to_light_maps, number_of_water_to_light_maps,
                        water_temp_ranges[i].end, &light_ranges[i].end);
    }
    number_of_light_ranges = number_of_temp_water_ranges;
    for (int i = 0; i < number_of_light_ranges; i++) {
        // PRINT_RANGE(light_ranges[i]);
    }
    

    creat_sub_ranges(light_temp_ranges, light_ranges, number_of_light_ranges,
                     light_to_temperature_maps, number_of_light_to_temperature_maps,
                     &number_of_temp_light_ranges);
    for (int i = 0; i < number_of_temp_light_ranges; i++) {
        propegate_layer(light_to_temperature_maps, number_of_light_to_temperature_maps,
                        light_temp_ranges[i].start, &temperature_ranges[i].start);
        propegate_layer(light_to_temperature_maps, number_of_light_to_temperature_maps,
                        light_temp_ranges[i].end, &temperature_ranges[i].end);
    }
    number_of_temperature_ranges = number_of_temp_light_ranges;
    for (int i = 0; i < number_of_temperature_ranges; i++) {
        // PRINT_RANGE(temperature_ranges[i]);
    }
    

    creat_sub_ranges(temperature_temp_ranges, temperature_ranges, number_of_temperature_ranges,
                     temperature_to_humidity_maps, number_of_temperature_to_humidity_maps,
                     &number_of_temp_temperature_ranges);
    for (int i = 0; i < number_of_temp_temperature_ranges; i++) {
        propegate_layer(temperature_to_humidity_maps, number_of_temperature_to_humidity_maps,
                        temperature_temp_ranges[i].start, &humidity_ranges[i].start);
        propegate_layer(temperature_to_humidity_maps, number_of_temperature_to_humidity_maps,
                        temperature_temp_ranges[i].end, &humidity_ranges[i].end);
    }
    number_of_humidity_ranges = number_of_temp_temperature_ranges;
    for (int i = 0; i < number_of_humidity_ranges; i++) {
        // PRINT_RANGE(humidity_ranges[i]);
    }
    

    creat_sub_ranges(humidity_temp_ranges, humidity_ranges, number_of_humidity_ranges,
                     humidity_to_location_maps, number_of_humidity_to_location_maps,
                     &number_of_temp_humidity_ranges);
    for (int i = 0; i < number_of_temp_humidity_ranges; i++) {
        propegate_layer(humidity_to_location_maps, number_of_humidity_to_location_maps,
                        humidity_temp_ranges[i].start, &location_ranges[i].start);
        propegate_layer(humidity_to_location_maps, number_of_humidity_to_location_maps,
                        humidity_temp_ranges[i].end, &location_ranges[i].end);
    }
    number_of_location_ranges = number_of_temp_humidity_ranges;
    for (int i = 0; i < number_of_location_ranges; i++) {
        PRINT_RANGE(location_ranges[i]);
    }

    uint64_t smallest_location = location_ranges[0].start;
    for (int i = 0; i < number_of_location_ranges; i++) {
        if (smallest_location > location_ranges[i].start) {
            smallest_location = location_ranges[i].start;
        }
    }

    dprintINT(smallest_location); 

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

void populat_maps(FILE *fp, Map *maps, int *number_of_maps)
{
    char current_line[MAX_LEN_LINE];
    uint64_t destination_range_start, source_range_start, range_length;
    int i = 0;

    get_line(fp, current_line);
    while (strcmp(current_line, "")) {
        sscanf(current_line, "%lu %lu %lu", &destination_range_start, &source_range_start, &range_length);
        maps[i].destination.start = destination_range_start;
        maps[i].destination.end = destination_range_start + range_length-1;
        maps[i].source.start = source_range_start;
        maps[i].source.end = source_range_start + range_length-1;
        get_line(fp, current_line);
        i++;
        *number_of_maps = i;
    }
}

void propegate_layer(Map* maps, int number_of_maps, uint64_t source, uint64_t *destination)
{
    // printf("\n");
    for (int i = 0; i < number_of_maps; i++) {
        if (source >= maps[i].source.start && source <= maps[i].source.end) {
            *destination = (source - maps[i].source.start) + maps[i].destination.start;
        } else if (*destination == 0) {
            *destination = source;
        }
    }
    // dprintUL(*destination);
}

void creat_sub_ranges(Range *temp_ranges, Range *ranges, int number_of_ranges, Map *maps, int number_of_maps, int *number_of_temp_ranges)
{
    Range current_range, temp_of_temp[MAX_NUM_OF_NUMBERS];
    int j, temp_index = 0;
    uint64_t smalles_index, biggest_index;

    *(number_of_temp_ranges) = 0;

    for (int k = 0; k < number_of_ranges; k++) {
        current_range = ranges[k];
        // PRINT_RANGE(current_range);
        j = 0;
        for (int i = 0; i < number_of_maps; i++) {
            // PRINT_RANGE(maps[i].source);
            if (maps[i].source.start < current_range.start &&
                maps[i].source.end < current_range.start) {
                    ;   /* no match */
            }
            if (maps[i].source.start < current_range.start &&
                maps[i].source.end <= current_range.end &&
                maps[i].source.end >= current_range.start) {
                    ; /* some match on the left */
                    temp_of_temp[j].start = current_range.start;
                    temp_of_temp[j].end = maps[i].source.end;
                    j++;
            }
            if (maps[i].source.start >= current_range.start &&
                maps[i].source.start < current_range.end &&
                maps[i].source.end <= current_range.end) {
                    ; /* all the map inside range */
                    temp_of_temp[j].start = maps[i].source.start;
                    temp_of_temp[j].end = maps[i].source.end;
                    j++;
            }
            if (maps[i].source.start >= current_range.start &&
                maps[i].source.start <= current_range.end &&
                maps[i].source.end > current_range.end) {
                    ; /* some match on the right */
                    temp_of_temp[j].start = maps[i].source.start;
                    temp_of_temp[j].end = current_range.end;
                    j++;
            }
            if (maps[i].source.start > current_range.end &&
                maps[i].source.end > current_range.end) {
                    ;   /* no match */
            }
            if (maps[i].source.start <= current_range.start &&
                maps[i].source.end >= current_range.end) {
                    ; /* all the range inside the map */
                    temp_of_temp[j].start = current_range.start;
                    temp_of_temp[j].end = current_range.end;
                    j++;
            }
        }
        temp_index = j; 

        // printf("\n");
        // dprintINT(temp_index);
        // dprintINT(k);
        qsort_ranges(temp_of_temp, 0, temp_index-1);

        // for (int i = 0; i < temp_index; i++) {
        //     PRINT_RANGE(temp_of_temp[i]);
        // }

        smalles_index = current_range.start;
        biggest_index = current_range.end;
        // dprintINT(smalles_index);
        // dprintINT(biggest_index);

        for (int i = 0; i < temp_index; i++) {
            if (smalles_index < temp_of_temp[i].start &&
                biggest_index >= temp_of_temp[i].end) {
                    temp_ranges[*(number_of_temp_ranges)].start = smalles_index;
                    temp_ranges[*(number_of_temp_ranges)].end = temp_of_temp[i].start-1;
                    (*number_of_temp_ranges)++;

                    smalles_index = temp_of_temp[i].end+1;
            }
            if (smalles_index == temp_of_temp[i].start &&
                biggest_index >= temp_of_temp[i].end) {
                    temp_ranges[(*number_of_temp_ranges)] = temp_of_temp[i];
                    (*number_of_temp_ranges)++;
                    smalles_index = temp_of_temp[i].end+1;
            }
        }
        if (temp_of_temp[temp_index-1].end < biggest_index) {
            temp_ranges[(*number_of_temp_ranges)].start = temp_of_temp[temp_index-1].end+1;
            temp_ranges[(*number_of_temp_ranges)].end = biggest_index;
            (*number_of_temp_ranges)++;
        } else if (temp_index == 0) {
            temp_ranges[(*number_of_temp_ranges)] = current_range;
            (*number_of_temp_ranges)++;
        }
        // for (int i = 0; i < temp_index; i++) {
        //     PRINT_RANGE(temp_of_temp[i]);
        // }
        // printf("\n");
    }

    qsort_ranges(temp_ranges, 0, *number_of_temp_ranges-1);

    // printf("\n");
    // for (int i = 0; i < *number_of_temp_ranges; i++) {
    //     PRINT_RANGE(temp_ranges[i]);
    // }

}

/* qsort: sort v[left]...v[right] int increasing order */
void qsort_ranges(Range *v, int left, int right)
{
    int i, last;

    if (left >= right)                  /* do nothing if array contains */
        return;                         /* fewer than two elements */
    swap_ranges(v, left, (left + right) / 2);  /* move partition elem */
    last = left;                        /* to v[0] */
    for (i = left + 1; i <= right; i++) /* partition */
        if (v[i].start < v[left].start)
            swap_ranges(v, ++last, i);
    swap_ranges(v, left, last); /* restore partition elem */
    qsort_ranges(v, left, last - 1);
    qsort_ranges(v, last + 1, right);
}

/* swap: interchange v[i] and v[j] */
void swap_ranges(Range *v, int i, int j)
{
    Range temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}