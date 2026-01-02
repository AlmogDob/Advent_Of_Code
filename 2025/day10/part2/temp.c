#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./Almog_String_Manipulation.h"
#include "./Almog_Dynamic_Array.h"
#define MATRIX2D_IMPLEMENTATION
#include "./Matrix2D.h"

#define MAX_LIGHTS_NUM 20
#define MAX_DEPTH 15

struct Req {
    size_t length;
    int list[MAX_LIGHTS_NUM];
};

struct BW {
    size_t length;
    int buttons[MAX_LIGHTS_NUM];
};

struct BW_List {
    size_t length;
    struct BW list[MAX_LIGHTS_NUM * 2];
};

struct Machine {
    struct Req req;
    struct BW_List bwl;
};

struct Machines{
  size_t length;
  size_t capacity;
  struct Machine *elements;
};

void req_print(struct Req req)
{
    for (size_t i = 0; i < req.length; i++) {
        printf("%d ", req.list[i]);
    }
    printf("\n");
}

void bw_print(struct BW bw)
{
    printf("(");
    for (size_t i = 0; i < bw.length; i++) {
        printf("%d,", bw.buttons[i]);
    }
    printf("\b)");
}

void bw_list_print(struct BW_List bwl)
{
    for (size_t i = 0; i < bwl.length; i++) {
        bw_print(bwl.list[i]);
        printf(" ");
    }
    printf("\n");
}

void machine_print(struct Machine m)
{
    bw_list_print(m.bwl);
    req_print(m.req);
}

void machines_print(struct Machines machines)
{
    for (size_t i = 0; i < machines.length; i++) {
        machine_print(machines.elements[i]);
        asm_print_many_times("-", 20);
    }
    printf("\n");
}

struct BW parse_word_to_bw(char *word)
{
    ADA_ASSERT(asm_length(word) > 0);

    char temp_num[ASM_MAX_LEN];
    struct BW bw = {0};

    while (asm_get_word_and_cut(temp_num, word, ',', 0)) {
        bw.buttons[bw.length++] = asm_str2int(temp_num, NULL, 10);
    }

    return bw;
}

struct BW update_bw(struct Req req, struct BW bw)
{
    struct BW new_bw = {0};
    for (size_t i = 0; i < req.length; i++) {
        new_bw.buttons[new_bw.length++] = 0;
    }
    for (size_t i = 0; i < bw.length; i++) {
        new_bw.buttons[bw.buttons[i]] = 1;
    }
    return new_bw;
}

void update_machines_bwl(struct Machine *m)
{
    for (size_t i = 0; i < m->bwl.length; i++) {
        m->bwl.list[i] = update_bw(m->req, m->bwl.list[i]);
    }
}

/* A|b */
void gauss_elimination(Mat2D m)
{
    size_t size = (size_t)fmin(m.rows, m.cols-1);
    for (size_t i = 0; i < size; i++) {
        if (!MAT2D_AT(m, i, i)) {   /* swapping only if it is zero */
            /* finding biggest first number (absolute value) */
            size_t biggest_r = i;
            for (size_t index = i; index < m.rows; index++) {
                if (fabs(MAT2D_AT(m, index, i)) > fabs(MAT2D_AT(m, biggest_r, i))) {
                    biggest_r = index;
                }
            }
            if (i != biggest_r) { /* i = biggest_r if all the column is zero */
                mat2D_swap_rows(m, i, biggest_r);
            }
        }
        for (size_t j = i+1; j < size; j++) {
            double factor = 1 / MAT2D_AT(m, i, i);
            double mat_value = MAT2D_AT(m, j, i);
            mat2D_sub_row_time_factor_to_row(m, j, i, mat_value * factor);
        }
    }
    if (MAT2D_AT(m, size-1, size-1)) {
        double factor = 1 / MAT2D_AT(m, size-1, size-1);
        mat2D_mult_row(m, size-1, factor);
        for (size_t c = size-1; c > 0; c--) {
            if (!MAT2D_AT(m, c, c)) continue;
            double factor = 1 / MAT2D_AT(m, c, c);
            for (int r = c-1; r >= 0; r--) {
                double mat_value = MAT2D_AT(m, r, c);
                mat2D_sub_row_time_factor_to_row(m, r, c, mat_value * factor);
            }
        }
    } else {
        if (size >= m.cols-1) {
            for (size_t c = size-2; c > 0; c--) {
                if (!MAT2D_AT(m, c, c)) continue;
                double factor = 1 / MAT2D_AT(m, c, c);
                for (int r = c-1; r >= 0; r--) {
                    double mat_value = MAT2D_AT(m, r, c);
                    mat2D_sub_row_time_factor_to_row(m, r, c, mat_value * factor);
                }
            }
        } else {
            double factor = 0;
            for (size_t j = size; j < m.cols-1; j++) {
                if (MAT2D_AT(m, size-1, j)) {
                    factor = 1 / MAT2D_AT(m, size-1, j);
                    break;
                }
            }
            if (!factor) {
                for (size_t c = size-2; c > 0; c--) {
                    if (!MAT2D_AT(m, c, c)) continue;
                    double factor = 1 / MAT2D_AT(m, c, c);
                    for (int r = c-1; r >= 0; r--) {
                        double mat_value = MAT2D_AT(m, r, c);
                        mat2D_sub_row_time_factor_to_row(m, r, c, mat_value * factor);
                    }
                }
            } else {
                mat2D_mult_row(m, size-1, factor);
                for (size_t c = size-1; c > 0; c--) {
                    if (c != size-1) {
                        if (!MAT2D_AT(m, c, c)) {
                            continue;
                        } else {
                            factor = 1 / MAT2D_AT(m, c, c);
                        }
                    }
                    for (int r = c-1; r >= 0; r--) {
                        double mat_value = MAT2D_AT(m, r, c);
                        mat2D_sub_row_time_factor_to_row(m, r, c, mat_value * factor);
                    }
                }
            }
        }
    }
}

int main(void)
{
    // FILE *fp = fopen("./test_input.txt", "rt");
    FILE *fp = fopen("./input.txt", "rt");

    char line[ASM_MAX_LEN];
    char word[ASM_MAX_LEN];
	asm_memset(line, '\0', sizeof(line));

    struct Machines machines = {0};
    ada_init_array(struct Machine, machines);

    while ((asm_get_line(fp, line)) != -1) {
        // asm_dprintSTRING(line);
        struct Machine machine = {0};

        int count = 0;
        while ((count = asm_str_in_str(line, "(")) > 0) {
            asm_get_word_and_cut(word, line, '(', 0);
            asm_get_word_and_cut(word, line, ')', 0);
            struct BW bw = parse_word_to_bw(word);
            machine.bwl.list[machine.bwl.length++] = bw;
        }

        asm_get_word_and_cut(word, line, '{', 0);
        asm_get_word_and_cut(word, line, '}', 0);
        struct BW ld = parse_word_to_bw(word);
        for (size_t i = 0; i < ld.length; i++) {
            machine.req.list[machine.req.length++] = ld.buttons[i];
        }

        ada_appand(struct Machine, machines, machine);
    }

    // machines_print(machines);

    for (size_t i = 0; i < machines.length; i++) {
        update_machines_bwl(&(machines.elements[i]));
    }

    // machines_print(machines);

    struct Machine m = machines.elements[20];
    
    machine_print(m);

    Mat2D a = mat2D_alloc(m.req.length, m.bwl.length+1);
    mat2D_fill(a, 0);
    for (size_t j = 0; j < m.bwl.length; j++) {
        struct BW bw = m.bwl.list[j];
        for (size_t i = 0; i < bw.length; i++) {
            MAT2D_AT(a, i, j) = bw.buttons[i];
        }
    }
    for (size_t i = 0, j = a.cols-1; i < a.rows; i++) {
        MAT2D_AT(a, i, j) = m.req.list[i];
    }
    
    gauss_elimination(a);



    MAT2D_PRINT(a);







    mat2D_free(a);
    free(machines.elements);

    return 0;
}

