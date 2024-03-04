#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX 80 


// function to handle in coming messages and chet bewteen the client and the server
void func(int connfd) {
  char buff[MAX];
  int n;

  for (;;) {
    bzero(buff, MAX);

    read(connfd, buff, sizeof(buff));
    printf("ONE %s \n ", buff);
    bzero(buff, MAX);
    n = 0;

    // copy the server message into the buffer
    // while ((buff[n++] = getchar()) != '\n')
    //   ;


    strcpy(buff, "+PONG\r\n");

    write(connfd, buff, strlen(buff));

    if (strncmp("exit", buff, 4) == 0) {
      printf("Server exit...\n");
      break;
    }
  }
}

int main() {
  // Disable output buffering
  setbuf(stdout, NULL);

  // You can use print statements as follows for debugging, they'll be visible
  // when running tests.
  printf("Logs from your program will appear here!\n");

  // Uncomment this block to pass the first stage

  int server_fd, client_addr_len, connfd;
  struct sockaddr_in client_addr;

  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    printf("Socket creation failed: %s...\n", strerror(errno));
    return 1;
  }

  // Since the tester restarts your program quite often, setting REUSE_PORT
  // ensures that we don't run into 'Address already in use' errors
  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) <
    0) {
    printf("SO_REUSEPORT failed: %s \n", strerror(errno));
    return 1;
  }

  struct sockaddr_in serv_addr = {
      .sin_family = AF_INET,
      .sin_port = htons(6379),
      .sin_addr = {htonl(INADDR_ANY)},
  };

  if (bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) != 0) {
    printf("Bind failed: %s \n", strerror(errno));
    return 1;
  }

  int connection_backlog = 5;
  if (listen(server_fd, connection_backlog) != 0) {
    printf("Listen failed: %s \n", strerror(errno));
    return 1;
  }

  printf("Waiting for a client to connect...\n");
  client_addr_len = sizeof(client_addr);

  connfd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
  if (connfd < 0) {
    printf("server accept failed...\n");
    exit(0);
  }
  else
    printf("server accept the client...\n");
  func(connfd);


  close(server_fd);

  return 0;
}
