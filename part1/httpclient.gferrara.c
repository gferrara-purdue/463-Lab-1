#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINE 512

int open_clientfd(char *hostname, int port)
{
  int clientfd;
  struct hostent *hp;
  struct sockaddr_in serveraddr;

  if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    return -1; /* check errno for cause of error */

  /* Fill in the server's IP address and port */
  if ((hp = gethostbyname(hostname)) == NULL)
    return -2; /* check h_errno for cause of error */
  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  bcopy((char *)hp->h_addr_list[0],
        (char *)&serveraddr.sin_addr.s_addr, hp->h_length);
  serveraddr.sin_port = htons(port);

  /* Establish a connection with the server */
  if (connect(clientfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
    return -1;
  return clientfd;
}


/* usage: ./echoclient host port */
int main(int argc, char **argv)
{
  int clientfd, port;
  char *host, *pathname;
  char buf[MAXLINE];
  int b_read;
  //char response_code[3] = "200";
  char *http_response = NULL;
  char *file_path;

  host = argv[1];
  port = atoi(argv[2]);
  pathname = argv[3];
  bzero(buf, MAXLINE);
  sprintf(buf, "GET %s HTTP/1.0\r\n\r\n", pathname);

  // -------------------------1-------------------------
  // 1) Handle the connection for the first GET request.
  clientfd = open_clientfd(host, port);
  if (clientfd < 0){
    printf("Error opening connection \n");
    exit(0);
  }
  write(clientfd, buf, strlen(buf));

  // 1a) Read the header and content of the first request.
  b_read = read(clientfd, buf, MAXLINE);
  //print[b_read] = '\0';
  http_response = strstr(buf, "200");

  if (!http_response) {
    printf("Error: 200 response not received.\n");
    return EXIT_FAILURE;
  }
  else {
    fprintf(stdout, buf);
    //fputs(print, stdout);
  }

  // 1b) Identify the new file path.
  file_path = strstr(buf, "\r\n\r\n");
  if (file_path) {
    file_path += sizeof(char) * 4;
  }
  char convert = file_path[0];
  int i = 0;
  while (convert != '\n')
    {
      i++;
      convert = file_path[i];
    }
  file_path[i] = '\0';

  // 1c) Store the new file path and clean up.
  sprintf(buf, "GET %s HTTP/1.0\r\n\r\n", file_path);
  close(clientfd);

  // -------------------------2-------------------------
  // 2) Handle the connection for the second GET request.
  clientfd = open_clientfd(host, port);
  if (clientfd < 0){
    printf("Error opening connection 2.\n");
    exit(0);
  }
  write(clientfd, buf, strlen(buf));
  bzero(buf, MAXLINE);

  // 1a) Spit back the header and content of the first request.
  while ( (b_read = read(clientfd, buf, MAXLINE-1)) > 0 ) {
    buf[b_read] = '\0';
    fputs(buf, stdout);
    bzero(buf, MAXLINE);
  }

  // Clean up, and exit.
  close(clientfd);
  exit(0);
}
