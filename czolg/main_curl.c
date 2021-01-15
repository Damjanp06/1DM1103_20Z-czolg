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

struct mapa
{
    char *pola[N][N];
};

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

struct mapa generate_map(){
    struct mapa m; 
    for ( int i = 0 ; i < N ; i++ ){
        for ( int j = 0 ; j < N ; j++ ){
            m.pola[i][j] = " ";
        }
    }
    return m;
}

void print_map(struct mapa m){
    for ( int i = N ; i > 0 ; i-- ){
        int y = i - 1;
        printf("%.2d ", i);
        for ( int j = 0 ; j < N ; j++ ){
            printf(" %s ", m.pola[j][y]);
        }
        printf("\n");
    }
    printf("\n   ");
    for ( int i = 1; i <= N; i++ )
        printf("%.2d ", i);
    printf("\n");
}

struct mapa update_map(struct mapa m, struct pola p){
    struct mapa mnew = m;
    if(strcmp(p.type1, "sand") == 0){
        mnew.pola[p.x1][p.y1] = "S";
    }
    else if(strcmp(p.type1, "grass") == 0){
        mnew.pola[p.x1][p.y1] = "G";
    }
    else if(strcmp(p.type1, "wall") == 0){
        mnew.pola[p.x1][p.y1] = "W";
    }
    if(strcmp(p.type2, "sand") == 0){
        mnew.pola[p.x2][p.y2] = "S";
    }
    else if(strcmp(p.type2, "grass") == 0){
        mnew.pola[p.x2][p.y2] = "G";
    }
    else if(strcmp(p.type2, "wall") == 0){
        mnew.pola[p.x2][p.y2] = "W";
    }
    if(strcmp(p.type3, "sand") == 0){
        mnew.pola[p.x3][p.y3] = "S";
    }
    else if(strcmp(p.type3, "grass") == 0){
        mnew.pola[p.x3][p.y3] = "G";
    }
    else if(strcmp(p.type3, "wall") == 0){
        mnew.pola[p.x3][p.y3] = "W";
    }
    return mnew;
}

void main(int argc, char **argv)
{
    struct pola p;
    struct mapa map = generate_map();

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
            struct mapa m;
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
            map = update_map(map, p);
            
        }
        else if(strcmp(argv[i], "r") == 0){
            printf("\nobrot w prawo\n");
            printf("%s\n", rotate("right"));
        }
        else if(strcmp(argv[i], "l") == 0){
            printf("\nobrot w lewo\n");
            printf("%s\n", rotate("left"));
        }
    print_map(map);
    }
}

