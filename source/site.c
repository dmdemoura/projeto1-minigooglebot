#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "site.h"

struct site 
{
    int code;
    char name[NAME_SIZE];
    int relevance;
    char link[LINK_SIZE];
    char (*tag)[TAG_SIZE];
    int tag_count;
};


SITE* site_create(int code, char name[NAME_SIZE], int relevance, char link[LINK_SIZE], char** tag, int tag_count){
    SITE* site = malloc(sizeof(SITE));
    int i;
    if(site == NULL){
        printf("site_create: memory allocation failed\n");
        return NULL;
    }
    if (code > 9999)
    {
        printf("site_create: code should have at most 4 digits\n");
        return NULL;
    }
    if (relevance < 0)
    {
        printf("site_create: min relevance = 0\n");
        return NULL;
    }
    if (relevance > MAX_RELEVANCE)
    {
        printf("site_create: max relevance = 1000\n");
        return NULL;
    }
    if(tag_count > MAX_TAG_COUNT){
        printf("site_create: max tag count = 10\n");
        return NULL;
    }
    if(tag_count < 0){
        printf("site_create: cannot have negative tag count\n");
        return NULL;
    }
    
    site->tag = malloc(sizeof(char[TAG_SIZE])*MAX_TAG_COUNT);

    site->code = code;
    strcpy(site->name, name);
    site->relevance = relevance;
    strcpy(site->link, link);
    site->tag_count = tag_count;
    for(i=0; i<tag_count; i++){
        strcpy(site->tag[i], tag[i]);
    }

    return site;
}

void site_destroy(SITE** site_ptr){
    if(site_ptr == NULL || *site_ptr == NULL){
        printf("site_destroy: site is null\n");
        return;
    }

    free((*site_ptr)->tag);
    (*site_ptr)->tag = NULL;

    free(*site_ptr);
    *site_ptr = NULL;
}

bool site_add_tag(SITE* site, char tag[TAG_SIZE]){
    if(site == NULL){
        printf("site_add_tag: site is null\n");
        return FALSE;
    }
    if(site->tag_count == MAX_TAG_COUNT){
        printf("site_add_tag: max tag count reached\n");
        return FALSE;
    }

    strcpy(site->tag[site->tag_count], tag);
    site->tag_count++;

    return TRUE;
}

bool site_update_relevance(SITE* site, int relevance){
    if(site == NULL){
        printf("site_update_relevance: site is null\n");
        return FALSE;
    }
    if(relevance < 0 || relevance > MAX_RELEVANCE){
        printf("site_update_relevance: invalid relevance\n");
        return FALSE;
    }

    site->relevance = relevance;

    return TRUE;
}

int site_get_code(SITE* site){
    if(site == NULL){
        printf("site_get_code: site is null\n");
        return FALSE;
    }

    return site->code;
}
void site_serialize(SITE* site, FILE* file){
    int i;

    fprintf(file, "%04d,%s,%d,%s", site->code, site->name, site->relevance, site->link);
    for(i=0; i<site->tag_count; i++){
        fprintf(file, ",%s", site->tag[i]);        
    }
    fprintf(file,"\n");
}
void site_print(SITE* site){
    int i;

    printf("======================================\n"
    "Code:\t\t%d\n"
    "Name:\t\t%s\n"
    "Relevance:\t%d\n"
    "Link:\t\t%s\n"
    "Tags:\t\t",
    site->code, site->name, site->relevance, site->link);
    for(i=0; i<site->tag_count; i++){
        printf("%s ", site->tag[i]);        
    }
    printf("\n======================================\n");
}