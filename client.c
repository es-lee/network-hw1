#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main (int argc, char **argv) {
  int sock_fd = 0;
  struct sockaddr_in server_addr;

  char *msg = malloc(1024);
  int msg_len = 0;

  sock_fd = socket(PF_INET, SOCK_STREAM, 0);

  memset(&server_addr, 0, sizeof(struct sockaddr_in));

  server_addr.sin_family = AF_INET;
  // for localhost
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  // for cn.snucse.org
  //server_addr.sin_addr.s_addr = inet_addr("147.46.240.40");
  server_addr.sin_port = htons(20421);

  connect(sock_fd, (struct sockaddr*) &server_addr, sizeof(server_addr));

  //TODO: 로그인
  //TODO: 메시지보내기
  //TODO: 메시지읽기
  //TODO: 디액
  msg_len = read(sock_fd, msg, 1024 - 1);


  close(sock_fd);
  return 0;
}
