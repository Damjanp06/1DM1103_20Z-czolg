#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

#define N 12

struct pola
{
    int x1; int y1; char *type1;
    int x2; int y2; char *type2;
    int x3; int y3; char *type3;
};

typedef struct
{
    char **fields;
    int size;
} Map;

int __max(int a, int b)
{
    return a > b ? a :b;
}

void initMap(Map *map, int initial_size)
{
    map->size = initial_size;
    map->fields = calloc(initial_size, sizeof(char*));
    for (int i = 0; i < map->size; i++)
        map->fields[i] = calloc(map->size, sizeof(char));
}

void printMap(Map *map) 
{
    for ( int i = map->size-1 ; i >= 0 ; i-- ){
        printf("%.2d ", i);
        for ( int j = 0 ; j < map->size ; j++ ){
            char field_type = *(*(map->fields + i) + j);
            printf("%c ", field_type ? field_type : ' ');
        }
        printf("\n");
    }
    printf("\n   ");
    for ( int i = 0; i < map->size; i++)
        printf("%.2d ", i);
    printf("\n");
}

void updateField(Map *map, int x, int y, char field_type)
{
    int new_size = __max(x, y) + 1;

    if (new_size > map->size)
    {
        int old_size = map->size;
        map->size = new_size;

        map->fields = realloc(map->fields, map->size * sizeof(char*));

        for (int i = 0; i < map->size; i++)
        {
            if (i < old_size){
                map->fields[i] = realloc(map->fields[i], map->size * sizeof(char));
                for(int j = old_size; j <= map->size; j++)
                    map->fields[i][j] = 0;
            }
            else{
                map->fields[i] = calloc(map->size, sizeof(char));
            }
        }
    }
    map->fields[x][y] = field_type;
}

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

        return chunk.response;
        free(chunk.response);
        curl_easy_cleanup(curl);

    }
}

char * info() { 
    char *res;
    char *url = "http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/info/qwerty_10";
    res = make_request(url);
    return res;
}

char * move() {
    char *res;
    char *url ="http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/move/qwerty_10";
    res = make_request(url);
    return res;
}

char * explore() {
    char *res;
    char *url ="http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/explore/qwerty_10";
    res = make_request(url);
    return res;
}

char * rotate(char *site) {
    char *res;
    char *url;
    if(strcmp(site, "right") == 0){
        url ="http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/rotate/qwerty_10/right";\
    }
    else if (strcmp(site, "left") == 0){
        url ="http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/rotate/qwerty_10/left";
    }
    res = make_request(url);
    return res;
}

struct pola get_exploration_data(char *exploration_result){
    
    struct pola p;

    cJSON *exploration_json = cJSON_Parse(exploration_result);
    cJSON *payload_json = cJSON_GetObjectItemCaseSensitive(exploration_json, "payload");
    cJSON *list_json = cJSON_GetObjectItemCaseSensitive(payload_json, "list");

    cJSON *pole1 = cJSON_GetArrayItem(list_json, 0);
    cJSON *pole2 = cJSON_GetArrayItem(list_json, 1);
    cJSON *pole3 = cJSON_GetArrayItem(list_json, 2);

    p.x1 = cJSON_GetObjectItemCaseSensitive(pole1, "x")->valueint;
    p.y1 = cJSON_GetObjectItemCaseSensitive(pole1, "y")->valueint;
    p.type1 = cJSON_GetObjectItemCaseSensitive(pole1, "type")->valuestring;

    p.x2 = cJSON_GetObjectItemCaseSensitive(pole2, "x")->valueint;
    p.y2 = cJSON_GetObjectItemCaseSensitive(pole2, "y")->valueint;
    p.type2 = cJSON_GetObjectItemCaseSensitive(pole2, "type")->valuestring;

    p.x3 = cJSON_GetObjectItemCaseSensitive(pole3, "x")->valueint;
    p.y3 = cJSON_GetObjectItemCaseSensitive(pole3, "y")->valueint;
    p.type3 = cJSON_GetObjectItemCaseSensitive(pole3, "type")->valuestring;

    return p;
}

char __get_field_type(char *originalType)
{
    if(strcmp(originalType, "sand") == 0)
        return 'S';

    if(strcmp(originalType, "grass") == 0)
        return 'G';

    if(strcmp(originalType, "wall") == 0)
        return 'W';

    return '?';
}

void update_map(Map *m, struct pola p){

    updateField(m, p.x1, p.y1, __get_field_type(p.type1));
    updateField(m, p.x2, p.y2, __get_field_type(p.type2));
    updateField(m, p.x3, p.y3, __get_field_type(p.type3));
}

void main(int argc, char **argv)
{
    struct pola p;
    Map map;

    initMap(&map, 1);

    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "info") == 0){
            printf("\ninformacje\n");
            printf("%s\n", info());
        }
        else if(strcmp(argv[i], "m") == 0){
            printf("\nruch\n");
            printf("%s\n", move());
        }
        else if(strcmp(argv[i], "e") == 0){
            char *expl = explore();
            printf("\nrozgladanie sie\n");
            printf("%s\n", expl);
            p = get_exploration_data(expl);
            // printf("%d\n", p.x1);
            // printf("%d\n", p.y1);
            // printf("%s\n\n", p.type1);
            // printf("%d\n", p.x2);
            // printf("%d\n", p.y2);
            // printf("%s\n\n", p.type2);
            // printf("%d\n", p.x3);
            // printf("%d\n", p.y3);
            // printf("%s\n\n", p.type3);
            update_map(&map, p);
            
        }
        else if(strcmp(argv[i], "r") == 0){
            printf("\nobrot w prawo\n");
            printf("%s\n", rotate("right"));
        }
        else if(strcmp(argv[i], "l") == 0){
            printf("\nobrot w lewo\n");
            printf("%s\n", rotate("left"));
        }
    printMap(&map);
    }
}

