#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./Almog_String_Manipulation.h"
#include "./Almog_Dynamic_Array.h"

#include "math.h"
#include "stddef.h"

#define points_equal(p1, p2) ((p1).x == (p2).x && (p1).y == (p2).y && (p1).z == (p2).z)

#define NUM_OF_POINTS_IN_CIRCUIT 1500

struct Point {
    int x;
    int y;
    int z;
};

struct Points{
    size_t length;
    size_t capacity;
    struct Point* elements;
};

struct Circuit{
    size_t length;
    struct Point elements[NUM_OF_POINTS_IN_CIRCUIT];
};

struct Circuit_list{
    size_t length;
    size_t capacity;
    struct Circuit* elements;
};

void point_print(struct Point point)
{
    printf("(%d, %d, %d)\n", point.x, point.y, point.z);
}

void circuit_print(struct Circuit circuit, int offset)
{
    for (size_t i = 0; i < circuit.length; i++) {
        printf("%*s", offset, "");
        point_print(circuit.elements[i]);
    }
}

void circuit_list_print(struct Circuit_list circuit_list)
{
    printf("------------------------------\n");
    for (size_t i = 0; i < circuit_list.length; i++) {
        printf("%zu:\n", i);
        circuit_print(circuit_list.elements[i], 4);
    }
    printf("------------------------------\n");
}


bool point_in_circuit(struct Circuit circuit, struct Point point)
{
    for (size_t i = 0; i < circuit.length; i++) {
        if (points_equal(circuit.elements[i], point)) {
            return true;
        }
    }

    return false;
}

int point_in_circuit_list(struct Circuit_list circuit_list, struct Point point)
{
    for (size_t  i = 0; i < circuit_list.length; i++) {
        if (point_in_circuit(circuit_list.elements[i], point)) {
            return (int)i;
        }
    }

    return -1;
}

void point_not_in_circuit_list_print(struct Circuit_list circuit_list, struct Points points)
{
    for (size_t i = 0; i < points.length; i++) {
        if (point_in_circuit_list(circuit_list, points.elements[i]) == -1) {
            point_print(points.elements[i]);
        }
    }
}

double points_dist(struct Point p1, struct Point p2)
{
    size_t x = 0, y = 0, z = 0;
    if (p1.x - p2.x < 0) {
        x = p2.x - p1.x;
    } else {
        x = p1.x - p2.x;
    }
    if (p1.y - p2.y < 0) {
        y = p2.y - p1.y;
    } else {
        y = p1.y - p2.y;
    }
    if (p1.z - p2.z < 0) {
        z = p2.z - p1.z;
    } else {
        z = p1.z - p2.z;
    }
    return sqrt(x * x + y * y + z * z);
}

double closest_points_indexes(struct Points points, double min_min_dist, size_t *p1_index, size_t *p2_index)
{
    double min_dist = __DBL_MAX__;
    for (size_t i = 0; i < points.length-1; i++) {
        for (size_t j = i; j < points.length; j++) {
            double current_dist = points_dist(points.elements[i], points.elements[j]);
            if (current_dist < min_dist && current_dist > min_min_dist) {
                min_dist = current_dist;
                if (p1_index) *p1_index = i;
                if (p2_index) *p2_index = j;
            }
        }
    }

    return min_dist;
}

bool add_point_to_circuit(struct Circuit *circuit, struct Point point)
{
    if (point_in_circuit(*circuit, point)) return false;
    circuit->elements[(circuit->length)++] = point;

    if (circuit->length > NUM_OF_POINTS_IN_CIRCUIT) {
        fprintf(stderr, "%s:%d:\n%s:\n[Error] need to increase 'NUM_OF_POINTS_IN_CIRCUIT (%d).\n\n", __FILE__, __LINE__, __func__, NUM_OF_POINTS_IN_CIRCUIT);
        exit(1);
    }

    return true;
}

void copy_circuit_to_circuit(struct Circuit *circuit1, struct Circuit circuit2)
{
    for (size_t i = 0; i < circuit2.length; i++) {
        if (!point_in_circuit(*circuit1, circuit2.elements[i])) {
            add_point_to_circuit(circuit1, circuit2.elements[i]);
        }
    }
}

void add_points_to_circuit_list(struct Circuit_list *circuit_list, struct Point p1, struct Point p2)
{
    struct Circuit_list temp_list = *circuit_list;

    int circuit_index_p1 = point_in_circuit_list(temp_list, p1);
    int circuit_index_p2 = point_in_circuit_list(temp_list, p2);

    if (circuit_index_p1 > -1 && circuit_index_p2 > -1) {
        if (circuit_index_p1 == circuit_index_p2) {
            return;
        } else {
            copy_circuit_to_circuit(&(temp_list.elements[circuit_index_p1]), temp_list.elements[circuit_index_p2]);
            ada_remove(struct Circuit, temp_list, circuit_index_p2);
        }
    } else if (circuit_index_p1 > -1 && circuit_index_p2 == -1) {
        add_point_to_circuit(&(temp_list.elements[circuit_index_p1]), p2);
    } else if (circuit_index_p2 > -1 && circuit_index_p1 == -1) {
        add_point_to_circuit(&(temp_list.elements[circuit_index_p2]), p1);
    } else if (circuit_index_p1 == -1 && circuit_index_p2 == -1) {
        struct Circuit temp_circuit = {0};
        add_point_to_circuit(&temp_circuit, p1);
        add_point_to_circuit(&temp_circuit, p2);
        ada_appand(struct Circuit, temp_list, temp_circuit);
    }

    *circuit_list = temp_list;
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
    struct Circuit_list circuit_list = {0};
    ada_init_array(struct Circuit, circuit_list);

    while ((asm_get_line(fp, line)) != -1) {
        struct Point point = {0};
        asm_get_word_and_cut(word, line, ',', 0);
        point.x = asm_str2int(word, NULL, 10);
        asm_get_word_and_cut(word, line, ',', 0);
        point.y = asm_str2int(word, NULL, 10);
        point.z = asm_str2int(line, NULL, 10);
        ada_appand(struct Point, points, point);
    }

    size_t p1_index = 0, p2_index = 0;
    double min_dist = closest_points_indexes(points, 0, &p1_index, &p2_index);
    add_points_to_circuit_list(&circuit_list, points.elements[p1_index], points.elements[p2_index]);

    for (int i = 0; i < 1000-1; i++) {
        min_dist = closest_points_indexes(points, min_dist, &p1_index, &p2_index);
        add_points_to_circuit_list(&circuit_list, points.elements[p1_index], points.elements[p2_index]);
    }

    for (size_t count = 0; count < 4; count++) {
        for (size_t i = 0; i < circuit_list.length-1; i++) {
            if (circuit_list.elements[i].length > circuit_list.elements[i+1].length) {
                struct Circuit temp = circuit_list.elements[i];
                circuit_list.elements[i] = circuit_list.elements[i+1];
                circuit_list.elements[i+1] = temp;
            }
        }
    }

    asm_dprintSIZE_T(circuit_list.elements[circuit_list.length-1].length * circuit_list.elements[circuit_list.length-2].length * circuit_list.elements[circuit_list.length-3].length);

    free(points.elements);
    free(circuit_list.elements);


    return 0;
}

