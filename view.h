#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct 
{
    FILE *mp3_fptr;
    char *title;
    char *artist_name;
    char *album;
    char *year;
    char *content_type;
    char *comment;
    // char *composer;
    char tag[5];
    int size;
}ID3v2aTag;

#define HEADER 10

typedef enum
{
    mp3_display,
    mp3_edit,
    mp3_invalid
} Operation;

typedef enum
{
    e_success,
    e_failure
} Status;

typedef enum
{
    title,
    artist,
    album,
    year,
    content_type,
    composer
}data;

Operation check_operation_type(char *argv);

Status read_and_validate(char *argv);

Status open_file(char *argv, ID3v2aTag *tag);

Status read_and_place(FILE *mp3_fptr, ID3v2aTag *tag);

void display(ID3v2aTag *tag);