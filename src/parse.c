#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "common.h"
#include "parse.h"


int create_db_header(int fd, struct dbHeader_t **headerOut){
	struct dbHeader_t *header = calloc(1, sizeof(struct dbHeader_t));
	
	if(header == NULL){
		printf("Calloc error\n");
		return STATUS_ERROR;
	}

	header->version = 0x1;
	header->count = 0;
	header->magic = HEADER_MAGIC;
	header->filesize = sizeof(struct dbHeader_t);

	*headerOut = header;

	return STATUS_SUCCESSFUL;

}

int validate_db_header(int fd, struct dbHeader_t **headerOut){

	if(fd < 0){
		printf("Bad FD\n");
		return STATUS_ERROR;
	}

	struct dbHeader_t *header = calloc(1, sizeof(struct dbHeader_t));

	if(header == NULL){
		printf("Calloc error\n");
		return STATUS_ERROR;
	}

	if(read(fd, header, sizeof(struct dbHeader_t)) != sizeof(struct dbHeader_t)){
		perror("Read");
		free(header);
		return STATUS_ERROR;
	}

	header->version = ntohs(header->version);
	header->count = ntohs(header->count);
	header->magic = ntohl(header->magic);
	header->filesize = ntohl(header->filesize);

	if(header->magic != HEADER_MAGIC){
		printf("Invalid header magic\n");
		free(header);
		return STATUS_ERROR;
	}

	if(header->version != 1){
		printf("Improper Header Version\n");
		free(header);
		return -1;
	}

	struct stat DBstat = {0};
	fstat(fd, &DBstat);

	if(header->filesize != DBstat.st_size){
		printf("Corrupted DB\n");
		free(header);
		return -1;
	}

	*headerOut = header;

	// return STATUS_SUCCESSFUL;
}

int output_file(int fd, struct dbHeader_t *DBHDR){
	
	if(fd < 0){
		printf("Invalid FD\n");
		return STATUS_ERROR;
	}
	int realcount = DBHDR->count;

	DBHDR->magic = htonl(DBHDR->magic);
	DBHDR->version = htons(DBHDR->version);
	DBHDR->count = htons(DBHDR->count);
	DBHDR->filesize = htonl(DBHDR->filesize);

	lseek(fd, 0, SEEK_SET);
	write(fd, DBHDR, sizeof(struct dbHeader_t));

	return STATUS_SUCCESSFUL;

}
