#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define SIZE 2048
#define PSIZE 50
#define MAX_CONN 30
#define ONLINE 1
#define OFFLINE 0
#define INVALIDID 0
#define PORT 20431

int invalidlogin (int);
int fdfindbyid (int);

typedef struct {
  sockaddr_in c_addr;
  int id;
  int active;
} ClientState;

ClientState clstate[MAX_CONN];

int main () {
  // 서버 소켓 생성 (TCP)
  int sock_fd_server = socket(PF_INET, SOCK_STREAM, 0);

  // server setting
  struct sockaddr_in server_addr = {};
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(PORT);

  bind(sock_fd_server, (struct sockaddr*) &server_addr, sizeof(struct sockaddr_in));

  listen(sock_fd_server, 5);

  // select() 사용하여 멀티플렉싱 서버 구현
  fd_set reads;
  FD_ZERO(&reads);
  FD_SET(sock_fd_server, &reads);
  int fd_max = sock_fd_server;

  while (1)
  {
    fd_set tmps = reads;
    timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    select(fd_max + 1, &tmps, 0, 0, &timeout);

    for (int fd = 0; fd <= fd_max; fd++)
    {
      if (FD_ISSET(fd, &tmps))
      {
        if (fd == sock_fd_server)
        {
          struct sockaddr_in c_addr;
          int c_len = sizeof(c_addr);
          int sock_fd_client = accept(sock_fd_server, (struct sockaddr *) &c_addr,(socklen_t *) &c_len);
          FD_SET(sock_fd_client, &reads);

          if (fd_max < sock_fd_client)
            fd_max = sock_fd_client;

          clstate[sock_fd_client].c_addr = c_addr;
          printf("client connected : fd%d\n", sock_fd_client);
        }
        else
        {
          // TODO: 메시지큐, 메시지 처리
printf("before read\n");
          char buf[SIZE];
          int str_len = read(fd, buf, PSIZE);
printf("after read\n");
          if (str_len <= 0)
          {
            // deactivate
            clstate[fd].id = INVALIDID;
            clstate[fd].active = OFFLINE;
            FD_CLR(fd, &reads);
            close(fd);
            printf("client disconnected : fd%d\n", fd);
          }
          else if (buf[0] == '0')
          {
            //login
            if (invalidlogin(buf[1]-'0'))
            {
            //login fail : 이미 로그인한 아이디로 로그인
              clstate[fd].id = INVALIDID;
              clstate[fd].active = OFFLINE;
              write(fd, "01", PSIZE);
              FD_CLR(fd, &reads);
              close(fd);
              printf("Invalid login : client disconnected : fd%d\n", fd);
            }
            else
            {
              //login success
              printf("fd%d 가 id(%d)로 로그인함\n", fd, buf[1]-'0');
              clstate[fd].id = buf[1] - '0';
              clstate[fd].active = ONLINE;
              write(fd, "00", PSIZE);
              // TODO : 지금까지 있는 로그 다 보내주고 파일 초기화
              char fname[2];
              fname[0] = buf[1];
              fname[1] = '\0';
              FILE * fp = fopen(fname, "r");
              while (fread(buf, 1, PSIZE, fp) != 0)
                write(fd, buf, PSIZE);
              printf("저장된 메시지를 fd%d에게 모두 전달함\n", fd);
              fclose(fp);
              fp = fopen(fname, "w");
              fclose(fp);
            }
          }
          else
          {
            //msg 처리
            printf("메시지가 왔어요! fd%d가 보냄\n", fd);
            int recv_id = buf[1] - '0';
            int recv = fdfindbyid(recv_id);
            if (recv)
            {
              //바로 보내기
              char wbuf[PSIZE];
              wbuf[0] = '1';
              wbuf[1] = buf[2];
              strncpy(wbuf+2, buf+3, PSIZE-3);
              write(recv, wbuf, PSIZE);
              printf("메시지 바로 전달 프로토콜 : %s\n", wbuf);
            }
            else
            {
              //TODO: 메시지큐에 저장
              printf("메시지큐에 저장해주세요!\n");
              char fname[2];
              fname[0] = buf[1];
              fname[1] = '\0';
              FILE * fp = fopen(fname, "a");
              char wbuf[PSIZE];
              wbuf[0] = '1';
              wbuf[1] = buf[2];
              strncpy(wbuf+2, buf+3, PSIZE-3);
              fwrite(wbuf, 1, PSIZE, fp);
              fclose(fp);
            }
          }
        }
      }
    }
  }

  close(sock_fd_server);
  return 0;
}

int fdfindbyid(int id)
{
  for (int i = 4; i < MAX_CONN; i++)
  {
    if(clstate[i].id == id)
      return i;
  }
  return 0;
}

int invalidlogin(int id)
{
  for (int i = 4; i < MAX_CONN; i++)
  {
    if (clstate[i].id == id)
      return -1;
  }
  return 0;
}
