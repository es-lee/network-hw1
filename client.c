#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define PORT 20421
#define PSIZE 50

int set_user_id();
int login(int, int);
int isvalidid(int id);
void readsize(char*, int);
void rmnewline(char*);

int main () {
  int sock_fd = socket(PF_INET, SOCK_STREAM, 0);

  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(struct sockaddr_in));

  server_addr.sin_family = AF_INET;
  // for cn.snucse.org;
  server_addr.sin_addr.s_addr = inet_addr("147.46.240.40");
  server_addr.sin_port = htons(PORT);

  connect(sock_fd, (struct sockaddr*) &server_addr, sizeof(server_addr));

  int user_id = set_user_id();
  while (!user_id)
    user_id = set_user_id();
  login(user_id, sock_fd);

  pid_t pid = fork();
  if (pid == 0)
  {
    while(1)
    {
      // TODO: 메시지 계속 읽어서 어딘가 저장하기
      char msg [PSIZE];
      int str_len = read(sock_fd, msg, PSIZE);
      printf("메시지 옴 : %s\n", msg);
      if(str_len <= 0)
      {
        printf("Read failed\n");
        exit(0);
      }
      msg[str_len]=0;
    }
  }
  else {
    while(1)
    {
      char msg [PSIZE];
      printf("╔═ ═ ═ ═ ═ ═ ═ ═ ═ ╗\n");
      printf("║ s : send message ║\n");
      printf("║ r : read message ║\n");
      printf("║ q : exit         ║\n");
      printf("╚═ ═ ═ ═ ═ ═ ═ ═ ═ ╝\n");
      readsize(msg, PSIZE);
      if (!strcmp(msg, "q\n"))
      {
        shutdown(sock_fd, SHUT_WR);
        close(sock_fd);
        kill(0, SIGKILL);
        exit(0);
      }
      else if(!strcmp(msg, "s\n"))
      {
        // 메시지 쓰기
        printf("=====Send Message=====\n");
        while(1)
        {
          printf("To : ");
          readsize(msg, PSIZE);

          if (isvalidid(msg[0]-'0') && strlen(msg)==2)
            break;

          printf("Invalid reciever id\n");
        }
        char buf[PSIZE];
        buf[0] = '1';
        buf[1] = msg[0];
        buf[2] = user_id + '0';
        printf("Content :\n");
        readsize(msg, PSIZE);
        // TODO: \n->\0
        rmnewline(msg);
        printf("[%s]\n", msg);
        strncpy(buf+3, msg, PSIZE-3);
        buf[PSIZE-1] = '\0';
        printf("msg send protocol : %s\n", buf);
        write(sock_fd, buf, PSIZE);
        printf("서버에 메시지 전송 완료!\n");
      }
      else if(!strcmp(msg, "r\n"))
      {
        // TODO: 메시지 읽은거 보여주기
        printf("=====Read Message=====\n");
      }
    }
  }

  close(sock_fd);
  return 0;
}

void rmnewline(char * str)
{
  int i = strlen(str);
  printf("strlen = %d\n", i);
  if (i < PSIZE-1)
    str[i-1] = '\0';
}

int isvalidid(int id)
{
  if (id < 1 || id > 9)
    return 0;
  return 1;
}

int set_user_id()
{
  char buf[PSIZE];
  printf("Enter your ID : ");
  readsize(buf, PSIZE);
  printf("id len : %d\n",strlen(buf));
  if (!isvalidid(buf[0]-'0') || strlen(buf)!=2)
  {
    printf("invalid id.\n");
    return 0;
  }
  else
    return buf[0]-'0';
}

int login(int user_id, int sock_fd)
{
  int login = 1;
  char buf[PSIZE];
  buf[0] = '0';
  buf[1] = user_id + '0';

  // 통신부
  write(sock_fd, buf, PSIZE);
  read(sock_fd, buf, PSIZE);
  if (buf[1] - '0')
  {
    printf("login failed\n");
    return -1;
  }
  printf("login success\n");
  return login;
}

void readsize(char* str, int count)
{
  fgets(str, count, stdin);
  fseek(stdin,0,SEEK_END);
  printf("버퍼 다 비움\n");
}
