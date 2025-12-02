#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>

#include "parse.h"
#include "files.h"
#include "common.h"

void usage(){

    printf("-u usage | -f filepath | -n new file\n");
}

int main(int argc, char *argv[]){

    char *portarg = NULL;
    char *filepath = NULL;

    bool newFile = false;
    bool list = false;
    int c;

    int DBFD = -1;
    struct DBHeader_t *DBFDHR = NULL;

    while ((c = getopt(argc, argv, "nf:a:l")) != -1) {
            switch (c) {
                case 'n':
                    newFile = true;
                    break;
                case 'f':
                    filepath = optarg;
                    break;
                case 'p':
                    portarg = optarg;
                    break;
                case 'l':
                    list = true;
                    break;
                case '?':
                    printf("Unknown option -%c\n", c);
                    break;
                default:
                    return -1;

            }
        }


    if(filepath == NULL){
        printf("Enter filepath\n");
        return -1;
    }

    if(newFile){
        DBFD = createFile(filepath);
        
        if(DBFD == STATUS_ERROR){
            printf("Unable to create DB file\n");
            return -1;
        }

        if(create_db_header(DBFD, &DBFDHR) == STATUS_ERROR){
            printf("Error creating BD header\n");
            return STATUS_ERROR;
        }
        
    }

    else{
        DBFD = openFile(filepath);

        if(DBFD == -1){
            printf("Error opening file\n");
            return STATUS_ERROR;
        }

        if(validate_db_header(DBFD, &DBFDHR) == STATUS_ERROR){
            printf("Couldn't validate DB header\n");
            return STATUS_ERROR;
        }

    }


    output_file(DBFD, DBFDHR);


    return 0;
}