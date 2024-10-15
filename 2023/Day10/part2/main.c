#include <string.h>
#define ALMOG_AOC_IMPLEMENTATION
#include "Almog_AoC.h"
#include <stdbool.h>

typedef enum {
    GROUND = 0,
    NORTH_SOUTH,
    EAST_WEST,
    NORTH_EAST,
    NORTH_WEST,
    SOUTH_WEST,
    SOUTH_EAST,
    START
} Symbol_type;

typedef enum {
    NORTH = 1,
    EAST,
    SOUTH,
    WEST
} Directions;

typedef struct {
    char symbol;
    Symbol_type type;
    Directions tile_enter;
    bool north;
    bool south;
    bool east;
    bool west;
    bool loop;
    bool dead_end;
} Tile;

typedef struct {
    size_t length;
    size_t capacity;
    Tile* elements;
} ada_Tile_array;

int offset2D(size_t x, size_t y, size_t nx);
void print_symbol_type(Symbol_type st);
void print_direction(Directions dir);
void print_tile(Tile tile);
void print_tile_array(ada_Tile_array *a);
void print_tile_array_skinny(ada_Tile_array *a);
void print_loop_skinny(ada_Tile_array *a);
void set_type(Tile *tile);
void set_conaction(ada_Tile_array *map, size_t x, size_t y);
void set_map_conaction(ada_Tile_array *map);
void init_map(ada_Tile_array *map);
void find_start(size_t *x, size_t *y, ada_Tile_array *map);
void next_xy(size_t *next_x, size_t *next_y, ada_Tile_array *map, size_t x, size_t y);

size_t nx, ny;

int main(int argc, char const *argv[])
{
    char input_dir[MAXDIR];
    FILE *fp;

    if (--argc != 1) {
        fprintf(stderr, "%s:%d: [ERROR]: not right usage\n", __FILE__, __LINE__);
        return -1;
    }
    strncpy(input_dir, (*(++argv)), MAXDIR);

    fp = fopen(input_dir, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: couldn't find file path\n");
        return -1;
    }

/*------------------------------------------------------------*/
    int line_len, first_line = 1;
    char current_line[MAX_LEN_LINE];
    ada_Tile_array map, loop;
    Tile current_tile;

    ada_array(Tile, map);
    ada_array(Tile, loop);

    while ((line_len = aaoc_get_line(fp, current_line)) != -1) {
        // dprintSTRING(current_line);
        ny++;
        for (int i = 0; i < aaoc_length(current_line); i++) {
            current_tile.symbol = current_line[i];
            ada_appand(Tile, map, current_tile);
            if (first_line) {
                nx++;
            }
        }
        first_line = 0;
    }

    init_map(&map);
    set_map_conaction(&map);
    print_tile_array_skinny(&map);

    size_t sx, sy, next_x, next_y;
    find_start(&sx, &sy, &map);
    ada_appand(Tile, loop, map.elements[offset2D(sx, sy, nx)]);
    next_xy(&next_x, &next_y, &map, sx, sy);
    while ((map.elements[offset2D(next_x, next_y, nx)].symbol != 'S')) {
        ada_appand(Tile, loop, map.elements[offset2D(next_x, next_y, nx)]);
        next_xy(&next_x, &next_y, &map, next_x, next_y);
    }
    print_loop_skinny(&map);
    
    print_tile_array(&loop);

    dprintINT((int)((loop.length)/2));

    return 0;
}

int offset2D(size_t x, size_t y, size_t nx)
{
    return x + y * nx;
}

void print_symbol_type(Symbol_type st)
{
    switch (st) {
    case NORTH_SOUTH:
        printf("NORTH_SOUTH\n");
        break;
    case EAST_WEST:
        printf("EAST_WEST\n");
        break;
    case NORTH_EAST:
        printf("NORTH_EAST\n");
        break;
    case NORTH_WEST:
        printf("NORTH_WEST\n");
        break;
    case SOUTH_WEST:
        printf("SOUTH_WEST\n");
        break;
    case SOUTH_EAST:
        printf("SOUTH_EAST\n");
        break;
    case GROUND:
        printf("GROUND\n");
        break;
    case START:
        printf("START\n");
        break;
    default:
        break;
    }
}

void print_direction(Directions dir)
{
    switch (dir) {
    case NORTH:
        printf("NORTH\n");
        break;
    case EAST:
        printf("EAST\n");
        break;
    case SOUTH:
        printf("SOUTH\n");
        break;
    case WEST:
        printf("WEST\n");
        break;
    default:
        break;
    }
}

void print_tile(Tile tile)
{
    printf("------------\n");
    printf("~%c~\n", tile.symbol);
    print_symbol_type(tile.type);
    printf(" %d\n", tile.north);
    printf("%d %d\n", tile.west, tile.east);
    printf(" %d\n", tile.south);
    if (tile.dead_end) {
        printf("dead end\n");
    }
    printf("tile enter: ");
    print_direction(tile.tile_enter);
    if (tile.loop) {
        printf("loop\n");
    }
    printf("------------\n");
}

void print_tile_array(ada_Tile_array *a)
{
    if (a->length == nx * ny) {
        for (size_t y = 0; y < ny; y++) {
            for (size_t x = 0; x < nx; x++) {
                printf("   ~%zu,%zu~\n", y, x);
                print_tile(a->elements[offset2D(x, y, nx)]);
                printf("\n");
            }
        }
    } else {
        for (size_t i = 0; i < a->length; i++) {
            printf("   ~%zu~\n",i);
            print_tile(a->elements[i]);
            printf("\n");
        }
    }
}

void print_tile_array_skinny(ada_Tile_array *a)
{
    printf("\n");
    for (size_t y = 0; y < ny; y++) {
        for (size_t x = 0; x < nx; x++) {
            printf("%c", a->elements[offset2D(x, y, nx)].symbol);
        }
        printf("\n");
    }
    printf("\n");
}

void print_loop_skinny(ada_Tile_array *a)
{
    printf("\n");
    for (size_t y = 0; y < ny; y++) {
        for (size_t x = 0; x < nx; x++) {
            if (a->elements[offset2D(x, y, nx)].loop) {
                printf("%c", a->elements[offset2D(x, y, nx)].symbol);
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("\n");

}

void set_type(Tile *tile)
{
    switch (tile->symbol) {
    case '|':
        tile->type = NORTH_SOUTH;
        break;
    case '-':
        tile->type = EAST_WEST;
        break;
    case 'L':
        tile->type = NORTH_EAST;
        break;
    case 'J':
        tile->type = NORTH_WEST;
        break;
    case '7':
        tile->type = SOUTH_WEST;
        break;
    case 'F':
        tile->type = SOUTH_EAST;
        break;
    case '.':
        tile->type = GROUND;
        break;
    case 'S':
        tile->type = START;
        break;
    default:
        break;
    }
}

void set_conaction(ada_Tile_array *map, size_t x, size_t y)
{
    if (x == 0 && y == 0) {
        /* x...
           ....
           ....
           .... */
        switch (map->elements[offset2D(x, y, nx)].type) {
        case NORTH_SOUTH:
            map->elements[offset2D(x, y+1, nx)].north = true;
            break;
        case EAST_WEST:
            map->elements[offset2D(x+1, y, nx)].west = true;
            break;
        case NORTH_EAST:
            map->elements[offset2D(x+1, y, nx)].west = true;
            break;
        case SOUTH_WEST:
            map->elements[offset2D(x, y+1, nx)].north = true;
            break;
        case SOUTH_EAST:
            map->elements[offset2D(x, y+1, nx)].north = true;
            map->elements[offset2D(x+1, y, nx)].west = true;
            break;
        default:
            break;
        }
    } else if (x == 0 && y == ny-1) {
        /* @...
           ....
           ....
           x... */
        switch (map->elements[offset2D(x, y, nx)].type) {
        case NORTH_SOUTH:
            map->elements[offset2D(x, y-1, nx)].south = true;
            break;
        case EAST_WEST:
            map->elements[offset2D(x+1, y, nx)].west = true;
            break;
        case NORTH_EAST:
            map->elements[offset2D(x, y-1, nx)].south = true;
            map->elements[offset2D(x+1, y, nx)].west = true;
            break;
        case NORTH_WEST:
            map->elements[offset2D(x, y-1, nx)].south = true;
            break;
        case SOUTH_EAST:
            map->elements[offset2D(x+1, y, nx)].west = true;
            break;
        default:
            break;
        }
    } else if (x == 0 && y != 0 && y != ny-1) {
        /* @...
           x...
           x...
           @... */
        switch (map->elements[offset2D(x, y, nx)].type) {
        case NORTH_SOUTH:
            map->elements[offset2D(x, y-1, nx)].south = true;
            map->elements[offset2D(x, y+1, nx)].north = true;
            break;
        case EAST_WEST:
            map->elements[offset2D(x+1, y, nx)].west = true;
            break;
        case NORTH_EAST:
            map->elements[offset2D(x, y-1, nx)].south = true;
            map->elements[offset2D(x+1, y, nx)].west = true;
            break;
        case NORTH_WEST:
            map->elements[offset2D(x, y-1, nx)].south = true;
            break;
        case SOUTH_WEST:
            map->elements[offset2D(x, y+1, nx)].north = true;
            break;
        case SOUTH_EAST:
            map->elements[offset2D(x, y+1, nx)].north = true;
            map->elements[offset2D(x+1, y, nx)].west = true;
            break;
        default:
            break;
        }
    } else if (x == nx-1 && y == 0) {
        /* @..x
           @...
           @...
           @... */
        switch (map->elements[offset2D(x, y, nx)].type) {
        case NORTH_SOUTH:
            map->elements[offset2D(x, y+1, nx)].north = true;
            break;
        case EAST_WEST:
            map->elements[offset2D(x-1, y, nx)].east = true;
            break;
        case NORTH_WEST:
            map->elements[offset2D(x-1, y, nx)].east = true;
            break;
        case SOUTH_WEST:
            map->elements[offset2D(x, y+1, nx)].north = true;
            map->elements[offset2D(x-1, y, nx)].east = true;
            break;
        case SOUTH_EAST:
            map->elements[offset2D(x, y+1, nx)].north = true;
            break;
        default:
            break;
        }
    } else if (x == nx-1 && y == ny-1) {
        /* @..@
           @...
           @...
           @..x */
        switch (map->elements[offset2D(x, y, nx)].type) {
        case NORTH_SOUTH:
            map->elements[offset2D(x, y-1, nx)].south = true;
            break;
        case EAST_WEST:
            map->elements[offset2D(x-1, y, nx)].east = true;
            break;
        case NORTH_EAST:
            map->elements[offset2D(x, y-1, nx)].south = true;
            break;
        case NORTH_WEST:
            map->elements[offset2D(x, y-1, nx)].south = true;
            map->elements[offset2D(x-1, y, nx)].east = true;
            break;
        case SOUTH_WEST:
            map->elements[offset2D(x-1, y, nx)].east = true;
            break;
        default:
            break;
        }
    } else if (x == nx-1 && y != 0 && y != ny-1) {
        /* @..@
           @..x
           @..x
           @..@ */
        switch (map->elements[offset2D(x, y, nx)].type) {
        case NORTH_SOUTH:
            map->elements[offset2D(x, y-1, nx)].south = true;
            map->elements[offset2D(x, y+1, nx)].north = true;
            break;
        case EAST_WEST:
            map->elements[offset2D(x-1, y, nx)].east = true;
            break;
        case NORTH_EAST:
            map->elements[offset2D(x, y-1, nx)].south = true;
            break;
        case NORTH_WEST:
            map->elements[offset2D(x, y-1, nx)].south = true;
            map->elements[offset2D(x-1, y, nx)].east = true;
            break;
        case SOUTH_WEST:
            map->elements[offset2D(x-1, y, nx)].east = true;
            map->elements[offset2D(x, y+1, nx)].north = true;
            break;
        case SOUTH_EAST:
            map->elements[offset2D(x, y+1, nx)].north = true;
        default:
            break;
        }
    } else if (x != 0 && x != nx-1 && y == 0) {
        /* @xx@
           @..@
           @..@
           @..@ */
        switch (map->elements[offset2D(x, y, nx)].type) {
        case NORTH_SOUTH:
            map->elements[offset2D(x, y+1, nx)].north = true;
            break;
        case EAST_WEST:
            map->elements[offset2D(x-1, y, nx)].east = true;
            map->elements[offset2D(x+1, y, nx)].west = true;
            break;
        case NORTH_EAST:
            map->elements[offset2D(x+1, y, nx)].west = true;
            break;
        case NORTH_WEST:
            map->elements[offset2D(x-1, y, nx)].east = true;
            break;
        case SOUTH_WEST:
            map->elements[offset2D(x-1, y, nx)].east = true;
            map->elements[offset2D(x, y+1, nx)].north = true;
            break;
        case SOUTH_EAST:
            map->elements[offset2D(x, y+1, nx)].north = true;
            map->elements[offset2D(x+1, y, nx)].west = true;
            break;
        default:
            break;
        }
    } else if (x != 0 && x != nx-1 && y == ny-1) {
        /* @@@@
           @..@
           @..@
           @xx@ */
        switch (map->elements[offset2D(x, y, nx)].type) {
        case NORTH_SOUTH:
            map->elements[offset2D(x, y-1, nx)].south = true;
            break;
        case EAST_WEST:
            map->elements[offset2D(x-1, y, nx)].east = true;
            map->elements[offset2D(x+1, y, nx)].west = true;
            break;
        case NORTH_EAST:
            map->elements[offset2D(x+1, y, nx)].west = true;
            map->elements[offset2D(x, y-1, nx)].south = true;
            break;
        case NORTH_WEST:
            map->elements[offset2D(x, y-1, nx)].south = true;
            map->elements[offset2D(x-1, y, nx)].east = true;
            break;
        case SOUTH_WEST:
            map->elements[offset2D(x-1, y, nx)].east = true;
            break;
        case SOUTH_EAST:
            map->elements[offset2D(x+1, y, nx)].west = true;
            break;
        default:
            break;
        }
    } else if (x != 0 && x != nx-1 && y != 0 && y != ny-1) {
        /* @@@@
           @xx@
           @xx@
           @@@@ */
        switch (map->elements[offset2D(x, y, nx)].type) {
        case NORTH_SOUTH:
            map->elements[offset2D(x, y-1, nx)].south = true;
            map->elements[offset2D(x, y+1, nx)].north = true;
            break;
        case EAST_WEST:
            map->elements[offset2D(x-1, y, nx)].east = true;
            map->elements[offset2D(x+1, y, nx)].west = true;
            break;
        case NORTH_EAST:
            map->elements[offset2D(x+1, y, nx)].west = true;
            map->elements[offset2D(x, y-1, nx)].south = true;
            break;
        case NORTH_WEST:
            map->elements[offset2D(x, y-1, nx)].south = true;
            map->elements[offset2D(x-1, y, nx)].east = true;
            break;
        case SOUTH_WEST:
            map->elements[offset2D(x-1, y, nx)].east = true;
            map->elements[offset2D(x, y+1, nx)].north = true;
            break;
        case SOUTH_EAST:
            map->elements[offset2D(x, y+1, nx)].north = true;
            map->elements[offset2D(x+1, y, nx)].west = true;
        default:
            break;
        }
    }
}

void set_map_conaction(ada_Tile_array *map)
{
    for (size_t y = 0; y < ny; y++) {
        for (size_t x = 0; x < nx; x++) {
            set_conaction(map, x, y);
        }
    }
}

void init_map(ada_Tile_array *map)
{
    for (size_t i = 0; i < map->length; i++) {
        set_type(&(map->elements[i]));
        map->elements[i].north = false;
        map->elements[i].east = false;
        map->elements[i].south = false;
        map->elements[i].west = false;
        map->elements[i].dead_end = true;
    }
}

void find_start(size_t *x, size_t *y, ada_Tile_array *map)
{
    for (size_t y_index = 0; y_index < ny; y_index++) {
        for (size_t x_index = 0; x_index < nx; x_index++) {
            if (map->elements[offset2D(x_index, y_index, nx)].type == START) {
                *x = x_index;
                *y = y_index;
                map->elements[offset2D(x_index, y_index, nx)].loop = true;
                map->elements[offset2D(x_index, y_index, nx)].dead_end = false;
                if (map->elements[offset2D(x_index, y_index, nx)].north && map->elements[offset2D(x_index, y_index, nx)].south) {
                    map->elements[offset2D(x_index, y_index, nx)].tile_enter = SOUTH;
                    map->elements[offset2D(x_index, y_index, nx)].type = NORTH_SOUTH;
                }
                if (map->elements[offset2D(x_index, y_index, nx)].east && map->elements[offset2D(x_index, y_index, nx)].west) {
                    map->elements[offset2D(x_index, y_index, nx)].tile_enter = WEST;
                    map->elements[offset2D(x_index, y_index, nx)].type = EAST_WEST;
                }
                if (map->elements[offset2D(x_index, y_index, nx)].north && map->elements[offset2D(x_index, y_index, nx)].east) {
                    map->elements[offset2D(x_index, y_index, nx)].tile_enter = EAST;
                    map->elements[offset2D(x_index, y_index, nx)].type = NORTH_EAST;
                }
                if (map->elements[offset2D(x_index, y_index, nx)].north && map->elements[offset2D(x_index, y_index, nx)].west) {
                    map->elements[offset2D(x_index, y_index, nx)].tile_enter = WEST;
                    map->elements[offset2D(x_index, y_index, nx)].type = NORTH_WEST;
                }
                if (map->elements[offset2D(x_index, y_index, nx)].south && map->elements[offset2D(x_index, y_index, nx)].west) {
                    map->elements[offset2D(x_index, y_index, nx)].tile_enter = WEST;
                    map->elements[offset2D(x_index, y_index, nx)].type = SOUTH_WEST;
                }
                if (map->elements[offset2D(x_index, y_index, nx)].south && map->elements[offset2D(x_index, y_index, nx)].east) {
                    map->elements[offset2D(x_index, y_index, nx)].tile_enter = SOUTH;
                    map->elements[offset2D(x_index, y_index, nx)].type = SOUTH_EAST;
                }
                return;
            }
        }
    }
}

void next_xy(size_t *next_x, size_t *next_y, ada_Tile_array *map, size_t x, size_t y)
{
    switch (map->elements[offset2D(x, y, nx)].tile_enter)
    {
    case NORTH:
        switch (map->elements[offset2D(x, y, nx)].type) {
        case NORTH_SOUTH:
            *next_x = x;
            *next_y = y+1;
            map->elements[offset2D(*next_x, *next_y, nx)].tile_enter = NORTH;
            map->elements[offset2D(*next_x, *next_y, nx)].loop = true;
            map->elements[offset2D(x, y, nx)].dead_end = false;
            break;
        case EAST_WEST:
            break;
        case NORTH_EAST:
            *next_x = x+1;
            *next_y = y;
            map->elements[offset2D(*next_x, *next_y, nx)].tile_enter = WEST;
            map->elements[offset2D(*next_x, *next_y, nx)].loop = true;
            map->elements[offset2D(x, y, nx)].dead_end = false;
            break;
        case NORTH_WEST:
            *next_x = x-1;
            *next_y = y;
            map->elements[offset2D(*next_x, *next_y, nx)].tile_enter = EAST;
            map->elements[offset2D(*next_x, *next_y, nx)].loop = true;
            map->elements[offset2D(x, y, nx)].dead_end = false;
            break;
        case SOUTH_WEST:
            break;
        case SOUTH_EAST:
            break;
        default:
            *next_x = -1;
            *next_y = -1;
            break;
        }
        break;
    case EAST:
        switch (map->elements[offset2D(x, y, nx)].type) {
        case NORTH_SOUTH:
            break;
        case EAST_WEST:
            *next_x = x-1;
            *next_y = y;
            map->elements[offset2D(*next_x, *next_y, nx)].tile_enter = EAST;
            map->elements[offset2D(*next_x, *next_y, nx)].loop = true;
            map->elements[offset2D(x, y, nx)].dead_end = false;
            break;
        case NORTH_EAST:
            *next_x = x;
            *next_y = y-1;
            map->elements[offset2D(*next_x, *next_y, nx)].tile_enter = SOUTH;
            map->elements[offset2D(*next_x, *next_y, nx)].loop = true;
            map->elements[offset2D(x, y, nx)].dead_end = false;
            break;
        case NORTH_WEST:
            break;
        case SOUTH_WEST:
            break;
        case SOUTH_EAST:
            *next_x = x;
            *next_y = y+1;
            map->elements[offset2D(*next_x, *next_y, nx)].tile_enter = NORTH;
            map->elements[offset2D(*next_x, *next_y, nx)].loop = true;
            map->elements[offset2D(x, y, nx)].dead_end = false;
            break;
        default:
            *next_x = -1;
            *next_y = -1;
            break;
        }
        break;
    case SOUTH:
        switch (map->elements[offset2D(x, y, nx)].type) {
        case NORTH_SOUTH:
            *next_x = x;
            *next_y = y-1;
            map->elements[offset2D(*next_x, *next_y, nx)].tile_enter = SOUTH;
            map->elements[offset2D(*next_x, *next_y, nx)].loop = true;
            map->elements[offset2D(x, y, nx)].dead_end = false;
            break;
        case EAST_WEST:
            break;
        case NORTH_EAST:
            break;
        case NORTH_WEST:
            break;
        case SOUTH_WEST:
            *next_x = x-1;
            *next_y = y;
            map->elements[offset2D(*next_x, *next_y, nx)].tile_enter = EAST;
            map->elements[offset2D(*next_x, *next_y, nx)].loop = true;
            map->elements[offset2D(x, y, nx)].dead_end = false;
            break;
        case SOUTH_EAST:
            *next_x = x+1;
            *next_y = y;
            map->elements[offset2D(*next_x, *next_y, nx)].tile_enter = WEST;
            map->elements[offset2D(*next_x, *next_y, nx)].loop = true;
            map->elements[offset2D(x, y, nx)].dead_end = false;
            break;
        default:
            *next_x = -1;
            *next_y = -1;
            break;
        }
        break;
    case WEST:
        switch (map->elements[offset2D(x, y, nx)].type) {
        case NORTH_SOUTH:
            break;
        case EAST_WEST:
            *next_x = x+1;
            *next_y = y;
            map->elements[offset2D(*next_x, *next_y, nx)].tile_enter = WEST;
            map->elements[offset2D(*next_x, *next_y, nx)].loop = true;
            map->elements[offset2D(x, y, nx)].dead_end = false;
            break;
        case NORTH_EAST:
            break;
        case NORTH_WEST:
            *next_x = x;
            *next_y = y-1;
            map->elements[offset2D(*next_x, *next_y, nx)].tile_enter = SOUTH;
            map->elements[offset2D(*next_x, *next_y, nx)].loop = true;
            map->elements[offset2D(x, y, nx)].dead_end = false;
            break;
        case SOUTH_WEST:
            *next_x = x;
            *next_y = y+1;
            map->elements[offset2D(*next_x, *next_y, nx)].tile_enter = NORTH;
            map->elements[offset2D(*next_x, *next_y, nx)].loop = true;
            map->elements[offset2D(x, y, nx)].dead_end = false;
            break;
        case SOUTH_EAST:
        default:
            *next_x = -1;
            *next_y = -1;
            break;
        }
        break;
    default:
        break;
    }
}
