typedef struct
{
    int x1; int y1; char *type1;
    int x2; int y2; char *type2;
    int x3; int y3; char *type3;
} LwExploration;

typedef struct 
{
    char direction;
} LwTankPosition;


LwTankPosition lw_info();
LwTankPosition lw_move();
LwTankPosition lw_rotate_left();
LwTankPosition lw_rotate_right();
LwExploration lw_explore(); 