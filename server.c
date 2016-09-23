#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define PSIZE 144
#define MAX_CONN 30
#define ONLINE 1
#define OFFLINE 0
#define INVALIDID 0
#define PORT 20421

int invalidlogin (int);
int fdfindbyid (int);

// store client information
typedef struct {
  struct sockaddr_in c_addr;    // client ip addr, port
  int id;
  int active;
} ClientState;

ClientState clstate[MAX_CONN];

int main () {
  // server socket setting (TCP)
  int sock_fd_server = socket(PF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_addr = {};
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(PORT);

  bind(sock_fd_server, (struct sockaddr*) &server_addr, sizeof(struct sockaddr_in));

  listen(sock_fd_server, 5);

  // setting for multiplexing
  fd_set reads;
  FD_ZERO(&reads);
  FD_SET(sock_fd_server, &reads);
  int fd_max = sock_fd_server;

  // multiplexing by using select()
  while (1)
  {
    fd_set tmps = reads;
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    select(fd_max + 1, &tmps, 0, 0, &timeout);

    int fd;
    for (fd = 0; fd <= fd_max; fd++)
    {
      if (FD_ISSET(fd, &tmps))    // find state-changed fd
      {
        if (fd == sock_fd_server) // accept connection request for client
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
        else // recieve data from connected client
        {
          char buf[PSIZE];
          int str_len = read(fd, buf, PSIZE);
          printf("get msg from fd%d\n", fd);

          if (str_len <= 0) // client deactiaved
          {
            clstate[fd].id = INVALIDID;
            clstate[fd].active = OFFLINE;
            FD_CLR(fd, &reads);
            close(fd);
            printf("client disconnected : fd%d\n", fd);
          }
          else if (buf[0] == '0') // login
          {
            if (invalidlogin(buf[1]-'0')) // invalid id check(ID already logged on)
            {
              // login failed
              clstate[fd].id = INVALIDID;
              clstate[fd].active = OFFLINE;
              write(fd, "01", PSIZE);
              FD_CLR(fd, &reads);
              close(fd);
              printf("Invalid login : client disconnected : fd%d\n", fd);
            }
            else
            {
              // login success
              printf("fd%d 가 id(%d)로 로그인함\n", fd, buf[1]-'0');
              clstate[fd].id = buf[1] - '0';
              clstate[fd].active = ONLINE;

              // send log on success sign
              write(fd, "00", PSIZE);

              // send all saved messages for user
              char fname[2];
              fname[0] = buf[1];
              fname[1] = '\0';
              FILE * fp = fopen(fname, "a");
              fclose(fp);
              fp = fopen(fname, "r");
              while (fread(buf, 1, PSIZE, fp) != 0)
                write(fd, buf, PSIZE);
              printf("저장된 메시지를 fd%d에게 모두 전달함\n", fd);
              fclose(fp);
              fp = fopen(fname, "w");
              fclose(fp);
            }
          }
          else  //processing message from clients
          {
            printf("메시지가 왔어요! fd%d가 보냄\n", fd);
            int recv_id = buf[1] - '0';
            int recv = fdfindbyid(recv_id);

            if (recv) //reciever logged on
            {
              char wbuf[PSIZE];
              wbuf[0] = '1';
              wbuf[1] = buf[2];
              strncpy(wbuf+2, buf+3, PSIZE-3);
              write(recv, wbuf, PSIZE);
              printf("메시지 바로 전달함: %s\n", wbuf);
            }
            else  //reciever deactivated
            {
              printf("store message.\n");
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

//find fd of logged on user by id
int fdfindbyid(int id)
{
  int i;
  for (i = 4; i < MAX_CONN; i++)
  {
    if(clstate[i].id == id)
      return i;
  }
  return 0;
}

//check id already logged on
int invalidlogin(int id)
{
  int i;
  for (i = 4; i < MAX_CONN; i++)
  {
    if (clstate[i].id == id)
      return -1;
  }
  return 0;
}
