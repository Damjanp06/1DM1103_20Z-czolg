typedef struct _Map Map;

#define INITIAL_MAP_SIZE 5
#define INITIAL_TANK_X_POS 2
#define INITIAL_TANK_Y_POS 2

#define MAP_GRASS_FIELD 'G'
#define MAP_WALL_FIELD 'W'
#define MAP_SAND_FIELD 'S'

#define MAP_TANK_NORTH_DIR '^'
#define MAP_TANK_EAST_DIR '>'
#define MAP_TANK_SOUTH_DIR 'v'
#define MAP_TANK_WEST_DIR '<'

Map* map_create(char tank_direction);
void map_print(Map *map);
void map_update_field(Map *map, int x, int y, char field_type); 