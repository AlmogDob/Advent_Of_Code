#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./Almog_String_Manipulation.h"
#include "./Almog_Dynamic_Array.h"


#define points_equal(p1, p2) ((p1).x == (p2).x && (p1).y == (p2).y && (p1).z == (p2).z)

struct Point {
    int x;
    int y;
};

struct Points{
    size_t length;
    size_t capacity;
    struct Point* elements;
};

void point_print(struct Point point)
{
    printf("(%d, %d)\n", point.x, point.y);
}

void points_print(struct Points points)
{
    asm_print_many_times("-", 20);
    for (size_t i = 0; i < points.length; i++) {
        point_print(points.elements[i]);
    }
    asm_print_many_times("-", 20);
}

size_t calc_area(struct Point p1, struct Point p2) {
    size_t diff_x = 0, diff_y = 0;
    if (p1.x > p2.x) {
        diff_x = p1.x - p2.x;
    } else {
        diff_x = p2.x - p1.x;
    }
    if (p1.y > p2.y) {
        diff_y = p1.y - p2.y;
    } else {
        diff_y = p2.y - p1.y;
    }

    return (diff_x + 1) * (diff_y + 1);
}

int main(void)
{
    // FILE *fp = fopen("./test_input.txt", "rt");
    FILE *fp = fopen("./input.txt", "rt");

    char line[ASM_MAX_LEN];
    char word[ASM_MAX_LEN];
	asm_memset(line, '\0', sizeof(line));
    struct Points points = {0};
    ada_init_array(struct Point, points);

    while ((asm_get_line(fp, line)) != -1) {
        struct Point point = {0};
        asm_get_word_and_cut(word, line, ',', 0);
        point.x = asm_str2int(word, NULL, 10);
        point.y = asm_str2int(line, NULL, 10);
        ada_appand(struct Point, points, point);
    }

    // points_print(points);

    size_t largest_area = 0;
    for (size_t i = 0; i < points.length-1; i++) {
        for (size_t j = i; j < points.length; j++) {
            size_t area = calc_area(points.elements[i], points.elements[j]);
            if (area > largest_area) {
                largest_area = area;
            }
        }
    }

    asm_dprintSIZE_T(largest_area);

    free(points.elements);

    return 0;
}

