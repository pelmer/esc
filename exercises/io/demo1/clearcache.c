#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
int main(int argc, char *argv[]) {
  if (argc==1) {
     puts(" usage: clearcache <filename>\n");
     return 0;
  }
  int fd;
  fd = open(argv[1], O_RDONLY);
  if (fd < 0) { perror("open"); return 1; }

  // flushes all data buffers of the file to disk
  if (fdatasync(fd)<0) { perror("fdatasync"); return 1; }

  // tell the kernel that we do not need this file soon
  // the kernel will immediately remove the associated pages from the cache
  if (posix_fadvise(fd, 0,0,POSIX_FADV_DONTNEED) < 0)
    { perror("posix_fadvice"); return 1; }

  close(fd);

  return 0;
}
