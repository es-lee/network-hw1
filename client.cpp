#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PSIZE 50

int set_user_id();
int login(int, int);
int isvalidid(int id);

int main () {
  int sock_fd = socket(PF_INET, SOCK_STREAM, 0);

  sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(struct sockaddr_in));

  server_addr.sin_family = AF_INET;
  // for localhost
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_addr.sin_port = htons(20421);

  connect(sock_fd, (struct sockaddr*) &server_addr, sizeof(server_addr));

  int user_id = set_user_id();
  login(user_id, sock_fd);


  pid_t pid = fork();
  if (pid == 0)
  {
    while(1)
    {
      char msg [PSIZE];
      fgets(msg, PSIZE, stdin);
      if (!strcmp(msg, "q\n"))
      {
        shutdown(sock_fd, SHUT_WR);
        close(sock_fd);
        printf("꺼지란말이야!!!! Succeding you, father");
        kill(getppid(), SIGKILL);
        exit(0);
      }
      else if(!strcmp(msg, "s\n"))
      {
        // TODO: 메시지 쓰기
        printf("=====Send Message=====\n");
        while(1)
        {
          printf("To : ");
          fgets(msg, PSIZE, stdin);

          if (isvalidid(msg[0]-'0'))
            break;

          printf("Invalid reciever id\n");
        }
        char buf[PSIZE];
        buf[0] = '1';
        // TODO: id valid check
        buf[1] = msg[0];
        printf("Content :\n");
        fgets(msg, PSIZE, stdin);
        printf("[%s]\n", msg);

        //write(sock_fd, msg, strlen(msg));
      }
      else if(!strcmp(msg, "r\n"))
      {
        // TODO: 메시지 읽은거 보여주기
        printf("=====Read Message=====\n");
      }
    }
  }
  else {
    while(1)
    {
      // TODO: 메시지 계속 읽어서 어딘가 저장하기
      char msg [PSIZE];
      int str_len = read(sock_fd, msg, PSIZE);
      if(str_len != 0)
      {
        printf("Read failed\n");
        exit(0);
      }
      msg[str_len]=0;
    }
  }

  close(sock_fd);
  return 0;
}

int isvalidid(int id)
{
  if (id < 1 || id > 9)
    return 0;
  return 1;
}

int set_user_id()
{
  char tmp;
  printf("Enter your ID : ");
  scanf("%c", &tmp);
  int id = tmp-'0';
  if (!isvalidid(id))
  {
    perror("invalid id. byebye\n");
    exit(0);
  }
  else
    return id;
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
