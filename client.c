#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define MAX_LEN 140
#define DUMMY 11111

int send_msg();
int read_msg();

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
  int user_id = DUMMY;
  printf("id : ");
  scanf("%d", &user_id);
  printf("your id is %d.\n", user_id);

  char cmd;
  char buf [MAX_LEN];
  int dst = DUMMY;
  do {
    printf("command : ");
    scanf(" %c", &cmd);

  //TODO: 메시지보내기
    if (cmd == 's')
    {
      send_msg(&dst, &buf);
    }
  //TODO: 메시지읽기
    else if (cmd == 'r')
    {
      read_msg(sock_fd, buf);
    }


  } while (cmd != 'q');


  close(sock_fd);
  return 0;
}

int send_msg(int * dst, char * buf) {

  printf("R: ");
  scanf("%d", dst);
  printf("M: ");
  scanf("%139s", buf);
  printf("%s\n", buf);
  return 0;
}

int read_msg(int sock_fd, char * msg) {
  //TODO: 프로토콜 파싱
  //메시지 다 받은걸 어떻게 알지? 서버에서 알려주거나 timeout
  int msg_len = read(sock_fd, msg, MAX_LEN-1);
  return 0;
}
