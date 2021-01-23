#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <string.h>
#include <stdlib.h>
#include "lw.h"

#define LW_CURL_DUMP 0

#if LW_CURL_DUMP
    #include <stdio.h>
#endif

#define URL_EXPLORE "http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/explore/qwerty_10"
#define URL_ROTATE_LEFT "http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/rotate/qwerty_10/left"
#define URL_ROTATE_RIGHT "http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/rotate/qwerty_10/right"
#define URL_INFO "http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/info/qwerty_10"
#define URL_MOVE "http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/move/qwerty_10"

typedef struct _Memory
{
    char *response;
    size_t size;
} Memory;

static size_t write_callback(void *data, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    Memory *mem = (Memory *) userp;
    char *ptr = NULL;

    if (mem->response != NULL)
        ptr = realloc(mem->response, mem->size + realsize + 1);
    else
        ptr = malloc(mem->size + realsize + 1);

    if (ptr == NULL)
        return 0;

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;

    return realsize;
}

char * make_request(char *url){
    CURL *curl;
    CURLcode res;
    Memory chunk;
    chunk.size = 0;
    chunk.response = NULL;

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_HEADER, 0L);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);

    #if LW_CURL_DUMP
        printf("%s\n", chunk.response);
    #endif
        return chunk.response;

            free(chunk.response);
            curl_easy_cleanup(curl);
    }
}

LwField parse_position_field(cJSON *payload_json)
{
    LwCoordinates coordinates = {   
                                    .x = cJSON_GetObjectItemCaseSensitive(payload_json, "current_x")->valueint, 
                                    .y = cJSON_GetObjectItemCaseSensitive(payload_json, "current_y")->valueint
                                };

    LwField f = {   
                    .coordinates = coordinates, 
                    .field_type = cJSON_GetObjectItemCaseSensitive(payload_json, "field_type")->valuestring
                };
    return f;
}

LwTankPosition parse_position_data(char *position_result)
{ 
    cJSON *position_json = cJSON_Parse(position_result);
    cJSON *payload_json = cJSON_GetObjectItemCaseSensitive(position_json, "payload");

    char direction = cJSON_GetObjectItemCaseSensitive(payload_json, "direction")->valuestring[0];

    LwTankPosition tank_position;
    tank_position.direction = direction;
    tank_position.field = parse_position_field(payload_json);

    return tank_position;
}

LwTankPosition lw_info()
{
    return parse_position_data(make_request(URL_INFO));
}

LwTankPosition lw_move() 
{
    return parse_position_data(make_request(URL_MOVE));
}

LwTankPosition lw_rotate_left() 
{
    return parse_position_data(make_request(URL_ROTATE_LEFT));
}

LwTankPosition lw_rotate_right() 
{
    return parse_position_data(make_request(URL_ROTATE_RIGHT));
}

LwField parse_exploration_field(cJSON *field_json)
{
    LwCoordinates coordinates = {
        .x = cJSON_GetObjectItemCaseSensitive(field_json, "x")->valueint,
        .y = cJSON_GetObjectItemCaseSensitive(field_json, "y")->valueint 
    };
    
    LwField f = {
        .coordinates = coordinates,
        .field_type = cJSON_GetObjectItemCaseSensitive(field_json, "type")->valuestring
    };

    return f;
}

LwExploration parse_exploration_data(char *exploration_result)
{
    LwExploration p;

    cJSON *exploration_json = cJSON_Parse(exploration_result);
    cJSON *payload_json = cJSON_GetObjectItemCaseSensitive(exploration_json, "payload");
    cJSON *list_json = cJSON_GetObjectItemCaseSensitive(payload_json, "list");

    cJSON *pole1 = cJSON_GetArrayItem(list_json, 0);
    cJSON *pole2 = cJSON_GetArrayItem(list_json, 1);
    cJSON *pole3 = cJSON_GetArrayItem(list_json, 2);

    p.field1 = parse_exploration_field(pole1);
    p.field2 = parse_exploration_field(pole2);
    p.field3 = parse_exploration_field(pole3);

    return p;
}

LwExploration lw_explore() 
{
    return parse_exploration_data(make_request(URL_EXPLORE));
}


