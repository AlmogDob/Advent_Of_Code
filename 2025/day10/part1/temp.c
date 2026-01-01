#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./Almog_String_Manipulation.h"
#include "./Almog_Dynamic_Array.h"

#define MAX_LIGHTS_NUM 20
#define MAX_DEPTH 7

struct Light_Diagram {
    size_t length;
    bool lights[MAX_LIGHTS_NUM];
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
    struct Light_Diagram ld;
    struct BW_List bwl;
};

struct Machines{
  size_t length;
  size_t capacity;
  struct Machine *elements;
};

void lights_print(struct Light_Diagram ld)
{
    for (size_t i = 0; i < ld.length; i++) {
        printf("%d", ld.lights[i]);
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
    lights_print(m.ld);
    bw_list_print(m.bwl);
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

bool lds_equal(struct Light_Diagram ld1, struct Light_Diagram ld2)
{
    if (ld1.length != ld2.length) return false;
    for (size_t i = 0; i < ld1.length; i++) {
        if (ld1.lights[i] != ld2.lights[i]) return false;
    }
    return true;
}

struct Light_Diagram apply_bw_on_light_diagram(struct Light_Diagram ld, struct BW bw)
{
    struct Light_Diagram res = ld;
    for (size_t i = 0; i < bw.length; i++) {
        int light_num = bw.buttons[i];
        res.lights[light_num] = res.lights[light_num] ? false : true;
    }

    return res;
}

int apply_bwl_on_ld(struct Light_Diagram target_ld, struct Light_Diagram current_ld, struct BW_List bwl, int current_depth)
{
    if (current_depth >= MAX_DEPTH) return MAX_DEPTH;
    if (lds_equal(target_ld, current_ld)) {
        return current_depth;
    }

    int min_good_depth = MAX_DEPTH;
    for (size_t i = 0; i < bwl.length; i++) {
        int current_good_depth = apply_bwl_on_ld(target_ld, apply_bw_on_light_diagram(current_ld, bwl.list[i]), bwl, current_depth+1);
        if (current_good_depth < min_good_depth) {
            min_good_depth = current_good_depth;
        }
    }

    return min_good_depth;
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

        asm_get_word_and_cut(word, line, '[', 0);
        asm_get_word_and_cut(word, line, ']', 0);

        machine.ld.length = asm_length(word);
        for (size_t i = 0; i < machine.ld.length; i++) {
            if (word[i] == '.') machine.ld.lights[i] = 0;
            if (word[i] == '#') machine.ld.lights[i] = 1;
        }

        int count = 0;
        while ((count = asm_str_in_str(line, "(")) > 0) {
            asm_get_word_and_cut(word, line, '(', 0);
            asm_get_word_and_cut(word, line, ')', 0);
            struct BW bw = parse_word_to_bw(word);
            machine.bwl.list[machine.bwl.length++] = bw;
        }

        ada_appand(struct Machine, machines, machine);
    }

    // machines_print(machines);

    size_t sum = 0;
    for (size_t i = 0; i < machines.length; i++) {
        printf("%zu/%zu\n", i, machines.length);
        int current_res = apply_bwl_on_ld(machines.elements[i].ld,(struct Light_Diagram){.length = machines.elements[i].ld.length, .lights = {0}}, machines.elements[i].bwl, 0);
        // asm_dprintINT(current_res);
        sum += current_res;
    }

    asm_dprintSIZE_T(sum);





    free(machines.elements);

    return 0;
}

