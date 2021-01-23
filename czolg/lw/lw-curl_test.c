#include <assert.h>
#include "lw-curl.c"

void test_parse_exploration_data()
{
    char *exploration_result = "{\"status\": \"Success\", \"payload\": {\"list\": [{\"x\": 4, \"y\": 3, \"type\": \"grass\"}, {\"x\": 3, \"y\": 3, \"type\": \"wall\"}, {\"x\": 2, \"y\": 3, \"type\": \"sand\"}]}}";
    LwExploration exploration = parse_exploration_data(exploration_result);

    assert(strcmp(exploration.field1.field_type, "grass") == 0);
    assert(exploration.field1.coordinates.x == 4);
    assert(exploration.field1.coordinates.y == 3);

    assert(strcmp(exploration.field2.field_type, "wall") == 0);
    assert(exploration.field2.coordinates.x == 3);
    assert(exploration.field2.coordinates.y == 3);

    assert(strcmp(exploration.field3.field_type, "sand") == 0);
    assert(exploration.field3.coordinates.x == 2);
    assert(exploration.field3.coordinates.y == 3);
}

void test_parse_position_data()
{
    char *position_result = "{\"status\": \"Success\", \"payload\": {\"name\": \"First world 11\", \"current_x\": 3, \"current_y\": 2, \"current_session\": \"984bd43f-e513-4380-86ce-32d70bfc200b\", \"direction\": \"N\", \"step\": 224, \"field_type\": \"grass\", \"field_bonus\": \"\"}}";
    LwTankPosition position = parse_position_data(position_result);

    assert(position.direction == 'N');
    assert(strcmp(position.field.field_type, "grass") == 0);
    assert(position.field.coordinates.x == 3);
    assert(position.field.coordinates.y == 2);
}

void main()
{
    test_parse_exploration_data();
    test_parse_position_data();
}