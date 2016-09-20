#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define MAX_LEN 140
#define DUMMY 11111
int login();
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

  int user_id = DUMMY;
  login(&user_id);

  char cmd;
  char buf [MAX_LEN];
  int dst = DUMMY;

  do {
    printf("command : ");
    scanf(" %c", &cmd);

  // 메시지보내기
    if (cmd == 's')
    {
      send_msg(&dst, &buf);
    }
  // 메시지읽기
    else if (cmd == 'r')
    {
      read_msg(sock_fd, buf);
    }
  } while (cmd != 'q');


  close(sock_fd);
  return 0;
}

int login(int * user_id)
{
  int login = 1;
  while (login)
  {
    //TODO: 통신부
    printf("id : ");
    scanf("%d", user_id);
    printf("your id is %d.\n", *user_id);

    //기다렸다가 로그인 성공
    login = 0;
    printf("Log on success!\n");
    //실패
    printf("Log on failed! (invalid ID)\n");
  }
  return login;
}

int send_msg(int * dst, char * buf) {
  //TODO: 패킷 만들어서 보내기?

  printf("R: ");
  scanf("%d", dst);
  printf("M: ");
  scanf("%139s", buf);
  printf("%s\n", buf);

  //TODO: 제대로 보내졌는지 확인하기
  //서버에서 반응 보내주기 & 타임아웃
  //타임아웃이면 타임아웃 알려주고 적당히 대응하기
  return 0;
}

int read_msg(int sock_fd, char * msg) {
  //TODO: 패킷 받기, 파싱
  //메시지 다 받은걸 어떻게 알지?
  //1. 서버에서 알려준다
  //2. 타임아웃
  //근데 타임아웃을 보내는데서도 체크하면 메시지 받을때는 서버가 다 보낸건지
  //아니면 그냥 타임아웃 된 건지 어떻게 구분을하지
  int msg_len = read(sock_fd, msg, MAX_LEN-1);
  return 0;
}
