#include<stdio.h>
#include"view.h"
// #include"edit.h"

Status read_and_validate(char *argv)
{
    if(strstr(argv, ".mp3") == NULL)
    {
        printf("ERROR: Enter correct file name has \".mp3\" in it.\n");
        return e_failure;
    }
    return e_success;
}

Status open_file(char *argv, ID3v2aTag *tag)
{
    tag->mp3_fptr = fopen(argv, "rb");
    if(tag->mp3_fptr == NULL)
    {
        printf("ERROR: File missing add correct file.\n");
        return e_failure;
    }
    return e_success;
}

Status read_and_place(FILE *mp3_fptr, ID3v2aTag *tag)
{
    fseek(mp3_fptr, 10, SEEK_SET); // Position file pointer

    for (int i = 0; i < 6; i++) // Loop through expected number of tags
    {
        // Read tag identifier
        if (fread(tag->tag, 1, 4, mp3_fptr) != 4)
        {
            printf("ERROR: While copying file tag.\n");
            return e_failure;
        }
        tag->tag[4] = '\0'; 

        unsigned char size_bytes[4];
        if (fread(size_bytes, 1, 4, mp3_fptr) != 4)
        {
            printf("ERROR: Failed to read tag size.\n");
            return e_failure;
        }
        
        tag->size = (size_bytes[0] << 21) | (size_bytes[1] << 14) | (size_bytes[2] << 7) | size_bytes[3];
        fseek(mp3_fptr, 3, SEEK_CUR); // Move file pointer forward

        char *buffer = (char *)malloc(tag->size + 1);
        if (buffer == NULL)
        {
            printf("ERROR: Memory allocation failed.\n");
            return e_failure;
        }
        
        if (fread(buffer, tag->size-1, 1, mp3_fptr) != 1)
        {
            printf("ERROR: Failed to read tag data.\n");
            free(buffer);
            return e_failure;
        }
        buffer[tag->size] = '\0';
        // Assign based on tag type
        if (strcmp(tag->tag, "TIT2") == 0)
        {
            tag->title = buffer;
        }
        else if (strcmp(tag->tag, "TPE1") == 0)
        {
            tag->artist_name = buffer;
        }
        else if (strcmp(tag->tag, "TALB") == 0)
        {
            tag->album = buffer;
        }
        else if (strcmp(tag->tag, "TYER") == 0)
        {
            tag->year = buffer;
        }
        else if (strcmp(tag->tag, "TCON") == 0)
        {
            tag->content_type = buffer;
        }
        else if (strcmp(tag->tag, "COMM") == 0)
        {
            tag->comment = buffer;
        }
        else
        {
            printf("%d\n",tag->size);
            printf("INFO: Unknown tag '%s' encountered; skipping.\n", tag->tag);
            free(buffer);
            fseek(mp3_fptr, tag->size-1, SEEK_CUR);
        }
    }
    return e_success;
}

void display(ID3v2aTag *tag)
{
    printf("----------------------------------------------------------------\n");
    printf("    MP3 TAG DETAILS\n");
    printf("----------------------------------------------------------------\n");
    printf("    Title      : %.60s\n", tag->title);
    printf("    Artist     : %.60s\n", tag->artist_name);
    printf("    Album      : %.60s\n", tag->album);
    printf("    Year       : %.60s\n", tag->year);
    printf("    Content    : %.60s\n", tag->content_type);
    printf("    comment    : %.60s\n", tag->comment);
    printf("----------------------------------------------------------------\n");
}