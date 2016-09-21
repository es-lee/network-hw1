#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define SIZE 2048

int main (int argc, char **argv) {

  int sock_fd_server = 0;
  struct sockaddr_in server_addr;

  fd_set reads, tmps;
  int fd_max;

  char buf[SIZE];
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

  // TODO: select() 사용하여 멀티플렉싱 서버 구현
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
int foo;
    foo=select(fd_max + 1, &tmps, 0, 0, &timeout);

    for (fd = 0; fd <= fd_max; fd++)
    {
      if (FD_ISSET(fd, &tmps))
      {
        if (fd == sock_fd_server)
        {
          c_len = sizeof(c_addr);
          sock_fd_client = accept(sock_fd_server, (struct sockaddr *) &c_addr,(socklen_t *) &c_len);
          FD_SET(sock_fd_client, &reads);

          if (fd_max < sock_fd_client)
            fd_max = sock_fd_client;

          printf("client connected : fd%d\n", sock_fd_client);
        }
        else
        {
          // TODO: 로그인, 디액, 메시지큐, 메시지 처리
          str_len = read(fd, buf, SIZE);
          if (str_len == 0)
          {
            FD_CLR(fd, &reads);
            close(fd);
            printf("client disconnected : fd%d\n", fd);
          }
          else
          {
            write(fd, buf, str_len);
          }
        }
      }
    }
  }

  close(sock_fd_server);
  return 0;
}
