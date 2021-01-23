#include <assert.h>
#include "map.c"

void test_map_create()
{
    Map *map = map_create('>');

    assert(map->x_size == 5);
    assert(map->y_size == 5);

    assert(map->tank.x_tank == 2);
    assert(map->tank.y_tank == 2);
    assert(map->tank.direction == '>');

    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            assert(map->fields[i][j] == 0);
        }
    }
}

void test_update_field()
{
    Map *map = map_create('>');

    map_update_field(map, 2, 3, 'T');

    assert(map->fields[2][3] == 'T');
}

void test_update_tank()
{
    Map *map = map_create('>');

    map_update_tank(map, 10, 20, 'v');

    assert(map->tank.direction == 'v');
    assert(map->tank.x_tank == 10);
    assert(map->tank.y_tank == 20);
}

void main()
{
    test_map_create();
    test_update_field();
    test_update_tank();
}