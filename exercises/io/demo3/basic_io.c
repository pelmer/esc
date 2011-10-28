#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main() {

  int fd = open("bigfile.dat", O_RDONLY);
  if (fd < 0) {
    perror("open");
    exit(1);
  }

  struct stat st;
  if (fstat(fd, &st)) {
    perror("fstat");
    exit(1);
  }

  long long offset = 0;
  long long filesize = st.st_size;
  printf("File length: %lld\n", filesize);
  char buf[1];
  while (offset < filesize) {
    if(pread(fd, buf, 1, offset) < 0) {
      perror("pread");
      exit(1);
    }
    // 8192: the OS reads from disk half file
    // 4096: the OS reads from disk the entire file
    offset += 4096;
  }

  return 0;
}

