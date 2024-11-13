#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"view.h"

typedef struct
{
    FILE *mp3_fptr;
    long int size_mp3;
    char tag[5];
    int size;    
}ID3v2Edit;

Status open_file_edit(char *argv, ID3v2Edit *tag);

Status read_and_validate_edit(char *argv[], ID3v2Edit *edit_tag);

Status Find_the_position(char sub_string[5], ID3v2Edit *tag, char *new_data);
