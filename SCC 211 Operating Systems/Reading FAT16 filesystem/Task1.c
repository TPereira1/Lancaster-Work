#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//Program opens text file h.txt and jumps to position 30 then reads the next 30 character then outputs them.
void main() {
  int fd = open("h.txt", O_RDONLY);
  lseek(fd, 30, SEEK_SET);
  char storage[30];
  read(fd, storage,sizeof(storage));
  printf("%s\n", storage);
  close(fd);
}