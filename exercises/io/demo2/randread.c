#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
 
int main(int argc, char **argv) {
  unsigned int io_size;
  unsigned long long file_size, working_set_size;
  off_t offset;
  struct stat st;
  int i=0;
  char *buf;
  long lseek_ret;
  int read_ret;

  if(argc!=3) errx(1,"Usage: <Filename> <I/O Size in bytes>");
  io_size=atoi(argv[2]);
 
  int fd = open(argv[1],O_RDWR);
  if(fd<0) perror("Can't open file"),exit(1);

  if (fstat(fd, &st) < 0) perror("fstat"),exit(1);
  file_size= st.st_size;

  buf = malloc(io_size);
  if(!buf)  perror("Can't allocate aligned memory"),exit(1);
 
  while(i<file_size) {
    offset = rand()%file_size;
    lseek_ret = lseek(fd, offset, SEEK_SET);
    if(lseek_ret < 0) perror("Error in lseek"),exit(1);
    read_ret = read(fd, buf, io_size);
    if (read_ret < 0) perror("Error in read"),exit(1);
    i += io_size;
  }
  free(buf);
  close(fd);
  return 0;
}

