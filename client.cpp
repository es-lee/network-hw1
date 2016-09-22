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
#define PSIZE 50

int set_user_id();
int login(int, int);

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
      char msg [1024];
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
      char msg [1024];
      int str_len = read(sock_fd, msg, MAX_LEN);
      if(str_len == 0)
        exit(0);
      msg[str_len]=0;
    }
  }

  close(sock_fd);
  return 0;
}

int set_user_id()
{
  char tmp;
  printf("Enter your ID : ");
  scanf("%c", &tmp);
  int id = tmp-'0';
  if (id < 1 || id > 9)
  {
    perror("invalid id. byebye\n");
    exit(-1);
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
  printf("%d\n", buf[1]-'0');
    //TODO: 통신부
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
