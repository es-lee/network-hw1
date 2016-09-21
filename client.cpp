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
  }
  return login;
}
