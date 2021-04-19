/* Программа, считывающая исходный код программы shcodew из области разделяемой памяти.*/

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
    char *input;

    if((key = ftok(pathname, 0)) < 0){
        printf("Cant generate the key!\n");
        exit(-1);
    }

    struct stat buf;
    stat("shcodew.c", &buf);
    int size = buf.st_size;

    if((shmid = shmget(key, size, 0)) < 0){
        printf("Can\'t get shared memory id\n");
        exit(-1);
    }
    if((input = (char *)shmat(shmid, NULL, 0)) == (char *)(-1)){
        printf("Can\'t attach shared memory\n");
        exit(-1);
    }
    for(int i = 0; i < size; i++){
        printf("%c", input[i]);
    }

    if (shmdt(input) < 0){
        printf("Can\'t detach shared memory\n");
        exit(-1);
    }
    shmctl(shmid, IPC_RMID, NULL);
}