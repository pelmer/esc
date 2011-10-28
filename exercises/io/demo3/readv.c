#include "funcs.h"

main()
{
  int passive, active, addr_len;
  char *s1; 
  struct sockaddr_in sin, client; 
  char *s2;
  int n;
  struct iovec strings[3];
  char *s3;

  passive = Socket(AF_INET, SOCK_STREAM, 0);
  memset(&sin, 0, sizeof(sin));
  sin.sin_port = htons(4444);    /*****  Fill up sockaddr_in.  *****/
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;

  Bind(passive, (struct sockaddr *) &sin, sizeof(sin)); 
  Listen(passive, 1);  /**** Announce intention to accept clients. ***/

  addr_len = sizeof(struct sockaddr_in);  /******  Listen for clients!  ******/
  if ((active = accept(passive, (struct sockaddr *) &client, &addr_len)) < 0)
  {
      perror("Accept error!");
      exit(1);
  }

  s1 = (char *) Malloc(20);  /******  Allocate space for strings to  ******/
  s2 = (char *) Malloc(30);  /******  be received from client.       ******/
  s3 = (char *) Malloc(40);

  strings[0].iov_base = (caddr_t) s1;  /***  Attach strings to iovec array. ***/
  strings[0].iov_len  = 20;
  strings[1].iov_base = (caddr_t) s2;
  strings[1].iov_len  = 30;
  strings[2].iov_base = (caddr_t) s3;
  strings[2].iov_len  = 40;

  n = readv(active, strings, 3);   /*****  Get the strings and scatter. *****/

  signal(SIGTTOU, SIG_IGN);  /**** Keep server from suspending on printf ****/

  printf("%d bytes read from client!!\n\n", n);
  printf("Strings from client are:\n");
  puts(s1); puts(s2); puts(s3);
  Close(active); Close(passive);
  exit(0);
}
