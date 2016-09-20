#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int main (int argc, char **argv) {
	int sock_fd = 0;
	// 연결할 서버 주소 정보를 위한 구조체
	struct sockaddr_in server_addr;
	// 서버에서 받아올 데이터 저장 공간
	char *message = malloc(1024);
	int message_len = 0;

	// 서버 접속을 위한 소켓(TCP) 생성
	sock_fd = socket(PF_INET, SOCK_STREAM, 0);
	// 초기화
	memset(&server_addr, 0, sizeof(server_addr));
	// TCP/IP 스택을 위한 주소 집합(Address Family)으로 설정
	server_addr.sin_family = AF_INET;
	// 연결할 서버의 주소는 로컬 호스트(127.0.0.1)로 지정
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	// 연결할 서버의 포트 번호는 2000번
	server_addr.sin_port = htons(2000);

	connect(sock_fd, (struct sockaddr*) &server_addr,
		sizeof(server_addr));

	message_len = read(sock_fd, message, 1024 - 1);

	printf("Message from server: %s \n", message);

	free(message);

	// 연결 종료
	close(sock_fd);
	return 0;
}
