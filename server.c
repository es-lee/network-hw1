#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main (int argc, char **argv) {
  int sock_fd_server = 0;
  struct sockaddr_in server_addr;

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
  //TODO: 로그인, 디액, 메시지큐, 메시지 처리

  close(sock_fd_server);
  return 0;
}
