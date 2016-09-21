#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define SIZE 100

int main (int argc, char **argv) {

  int sock_fd_server = 0;
  struct sockaddr_in server_addr;

  fd_set reads, tmps;
  int fd_max;

  char msg[SIZE];
  int str_len;
  struct timeval timeout;

  // 서버 소켓 생성 (TCP)
  sock_fd_server = socket(PF_INET, SOCK_STREAM, 0);

  // 초기화
  memset(&server_addr, 0, sizeof(struct sockaddr_in));

  // server setting
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(20421);

  bind(sock_fd_server, (struct sockaddr*) &server_addr, sizeof(struct sockaddr_in));

  listen(sock_fd_server, 5);

  //TODO: select() 사용하여 멀티플렉싱 서버 구현
  FD_ZERO(&reads);
  FD_SET(sock_fd_server, &reads);
  fd_max = sock_fd_server;

  while (1)
  {
    int fd;
    int sock_fd_client, c_len;
    struct sockaddr_in c_addr;

    tmps = reads;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    select(fd_max + 1, &tmps, 0, 0, &timeout);

printf("select!!!!!!!!!!!!!!!\n");
    for (fd = 0; fd <= fd_max; fd++)
    {
      if (FD_ISSET(fd, &tmps))
      {
printf("1\n");
        if (fd == sock_fd_server)
        {
printf("2\n");
          c_len = sizeof(c_addr);
printf("4\n");
          sock_fd_client = accept(sock_fd_client, (struct sockaddr *) &c_addr, &c_len);
printf("5\n");
          FD_SET(sock_fd_client, &reads);
printf("6\n");

          if (fd_max < sock_fd_client)
            fd_max = sock_fd_client;

          printf("client connected : fd%d\n", sock_fd_client);
        }
        else
        {
printf("3\n");
          str_len = read(fd, msg, SIZE);
          if (str_len == 0)
          {
            FD_CLR(fd, &reads);
            close(fd);
            printf("client disconnected : fd%d\n", fd);
          }
          else
          {
            write(fd, msg, str_len);
          }
        }
      }
    }
  }
  //TODO: 로그인, 디액, 메시지큐, 메시지 처리

  close(sock_fd_server);
  return 0;
}
