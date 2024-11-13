#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "edit.h"

// Opens an MP3 file for editing
Status open_file_edit(char *file_path, ID3v2Edit *tag)
{
    tag->mp3_fptr = fopen(file_path, "r+b");
    if (tag->mp3_fptr == NULL)
    {
        printf("ERROR: File not found or could not be opened for editing.\n");
        return e_failure;
    }
    fseek(tag->mp3_fptr, 0, SEEK_END);
    tag->size_mp3 = ftell(tag->mp3_fptr);
    fseek(tag->mp3_fptr, 0, SEEK_SET);
    return e_success;
}

// Validates and edits the specified tag in the MP3 file
Status read_and_validate_edit(char *argv[], ID3v2Edit *edit_tag)
{        
    if (argv[2][0] == '-' || argv[3] != NULL) 
    {
        // Determine the tag type based on the second argument
        switch (argv[2][1]) 
        { 
            case 't': // Title
                printf("Editing Title.\n");
                if (Find_the_position("TIT2", edit_tag, argv[3]) != e_success) 
                {
                    printf("ERROR: Could not change the title.\n");
                    return e_failure;
                }
                printf("Successfully changed the title.\n");
                break;

            case 'a': // Artist
                printf("Editing Artist.\n");
                if (Find_the_position("TPE1", edit_tag, argv[3]) != e_success) 
                {
                    printf("ERROR: Could not change the artist.\n");
                    return e_failure;
                }
                printf("Successfully changed the artist.\n");
                break;

            case 'b': // Album
                printf("Editing Album.\n");
                if (Find_the_position("TALB", edit_tag, argv[3]) != e_success) 
                {
                    printf("ERROR: Could not change the album.\n");
                    return e_failure;
                }
                printf("Successfully changed the album.\n");
                break;

            case 'y': // Year
                printf("Editing Year.\n");
                if (Find_the_position("TYER", edit_tag, argv[3]) != e_success) 
                {
                    printf("ERROR: Could not change the year.\n");
                    return e_failure;
                }
                printf("Successfully changed the year.\n");
                break;

            case 'c': // Comment
                printf("Editing Comment.\n");
                if (Find_the_position("COMM", edit_tag, argv[3]) != e_success) 
                {
                    printf("ERROR: Could not change the comment.\n");
                    return e_failure;
                }
                printf("Successfully changed the comment.\n");
                break;

            default:
                printf("ERROR: Invalid tag option.\nUsage:\n-t <title>    Change title\n-a <artist>   Change artist\n-b <album>    Change album\n-y <year>     Change year\n-c <comment>  Change comment\n");
                return e_failure;
        }
    }
    return e_success;
}

Status Find_the_position(char sub_string[5], ID3v2Edit *tag, char *new_data) 
{
    fseek(tag->mp3_fptr, 10, SEEK_SET); // Skip header

    for(char i=0 ; i<6 ; i++) 
    {
        // Read tag identifier
        if (fread(tag->tag, 1, 4, tag->mp3_fptr) != 4) 
        {
            printf("ERROR: Could not read tag.\n");
            return e_failure;
        }
        tag->tag[4] = '\0';

        // Read size
        unsigned char size_bytes_1[4];
        if (fread(size_bytes_1, 1, 4, tag->mp3_fptr) != 4) 
        {
            printf("ERROR: Could not read tag size.\n");                
            return e_failure;
        }
        tag->size = ((size_bytes_1[0] << 21) | (size_bytes_1[1] << 14) | (size_bytes_1[2] << 7) | size_bytes_1[3])-1;
        
        if (strcmp(tag->tag, sub_string) == 0) 
        {
            printf("Found tag.\n");
            
            FILE *temp_fptr = fopen("temp.mp3", "w+b");
            if (temp_fptr == NULL)
            {
                printf("ERROR: Temporary file could not be created.\n");
                return e_failure;
            }
            fseek(tag->mp3_fptr, -4, SEEK_CUR);
            unsigned int pos = ftell(tag->mp3_fptr);
            fseek(tag->mp3_fptr, 0, SEEK_SET);
            char *header = (char *)malloc(pos);
            if (header == NULL) 
            {
                printf("Memory allocation failed.\n");
                fclose(temp_fptr);
                return e_failure;
            }

            if (fread(header, pos, 1, tag->mp3_fptr) != 1)
            {
                printf("ERROR: Could not read MP3 header.\n");
                fclose(temp_fptr);
                return e_failure;
            }
            if(fwrite(header, pos, 1, temp_fptr) != 1)
            {
                printf("ERROR: Could not write MP3 header.\n");
                fclose(temp_fptr);
                return e_failure;
            }
            free(header);

            // Read size
            fseek(tag->mp3_fptr, 4, SEEK_CUR);
            char *size_bytes = (char *)malloc(4);
            // Update the tag's data
            int new_data_length = strlen(new_data)+1;
            size_bytes[0] = (new_data_length >> 24) & 0xFF;
            size_bytes[1] = (new_data_length >> 16) & 0xFF;
            size_bytes[2] = (new_data_length >> 8) & 0xFF;
            size_bytes[3] = new_data_length & 0xFF;


            if(fwrite(size_bytes, 4, 1, temp_fptr) != 1)
            {
                printf("ERROR: While pasting size.\n");
                return e_failure;
            }

            free(size_bytes);
            char *flags = (char*)malloc(3);
            if(fread(flags, 3, 1, tag->mp3_fptr) != 1)
            {
                printf("ERROR: While copying flags.\n");
                return e_failure;
            }
            if(fwrite(flags, 3, 1, temp_fptr) != 1)
            {
                printf("ERROR: While pasting flags.\n");
                return e_failure;
            }
            free(flags);

            if(fwrite(new_data, new_data_length-1, 1, temp_fptr) != 1)
            {
                printf("ERROR: While pasting the new data.\n");
                return e_failure;
            }
            fseek(tag->mp3_fptr, tag->size, SEEK_CUR);

            long int diff = tag->size_mp3 - ftell(tag->mp3_fptr);
            char *remaining_data = (char *)malloc(diff);
            if(fread(remaining_data, diff, 1, tag->mp3_fptr) != 1)
            {
                printf("ERROR: While copying remaining data.\n");
                return e_failure;
            }
            if(fwrite(remaining_data, diff, 1, temp_fptr) != 1)
            {
                printf("ERROR: While pasting remaining data.\n");
                return e_failure;
            }
            free(remaining_data);
            fclose(tag->mp3_fptr);
            fclose(temp_fptr);
            remove("original.mp3");
            rename("temp.mp3", "original.mp3");
            return e_success;
        } 
        else 
        {
            fseek(tag->mp3_fptr, 3, SEEK_CUR);
            fseek(tag->mp3_fptr, tag->size, SEEK_CUR);
        }
    }

    printf("ERROR: Specified tag not found.\n");
    return e_failure;
}
