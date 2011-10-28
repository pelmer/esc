#include        <sys/types.h>
#include        <sys/stat.h>
#include        <sys/mman.h>    /* mmap() */
#include        <fcntl.h>
#include        <stdlib.h>
#include        <unistd.h>
#include        <err.h>

int main(int argc, char **argv) {
  int fdin, fdout;
  char *src;
  struct stat statbuf;

  if (argc != 3) errx(1,"usage: %s <fromfile> <tofile>", argv[0]);

  // open input file
  if ((fdin = open(argv[1], O_RDONLY)) < 0) perror("open"),exit(1);

  // open output file
  if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0)
    perror("open"),exit(1);

  // get the size of the input file
  if(fstat(fdin, &statbuf) < 0) perror("fstat"),exit(1);

  // map the input file to memory
  if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_FILE | MAP_SHARED, fdin, 0)) < 0)
    perror("mmap"),exit(1);

  // dump the memory mapped to the input file to the output file
  if(write(fdout, src, statbuf.st_size) < 0) perror("write"),exit(1);

  // block until file is on disk
  // Comment next line out if you want to return as soon as the file is
  // written in the page cache
  if (fsync(fdout) < 0) perror ("msync"),exit(1); 

  // unmap
  if(munmap(src, statbuf.st_size) < 0) perror("munmap"),exit(1);

  return 0;
}

