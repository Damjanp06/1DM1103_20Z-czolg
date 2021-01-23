#include <stdio.h>
#include <string.h>
#include "map/map.h"
#include "lw/lw.h"

char __get_field_type(char *originalType)
{
    if(strcmp(originalType, "sand") == 0)
        return MAP_SAND_FIELD;
    
    if(strcmp(originalType, "grass") == 0)
        return MAP_GRASS_FIELD;
    
    if(strcmp(originalType, "wall") == 0)
        return MAP_WALL_FIELD;
    
    return '?';
}

void update_map(Map *m, LwExploration p){
    
    map_update_field(m, p.x1, p.y1, __get_field_type(p.type1));
    map_update_field(m, p.x2, p.y2, __get_field_type(p.type2));
    map_update_field(m, p.x3, p.y3, __get_field_type(p.type3));
}

char get_tank_direction()
{
    LwTankPosition tank_position = lw_info();

    switch (tank_position.direction)
    {
        case 'N':
            return MAP_TANK_NORTH_DIR;
            break;
        case 'E':
            return MAP_TANK_EAST_DIR;
            break;
        case 'S':
            return MAP_TANK_SOUTH_DIR;
            break;
        case 'W':
            return MAP_TANK_WEST_DIR;
            break;
    default:
        break;
    }
}

void run_command(Map *map, char *command)
{
    if(strcmp(command, "info") == 0){
        printf("\ninformacje\n");
        lw_info();
    }
    else if(strcmp(command, "m") == 0){
        printf("\nruch\n");
        lw_move();
    }
    else if(strcmp(command, "e") == 0){
        printf("\nrozgladanie sie\n");
        LwExploration p = lw_explore();
        update_map(map, p);
    }
    else if(strcmp(command, "r") == 0){
        printf("\nobrot w prawo\n");
        lw_rotate_right();
    }
    else if(strcmp(command, "l") == 0){
        printf("\nobrot w lewo\n");
        lw_rotate_left();
    } else {
        printf("\nNieznana komenda \'%s\'\n", command);
    }
}

void main(int argc, char **argv)
{
    char tank_direction = get_tank_direction();
    Map *map = map_create(tank_direction);
    for(int i = 1; i < argc; i++) {
        char* command = argv[i];
        
        run_command(map, command);
        
        map_print(map);
    }
}
