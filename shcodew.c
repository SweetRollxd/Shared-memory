/* Программа, записывающая свой исходный код в область разделяемой памяти.*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
    int shmid;
    char pathname[] = "mem.c";
    key_t key;
    char *output;

    if((key = ftok(pathname, 0)) < 0){
        printf("Cant generate the key!\n");
        exit(-1);
    }

    int fd;
    if((fd = open("shcodew.c", O_RDONLY)) < 0){
        printf("Cant open the source file!\n");
        exit(-1);
    }
    struct stat statistics;
    fstat(fd, &statistics);
    int size = statistics.st_size;
    printf("Size: %d\n", size);

    if((shmid = shmget(key, size, IPC_CREAT | 0666)) < 0){
        printf("Can\'t create shared memory\n");
        exit(-1);
    }

    if((output = shmat(shmid, NULL, 0)) == (char *)(-1)){
        printf("Can\'t attach shared memory\n");
        exit(-1);
    }

    if(read(fd, output, size) < 0){
        printf("Can\'t read the file\n");
        exit(-1);
    }
    if (shmdt(output) < 0){
        printf("Can\'t detach shared memory\n");
        exit(-1);
    }

    close(fd);
    printf("Exit...\n");

}