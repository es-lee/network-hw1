#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define PORT 20421
#define PSIZE 144
#define MAXNUM 128

int set_user_id();
int login(int, int);
int isvalidid(int id);
void readsize(char*, int);
void rmnewline(char*);
void *msgrecieve(void *);

char msglog[MAXNUM][PSIZE]; // store messages
int msgnum;                 // the number of saved messages

int main () {
  // client socket setting (TCP)
  int sock_fd = socket(PF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_addr = {};
  server_addr.sin_family = AF_INET;
  // for cn.snucse.org;
  server_addr.sin_addr.s_addr = inet_addr("147.46.240.40");
  server_addr.sin_port = htons(PORT);

  // connect to server
  connect(sock_fd, (struct sockaddr*) &server_addr, sizeof(server_addr));

  // try log on
  int user_id = set_user_id();
  while (!user_id)
    user_id = set_user_id();
  if (!login(user_id, sock_fd))   // if login failed, quit program.
    return 0;

  // create thread for recieving messages from server
  pthread_t tid;
  pthread_create(&tid, NULL, msgrecieve, (void *)&sock_fd);

  // chatting process
  while(1)
  {

    // show commands
    printf("╔═ ═ ═ ═ ═ ═ ═ ═ ═ ╗\n");
    printf("║ s : send message ║\n");
    printf("║ r : read message ║\n");
    printf("║ q : exit         ║\n");
    printf("╚═ ═ ═ ═ ═ ═ ═ ═ ═ ╝\n");

    // get command
    char msg [PSIZE];
    readsize(msg, PSIZE);

    if (!strcmp(msg, "q\n"))  // quit
    {
      shutdown(sock_fd, SHUT_WR);
      pthread_join(tid, (void **)&msg);
      break;
    }
    else if(!strcmp(msg, "s\n"))  // send message
    {
      printf("=====Send Message=====\n");

      // get reciever id
      while(1)
      {
        printf("To : ");
        readsize(msg, PSIZE);

        if (isvalidid(msg[0]-'0') && strlen(msg)==2)
          break;

        printf("Invalid reciever id\n");
      }

      // make message
      char buf[PSIZE];
      buf[0] = '1';
      buf[1] = msg[0];
      buf[2] = user_id + '0';

      // get message content
      printf("Content :\n");
      readsize(msg, PSIZE);
      rmnewline(msg);

      // make message
      strncpy(buf+3, msg, PSIZE-3);
      buf[PSIZE-1] = '\0';

      // send message to server
      write(sock_fd, buf, PSIZE);
      printf("======================\n");
    }
    else if(!strcmp(msg, "r\n"))  // read messages from archive(msglog)
    {
      printf("=====Read Message=====\n");
      int i;
      if (msgnum)
      {
        for (i = 0; i < msgnum; i++)
        {
          printf("From : %c\n", msglog[i][1]);
          printf("Content :\n");
          printf("%s\n",msglog[i]+2);
          printf("======================\n");
        }
        msgnum = 0;
      }
      else
      {
        printf("No message\n");
        printf("======================\n");
      }
    }
  }

  // connection close
  close(sock_fd);
  return 0;
}

// remove new line from string and add null character
void rmnewline(char * str)
{
  int i = strlen(str);
  if (i < PSIZE-1)
    str[i-1] = '\0';
}

// id format check
int isvalidid(int id)
{
  if (id < 1 || id > 9)
    return 0;
  return 1;
}

// get id from prompt
int set_user_id()
{
  char buf[PSIZE];
  printf("Enter your ID : ");
  readsize(buf, PSIZE);
  if (!isvalidid(buf[0]-'0') || strlen(buf)!=2)
  {
    printf("invalid id.\n");
    return 0;
  }
  else
    return buf[0]-'0';
}

// try to login
int login(int user_id, int sock_fd)
{
  int login = 1;
  char buf[PSIZE];
  buf[0] = '0';
  buf[1] = user_id + '0';

  // send login message to server
  write(sock_fd, buf, PSIZE);

  // recieve login success/fail message from server
  read(sock_fd, buf, PSIZE);

  if (buf[1] - '0')
  {
    printf("login failed\n");
    return 0;
  }
  printf("login success\n");
  return login;
}

// read string(length=count) from stdin buffer and flush
void readsize(char* str, int count)
{
  fgets(str, count, stdin);
  fseek(stdin,0,SEEK_END);
}

// thread process.
// constantly recieve messages from server and store
void *msgrecieve(void *vargp)
{
  int sock_fd = *((int *)vargp);

  while(1)
  {
    char msg [PSIZE];

    // recieve message from server
    int str_len = read(sock_fd, msg, PSIZE);

    if(str_len <= 0)
      break;

    // store message to archive
    strncpy(msglog[msgnum++], msg, PSIZE);
    msg[str_len]=0;
  }

  close(sock_fd);
  return NULL;
}
