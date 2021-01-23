#include <stdio.h>
#include <string.h>
#include "map/map.h"
#include "lw/lw.h"

char get_field_type(char *originalType)
{
    if(strcmp(originalType, LW_SAND_FIELD) == 0)
        return MAP_SAND_FIELD;
    
    if(strcmp(originalType, LW_GRASS_FIELD) == 0)
        return MAP_GRASS_FIELD;
    
    if(strcmp(originalType, LW_WALL_FIELD) == 0)
        return MAP_WALL_FIELD;
    
    return '?';
}

char get_tank_direction(char direction)
{
    switch(direction)
    {
        case LW_TANK_NORTH_DIR:
            return MAP_TANK_NORTH_DIR;
            break;
        case LW_TANK_EAST_DIR:
            return MAP_TANK_EAST_DIR;
            break;
        case LW_TANK_SOUTH_DIR:
            return MAP_TANK_SOUTH_DIR;
            break;
        case LW_TANK_WEST_DIR:
            return MAP_TANK_WEST_DIR;
            break;
        default:
            return '?';
            break;
    }
}

void update_map_with_field(Map *m, LwField field)
{
    map_update_field(m, field.coordinates.x, field.coordinates.y, get_field_type(field.field_type));
}

void update_map_with_exploration(Map *m, LwExploration exploration)
{    
    update_map_with_field(m, exploration.field1);
    update_map_with_field(m, exploration.field2);
    update_map_with_field(m, exploration.field3);
}

void update_map_with_tank(Map *m, LwTankPosition tank_position)
{
    map_update_tank(m, tank_position.field.coordinates.x, tank_position.field.coordinates.y, get_tank_direction(tank_position.direction));
}

void update_map_with_tank_position(Map *m, LwTankPosition tank_position)
{
    update_map_with_field(m, tank_position.field);
    update_map_with_tank(m, tank_position);
}

void run_command(Map *map, char *command)
{
    if(strcmp(command, "info") == 0){
        printf("\ninformacje\n");
        LwTankPosition tank_position = lw_info();
        update_map_with_tank_position(map, tank_position);
    }
    else if(strcmp(command, "m") == 0){
        printf("\nruch\n");
        LwTankPosition tank_position = lw_move();
        update_map_with_tank_position(map, tank_position);
    }
    else if(strcmp(command, "e") == 0){
        printf("\nrozgladanie sie\n");
        LwExploration exploration = lw_explore();
        update_map_with_exploration(map, exploration);
    }
    else if(strcmp(command, "r") == 0){
        printf("\nobrot w prawo\n");
        LwTankPosition tank_position = lw_rotate_right();
        update_map_with_tank_position(map, tank_position);
    }
    else if(strcmp(command, "l") == 0){
        printf("\nobrot w lewo\n");
        LwTankPosition tank_position = lw_rotate_left();
        update_map_with_tank_position(map, tank_position);
    } else {
        printf("\nNieznana komenda \'%s\'\n", command);
    }
}

void main(int argc, char **argv)
{
    LwTankPosition initial_tank_position = lw_info();
    
    char tank_direction = get_tank_direction(initial_tank_position.direction);
    
    Map *map = map_create(tank_direction);
    
    for(int i = 1; i < argc; i++) {
        char* command = argv[i];
        
        run_command(map, command);
        
        map_print(map);
    }
}
