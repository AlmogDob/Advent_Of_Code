#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./Almog_String_Manipulation.h"
#include "./Almog_Dynamic_Array.h"

struct range {
    size_t lower;
    size_t upper;
};

struct ranges {
    size_t length;
    size_t capacity;
    struct range* elements;
};

enum intersection {
    NO_INTERSECTION,
    UPPER_IN,
    LOWER_IN,
    INCLUDE_1_IN_2,
    INCLUDE_2_IN_1,
    NUM_OF_INTERSECTION,
};

void print_intersection(enum intersection category)
{
    switch (category) {
    case NO_INTERSECTION:
        printf("no_intersection\n");
        break;
    case UPPER_IN:
        printf("upper_in\n");
        break;
    case LOWER_IN:
        printf("lower_in\n");
        break;
    case INCLUDE_2_IN_1:
        printf("2_in_1\n");
        break;
    case INCLUDE_1_IN_2:
        printf("1_in_2\n");
        break;
    default:
        break;
    }
}

enum intersection categorize_intersection(struct range range1, struct range range2)
{
    // No overlap at all
    if (range2.upper < range1.lower || range2.lower > range1.upper) {
        return NO_INTERSECTION;
    }

    // range2 completely inside range1
    if (range2.lower >= range1.lower && range2.upper <= range1.upper) {
        return INCLUDE_2_IN_1;
    }

    // range1 completely inside range2
    if (range1.lower >= range2.lower && range1.upper <= range2.upper) {
        return INCLUDE_1_IN_2;
    }

    // range2 starts inside range1 and extends above it
    if (range2.lower >= range1.lower && range2.lower <= range1.upper &&
        range2.upper > range1.upper) {
        return UPPER_IN;
    }

    // range2 ends inside range1 and starts below it
    if (range2.upper >= range1.lower && range2.upper <= range1.upper &&
        range2.lower < range1.lower) {
        return LOWER_IN;
    }

    return NUM_OF_INTERSECTION;
}

void print_range(struct range range)
{
    printf("[%zu -> %zu]\n", range.lower, range.upper);
}

void print_list(struct ranges list)
{
    for (size_t i = 0; i < list.length; i++) {
        printf("%zu: ", i);
        print_range(list.elements[i]);
    }
}

bool range_valid(struct range range)
{
    return range.lower <= range.upper;
}

void remove_invalid_ranges(struct ranges *list)
{
    struct ranges temp_list = *list;

    for (int j = 0; j < (int)temp_list.length; j++) {
        if (!range_valid(temp_list.elements[j])) {
            ada_remove(struct range, temp_list, j);
            j--;
        }
    }

    *list = temp_list;
}

void fix_intersection(struct range *range1, struct range *range2)
{
    enum intersection category = categorize_intersection(*range1, *range2);
    size_t temp;
    switch (category) {
    case NO_INTERSECTION:
        break;
    case UPPER_IN:
        range2->lower = range1->upper+1;
        break;
    case LOWER_IN:
        range2->upper = range1->lower-1;
        break;
    case INCLUDE_2_IN_1:
        range2->upper = 1;
        range2->lower = 2;
        break;
    case INCLUDE_1_IN_2:
        range1->upper = 1;
        range1->lower = 2;
        break;
    default:
        break;
    }
}

void sort_ranges(struct ranges *list)
{
    for (size_t i = 0; i < list->length-1; i++) {
        for (size_t j = i+1; j < list->length; j++) {
            if (list->elements[i].lower > list->elements[j].lower) {
                struct range temp = list->elements[i];
                list->elements[i] = list->elements[j];
                list->elements[j] = temp;
            }
        }
    }
}

void fix_ranges(struct ranges *list)
{
    struct ranges temp_list = *list;

    sort_ranges(&temp_list);

    for (int i = 0; i < (int)temp_list.length - 1; i++) {
        for (int j = i+1; j < (int)temp_list.length; j++) {
            fix_intersection(&(temp_list.elements[i]), &(temp_list.elements[j]));
            if (!range_valid(temp_list.elements[j])) {
                ada_remove(struct range, temp_list, j);
                j--;
            }
            if (!range_valid(temp_list.elements[i])) {
                ada_remove(struct range, temp_list, i);
                i--;
                break;
            }
        }
    }


    *list = temp_list;
}

int main(void)
{
    // FILE *fp = fopen("./test_input.txt", "rt");
    FILE *fp = fopen("./input.txt", "rt");

    char line[ASM_MAX_LEN] = {'\0'};
    char word[ASM_MAX_LEN] = {'\0'};

    struct ranges list = {0};
    ada_init_array(struct range, list);

    int len = 0;

    size_t count = 0;
    while ((len = asm_get_line(fp, line)) != -1) {
        if (len == 0) {
            break;
        }
        asm_get_word_and_cut(word, line, '-', 0);
        struct range temp = {.lower = asm_str2size_t(word, NULL, 10),
                                .upper = asm_str2size_t(line, NULL, 10)};
        if (range_valid(temp)) {
            ada_appand(struct range, list, temp);
        }
    }
    for (int i = 0; i < 20; i++) {
        fix_ranges(&list);
    }
    for (size_t i = 0; i < list.length; i++) {
        if (list.elements[i].upper > list.elements[i].lower) {
            size_t diff = list.elements[i].upper - list.elements[i].lower;
            count += diff+1;
        } if (list.elements[i].upper == list.elements[i].lower) {
            count++;
        }
    }

    asm_dprintSIZE_T(count);

    return 0;
}

