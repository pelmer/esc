/*****************  Demo of Gather Write Over Internet Socket  ****************/
#include "funcs.h"

main() {
  int active;
  char *s1;
  struct sockaddr_in sin; 
  char *s2;
  struct iovec strings[3];
  int n;
  char *s3;

  active = socket(AF_INET, SOCK_STREAM, 0);
  memset(&sin, 0, sizeof(struct sockaddr_in));

  sin.sin_port = htons(4444);    /*****  Fill up sockaddr_in.  *****/
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = inet_addr("0.0.0.0");

  Connect(active, (struct sockaddr *) &sin, sizeof(sin));

  s1 = (char *) Malloc(20);      /***  Fill up strings to be gathered. ***/
  strcpy(s1, "This is string #1.");    
  s2 = (char *) Malloc(30);
  strcpy(s2, "String #2 is a bigger string.");
  s3 = (char *) Malloc(40);
  strcpy(s3, "This is an even bigger string than #2!");

  strings[0].iov_base = (caddr_t) s1;  /**  Attach strings to iovec array. **/
  strings[0].iov_len  = 20;
  strings[1].iov_base = (caddr_t) s2;
  strings[1].iov_len  = 30;
  strings[2].iov_base = (caddr_t) s3;
  strings[2].iov_len  = 40;

  n = writev(active, strings, 3); /*** Send gathered strings thru socket. ***/

  printf("%d bytes sent to server!!\n\n", n);
  close(active);
}

