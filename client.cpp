#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX_LEN 140

int set_user_id();
int login(int, int);
int send_msg();
int read_msg();

int main (int argc, char **argv) {

  int sock_fd = 0;
  pid_t pid;
  char msg [1024];
  int str_len;
  struct sockaddr_in server_addr;

  sock_fd = socket(PF_INET, SOCK_STREAM, 0);

  memset(&server_addr, 0, sizeof(struct sockaddr_in));

  server_addr.sin_family = AF_INET;
  // for localhost
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  // for cn.snucse.org
  //server_addr.sin_addr.s_addr = inet_addr("147.46.240.40");
  server_addr.sin_port = htons(20421);

  connect(sock_fd, (struct sockaddr*) &server_addr, sizeof(server_addr));

  int user_id = set_user_id();
  login(user_id, sock_fd);

  pid = fork();
  if (pid == 0)
  {
    while(1)
    {
      fgets(msg, MAX_LEN, stdin);
      if(!strcmp(msg, "q\n"))
      {
        shutdown(sock_fd, SHUT_WR);
        close(sock_fd);
        exit(0);
      }
      write(sock_fd, msg, strlen(msg));
    }
  }
  else {
    while(1)
    {
      str_len = read(sock_fd, msg, MAX_LEN);
      if(str_len == 0)
        exit(0);
      msg[str_len]=0;
    }
  }

  /*
  int user_id = DUMMY;
  login(&user_id);

  char cmd [MAX_LEN];
  char buf [MAX_LEN];
  int dst = DUMMY;

  do {
    printf("command : ");
    scanf("%s", cmd);

  // 메시지보내기
    if (strcmp(cmd, "s") == 0)
    {
      send_msg(&dst, &buf);
    }
  // 메시지읽기
    else if (strcmp(cmd, "r") == 0)
    {
      read_msg(sock_fd, buf);
    }
  } while (strcmp(cmd, "q") != 0);
*/

  close(sock_fd);
  return 0;
}

int set_user_id()
{
  int id;
  printf("Enter your ID : ");
  scanf("%d", &id);
  return id;
}

int login(int user_id, int sock_fd)
{
  int login = 1;
  std::string buf = "0 ";
  buf += std::to_string(user_id);
std::cout << buf;
  while (login)
  {
    //TODO: 통신부
    write(sock_fd, buf.c_str(), buf.length());
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
  //
  //위에거 다 치우고. 액티브 된 상태에서 항상 오는 메시지를 받아야하니까 이걸
  //어떻게 받을지 고민을 해야한다. 스레드를 하나 더 돌리든 프로세스 하나를 더
  //돌리든. 메시지가 잘 전송된건 tcp가 보장해주는걸로.
  //일단 돌아가는거 만들고 나서 ack추가할거면 하든가.
  int msg_len = read(sock_fd, msg, MAX_LEN-1);
  printf("%d\n",msg_len);
  return 0;
}
