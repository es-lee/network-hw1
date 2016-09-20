#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int main (int argc, char **argv) {
	int sock_fd_server = 0;
	int sock_fd_client = 0;

	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	int client_addr_size;

	char message[] = "Hello Client!\n";

	// 서버 소켓 생성 (TDP)
	sock_fd_server = socket(PF_INET, SOCK_STREAM, 0);

	memset(&server_addr, 0, sizeof(struct sockaddr_in));

	// TCP/IP 스택의 주소 집합으로 설정
	server_addr.sin_family = AF_INET;
	// 어떤 클라이언트 주소도 접근할 수 있음
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	// 서버의 포트를 2000번으로 설정
	server_addr.sin_port = htons(2000);

	// 서버 소켓에 주소 살당
	bind(sock_fd_server, (struct sockaddr*) &server_addr,
		sizeof(struct sockaddr_in));
	// 서버 대기 상태로 설정
	listen(sock_fd_server, 5);

	client_addr_size = sizeof(struct sockaddr_in);

	// 클라이언트 요청을 수락하고, 클라이언트 소켓 할당
	sock_fd_client = accept(sock_fd_server,
		(struct sockaddr *) &client_addr,
		(socklen_t *) &client_addr_size);

	// 클라이언트에 데이터 전송
	write(sock_fd_client, message, sizeof(message));
	// 클라이언트 소켓 종료
	close(sock_fd_client);
	// 서버 소켓 종료
	close(sock_fd_server);
	return 0;
}
