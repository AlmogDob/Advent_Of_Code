#define ALMOG_STRING_MANIPULATION_IMPLEMENTATION
#include "./Almog_String_Manipulation.h"
#include "./Almog_Dynamic_Array.h"

#define INF 100000

#define points_equal(p1, p2) ((p1).x == (p2).x && (p1).y == (p2).y)
#define edges_equal(e1, e2) (points_equal(e1.p1, e2.p1) && points_equal(e1.p2, e2.p2))

struct Point {
    int x;
    int y;
};

struct Points {
    size_t length;
    size_t capacity;
    struct Point* elements;
};

struct Edge {
    struct Point p1;
    struct Point p2;
};

struct Edges {
    size_t length;
    size_t capacity;
    struct Edge* elements;
};

void point_print(struct Point point)
{
    printf("(%d, %d)\n", point.x, point.y);
}

void edge_print(struct Edge edge)
{
    printf("(%d, %d) -> (%d, %d)\n", edge.p1.x, edge.p1.y, edge.p2.x, edge.p2.y);
}

void points_print(struct Points points)
{
    asm_print_many_times("-", 20);
    for (size_t i = 0; i < points.length; i++) {
        point_print(points.elements[i]);
    }
    asm_print_many_times("-", 20);
}

void edges_print(struct Edges edges)
{
    asm_print_many_times("-", 20);
    for (size_t i = 0; i < edges.length; i++) {
        edge_print(edges.elements[i]);
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

bool point_in_points(struct Points points, struct Point p)
{
    for (size_t i = 0; i < points.length; i++) {
        if (points_equal(points.elements[i], p)) {
            return true;
        }
    }

    return false;
}

int orient(struct Point a, struct Point b, struct Point c) {
    long long v =
        (long long)(b.x - a.x) * (long long)(c.y - a.y) -
        (long long)(b.y - a.y) * (long long)(c.x - a.x);

    if (v == 0) return 0;
    return (v > 0) ? 1 : -1;
}

bool on_segment(struct Point a, struct Point b, struct Point p) {

    if (orient(a, b, p) != 0) return false; // must be collinear

    int min_x = (a.x < b.x) ? a.x : b.x;
    int max_x = (a.x > b.x) ? a.x : b.x;
    int min_y = (a.y < b.y) ? a.y : b.y;
    int max_y = (a.y > b.y) ? a.y : b.y;

    return p.x >= min_x && p.x <= max_x && p.y >= min_y && p.y <= max_y;
}

/* Returns true if the two line segments intersect (including touching / overlapping). */
int edges_intersect(struct Edge e1, struct Edge e2) {
    struct Point a = e1.p1;
    struct Point b = e1.p2;
    struct Point c = e2.p1;
    struct Point d = e2.p2;

    int o1 = orient(a, b, c);
    int o2 = orient(a, b, d);
    int o3 = orient(c, d, a);
    int o4 = orient(c, d, b);

    /* General case: proper intersection */
    if (o1 != o2 && o3 != o4) {
        return 1;
    }

    /* Special cases: collinear + touching/overlap */
    if ((o1 == 0 || o2 == 0) && (on_segment(a, b, c) || on_segment(a, b, d))) return 3; // c or d on ab
    if ((o3 == 0 || o4 == 0) && (on_segment(c, d, a) || on_segment(c, d, b))) return 3; // a or b on cd
    if (o1 == 0 && on_segment(a, b, c)) return 2; // c on ab
    if (o2 == 0 && on_segment(a, b, d)) return 2; // d on ab
    if (o3 == 0 && on_segment(c, d, a)) return 2; // a on cd
    if (o4 == 0 && on_segment(c, d, b)) return 2; // b on cd

    return 0;
}

bool point_on_poly(struct Edges poly, struct Point p)
{
    for (size_t i = 0; i < poly.length; i++) {
        if (on_segment(poly.elements[i].p1, poly.elements[i].p2, p)) {
            return true;
        }
    }
    return false;
}

bool point_in_on_poly(struct Edges poly, struct Point p)
{
    bool inside = false;

    if (point_on_poly(poly, p)) return true;

    for (size_t i = 0; i < poly.length; i++) {
        struct Point a = poly.elements[i].p1;
        struct Point b = poly.elements[i].p2;

        // half-open straddle to avoid double counting vertices
        bool above_a = a.y > p.y;
        bool above_b = b.y > p.y;
        if (above_a == above_b) continue;

        long long dx = (long long)b.x - a.x;
        long long dy = (long long)b.y - a.y;

        // Compare p.x with intersection x without floating point
        long long lhs = ((long long)p.x - a.x) * dy;
        long long rhs = dx * ((long long)p.y - a.y);

        if (dy > 0) {
            if (lhs < rhs) inside = !inside;
        } else {
            if (lhs > rhs) inside = !inside;
        }
    }

    return inside;
}

bool edge_is_part_of_poly(struct Edges edges, struct Edge edge)
{
    for (size_t i = 0; i < edges.length; i++) {
        if (edges_equal(edge, edges.elements[i])) {
            return true;
        }
    }

    return false;
}

int edge_intersect_poly(struct Edges edges, struct Edge edge)
{
    int count = 0; 
    for (size_t i = 0; i < edges.length; i++) {
        int res = edges_intersect(edge, edges.elements[i]);
        if (res) {
            count++;
        }
    }

    return count;
}

bool check_valid_rect(struct Edges edges, struct Point p1, struct Point p2)
{
    size_t min_x = (p1.x < p2.x) ? p1.x : p2.x;
    size_t max_x = (p1.x > p2.x) ? p1.x : p2.x;
    size_t min_y = (p1.y < p2.y) ? p1.y : p2.y;
    size_t max_y = (p1.y > p2.y) ? p1.y : p2.y;

    struct Point point = {0};

    point.x = min_x;
    point.y = min_y;
    if (!point_in_on_poly(edges, point)) return false;
    point.x = max_x;
    point.y = min_y;
    if (!point_in_on_poly(edges, point)) return false;
    point.x = max_x;
    point.y = max_y;
    if (!point_in_on_poly(edges, point)) return false;
    point.x = min_x;
    point.y = max_y;
    if (!point_in_on_poly(edges, point)) return false;

    for (size_t x = min_x; x <= max_x; x++) {
        for (size_t y = min_y; y <= min_y; y++) {
            point.x = x;
            point.y = y;
            if (!point_in_on_poly(edges, point)) return false;
        }
    }
    for (size_t x = min_x; x <= max_x; x++) {
        for (size_t y = max_y; y <= max_y; y++) {
            point.x = x;
            point.y = y;
            if (!point_in_on_poly(edges, point)) return false;
        }
    }
    for (size_t x = min_x; x <= min_x; x++) {
        for (size_t y = min_y; y <= max_y; y++) {
            point.x = x;
            point.y = y;
            if (!point_in_on_poly(edges, point)) return false;
        }
    }
    for (size_t x = max_x; x <= max_x; x++) {
        for (size_t y = min_y; y <= max_y; y++) {
            point.x = x;
            point.y = y;
            if (!point_in_on_poly(edges, point)) return false;
        }
    }

    return true;
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
    struct Edges edges = {0};
    ada_init_array(struct Edge, edges);

    while ((asm_get_line(fp, line)) != -1) {
        struct Point point = {0};
        asm_get_word_and_cut(word, line, ',', 0);
        point.x = asm_str2int(word, NULL, 10);
        point.y = asm_str2int(line, NULL, 10);
        ada_appand(struct Point, points, point);
    }

    for (size_t i = 0; i < points.length; i++) {
        struct Edge edge = {.p1 = points.elements[i],
                            .p2 = points.elements[(i+1) % points.length]};
        ada_appand(struct Edge, edges, edge);
    }

    points_print(points);
    edges_print(edges);

    struct Point p1 = {0}, p2 = {0};
    size_t largest_area = 0;
    for (size_t i = 0; i < points.length-1; i++) {
        asm_dprintSIZE_T(i);
        for (size_t j = i+1; j < points.length; j++) {
            // point_print(points.elements[i]);
            // point_print(points.elements[j]);
            // asm_dprintSIZE_T(area);
            // asm_print_many_times("-", 10);

            if (check_valid_rect(edges, points.elements[i], points.elements[j])) {
                size_t area = calc_area(points.elements[i], points.elements[j]);
                if (area > largest_area) {
                    largest_area = area;
                    p1 = points.elements[i];
                    p2 = points.elements[j];
                }
            }
        }
    }

    point_print(p1);
    point_print(p2);
    asm_dprintSIZE_T(largest_area);

    free(points.elements);
    free(edges.elements);

    return 0;
}

