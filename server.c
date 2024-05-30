#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include "sock.h"

void report(const char* msg, int terminate) {
  perror(msg);
  if (terminate) exit(-1); /* failure */
}

int main(int argc, char* argv[]) {

    if(argc != 2) {
		perror("Please specify the target word\n");
		exit(-1);
	}

    char *targetWord = argv[1];

    int fd = socket(AF_INET,     /* network versus AF_LOCAL */
                  SOCK_STREAM, /* reliable, bidirectional, arbitrary payload size */
                  0);          /* system picks underlying protocol (TCP) */
    if (fd < 0) report("socket", 1); /* terminate */

    /* bind the server's local address in memory */
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));          /* clear the bytes */
    saddr.sin_family = AF_INET;                /* versus AF_LOCAL */
    saddr.sin_addr.s_addr = htonl(INADDR_ANY); /* host-to-network endian */
    saddr.sin_port = htons(PortNumber);        /* for listening */

    if (bind(fd, (struct sockaddr *) &saddr, sizeof(saddr)) < 0)
        report("bind", 1); /* terminate */

    /* listen to the socket */
    if (listen(fd, MaxConnects) < 0) /* listen for clients, up to MaxConnects */
        report("listen", 1); /* terminate */

    fprintf(stderr, "Listening on port %i for clients...\n", PortNumber);
    /* a server traditionally listens indefinitely */
    while (1) {
        struct sockaddr_in caddr; /* client address */
        int len = sizeof(caddr);  /* address length could change */

    int client_fd = accept(fd, (struct sockaddr*) &caddr, &len);  /* accept blocks */
    if (client_fd < 0) {
      report("accept", 0); /* don't terminate, though there's a problem */
      continue;
    }
    
    char buffer[BuffSize]; 
    char *lines;
    int ret;
    char *needleCheck;
    char *result = (char*)malloc(sizeof(char));
    char *bufferOut = (char*)malloc(sizeof(char));
    for (;;) {
      ret = read (client_fd, buffer, sizeof(buffer));
      if (ret == -1) {
        perror ("read");
        exit (EXIT_FAILURE);
      }
      
      buffer[sizeof(buffer) - 1] = 0;
      lines = strtok(buffer, "\n");
      
      // Prints file to server terminal
      while (lines != NULL) {
        needleCheck = strstr (lines, targetWord);
        if (needleCheck != NULL) {
          result = strcat(result, lines);
          result = strcat(result, "\n");
        }
        lines = strtok(NULL, "\n");
      }
      printf("%s\n", result);
    }
      
    sprintf (bufferOut, "%s", result);
    ret = write (client_fd, bufferOut, sizeof(bufferOut));
    if (ret == -1) {
      perror ("write");
      exit (EXIT_FAILURE);
    }
    
    close (client_fd); /* break connection */
  }  /* while(1) */
  return 0;
}
