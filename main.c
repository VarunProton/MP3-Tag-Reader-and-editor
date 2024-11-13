#include<stdio.h>
#include"edit.h"

int main(char argc, char *argv[])
{
    if(strcmp(argv[1],"-h") == 0 | argc < 2)
    {
        printf("USAGE:\nTo view please pass like: ./a.out -v mp3filename\nTo edit please pass like: ./a.out -e -t/-a/-b/-y/-c mp3filename");
        printf("\n-v for display of the message\n-e for editing the file\nIn edit\n-t for Tittle\n-a for Artist name\n-b for Album name\n-y for Year\n-c for Comment on thr mp3\n");
        return 1;
    }
    Operation op = check_operation_type(argv[1]);
    if(op == mp3_display)
    {
        ID3v2aTag tag;
        if(read_and_validate(argv[2]) == e_failure)
        {
            printf("ERROR: Enter correct command line arguments.\n");
            return 1;
        }
        if(open_file(argv[2] , &tag) == e_failure)
        {
            printf("ERROR: Enter correct command line arguments.\n");
            return 1;
        }
        printf("Successfully opened all files.\n");
        
        if(read_and_place(tag.mp3_fptr, &tag) == e_failure)
        {
            printf("ERROR: While reading and placing the data.\n");
            return 1;
        }
        printf("Successfully read all the data.\n");

        display(&tag);

        printf("Successfully printed all data.\n");
    }
    else if(op == mp3_edit)
    {
        ID3v2Edit edit;
        ID3v2aTag tag;
        if(argc < 5)
        {
            printf("ERROR: Very few arguments given.\n");
            return 1;
        }

        if(open_file_edit(argv[4],&edit) != e_success)
        {
            printf("ERROR While opening file.\n");
            return 1;
        }

        if(read_and_validate_edit(argv, &edit) != e_success)
        {
            printf("ERROR: While the edit process been taken place.\n");
            return 1;
        }

        if(open_file(argv[4],&tag) != e_success)
        {
            printf("ERROR While opening file.\n");
            return 1;
        }

        if(read_and_place(tag.mp3_fptr, &tag) == e_failure)
        {
            printf("ERROR: While reading and placing the data.\n");
            return 1;
        }
        printf("Successfully read all the data.\n");
        display(&tag);
        printf("All the editing operations were done and displayed.\n");
    }
    else
    {
        printf("Error: Invalid argument, give correct command line argument.\n");
        return 1;
    }
}
Operation check_operation_type(char *argv)
{
    if(strcmp(argv,"-v") == e_success)
        return mp3_display;
    else if (strcmp(argv,"-e") == e_success)
        return mp3_edit;
    else
        return mp3_invalid;
}