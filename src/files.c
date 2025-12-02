#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include "common.h"

// STATUS_ERROR | STATUS_SUCCESSFUL

int openFile(char *filepath){

    int fd = open(filepath, O_RDWR);

    if(fd == -1){
        perror("Open");
        return STATUS_ERROR;
    }

    return fd;
}

int createFile(char *filepath){

    int fd = open(filepath, O_RDONLY);
    
    if(fd != -1){
       close(fd);
       printf("File Already Exists\n");
       return STATUS_ERROR;
    }

    fd = open(filepath, O_RDWR | O_CREAT, 0644);

    if(fd == -1){
        close(fd);
        perror("Creation");
        return STATUS_ERROR;

    }
    
    return fd;

}