#include "map.h"
#include <stdio.h>
#include <stdlib.h>

struct _Tank
{
    int x_tank;
    int y_tank;
    char direction;
};

struct _Map
{
    char **fields;
    int x_size;
    int y_size;
    struct _Tank tank;
};

int max(int a, int b)
{
    return a > b ? a :b;
}

void init_fields(Map *map)
{
    map->fields = calloc(map->y_size, sizeof(char*));
    for (int i = 0; i < map->x_size; i++)
        map->fields[i] = calloc(map->x_size, sizeof(char));
}

void init_tank(struct _Tank *tank, char direction)
{
    tank->x_tank = INITIAL_TANK_X_POS;
    tank->y_tank = INITIAL_TANK_Y_POS;
    tank->direction = direction;
}

Map* map_create(char tank_direction)
{
    Map *map = (Map *)malloc(sizeof(struct _Map));

    map->x_size = INITIAL_MAP_SIZE;
    map->y_size = INITIAL_MAP_SIZE;

    init_fields(map);
    init_tank(&map->tank, tank_direction);

    return map;
}

char get_drawing_character(Map *map, int x, int y)
{
    if(x == map->tank.x_tank && y == map->tank.y_tank)
        return map->tank.direction;

    char field_type = *(*(map->fields + x) + y); 
    return field_type ? field_type : ' ';
}

void map_print(Map *map) 
{
    for ( int i = map->y_size-1 ; i >= 0 ; i-- ){
        printf("%.2d ", i);
        for ( int j = 0 ; j < map->x_size ; j++ ){
            printf("%c ", get_drawing_character(map, j, i));
        }
        printf("\n");
    }
    printf("\n   ");
    for ( int i = 0; i < map->x_size; i++)
        printf("%.2d ", i);
    printf("\n");
}

void map_update_field(Map *map, int x, int y, char field_type)
{
    int new_size = max(x, y) + 1;

    if (new_size > map->x_size)
    {
        int old_size = map->x_size;
        map->x_size = new_size;
        map->y_size = new_size;

        map->fields = realloc(map->fields, map->y_size * sizeof(char*));

        for (int i = 0; i < map->y_size; i++)
        {
            if (i < old_size){
                map->fields[i] = realloc(map->fields[i], map->x_size * sizeof(char));
                for(int j = old_size; j <= map->x_size; j++)
                    map->fields[i][j] = 0;
            }
            else{
                map->fields[i] = calloc(map->x_size, sizeof(char));
            }
        }
    }
    map->fields[x][y] = field_type;
}
