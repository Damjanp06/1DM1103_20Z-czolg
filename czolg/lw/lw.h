#define LW_GRASS_FIELD "grass"
#define LW_WALL_FIELD "wall"
#define LW_SAND_FIELD "sand"

#define LW_TANK_NORTH_DIR 'N'
#define LW_TANK_EAST_DIR 'E'
#define LW_TANK_SOUTH_DIR 'S'
#define LW_TANK_WEST_DIR 'W'

typedef struct
{
    int x;
    int y;
} LwCoordinates;

typedef struct
{
    LwCoordinates coordinates; 
    char *field_type;
} LwField;

typedef struct
{
    LwField field1;
    LwField field2;
    LwField field3;
} LwExploration;

typedef struct 
{
    char direction;
    LwField field;
} LwTankPosition;


LwTankPosition lw_info();
LwTankPosition lw_move();
LwTankPosition lw_rotate_left();
LwTankPosition lw_rotate_right();
LwExploration lw_explore(); 