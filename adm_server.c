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

	// ���� ���� ���� (TDP)
	sock_fd_server = socket(PF_INET, SOCK_STREAM, 0);

	memset(&server_addr, 0, sizeof(struct sockaddr_in));

	// TCP/IP ������ �ּ� �������� ����
	server_addr.sin_family = AF_INET;
	// � Ŭ���̾�Ʈ �ּҵ� ������ �� ����
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	// ������ ��Ʈ�� 2000������ ����
	server_addr.sin_port = htons(2000);

	// ���� ���Ͽ� �ּ� ���
	bind(sock_fd_server, (struct sockaddr*) &server_addr,
		sizeof(struct sockaddr_in));
	// ���� ��� ���·� ����
	listen(sock_fd_server, 5);

	client_addr_size = sizeof(struct sockaddr_in);

	// Ŭ���̾�Ʈ ��û�� �����ϰ�, Ŭ���̾�Ʈ ���� �Ҵ�
	sock_fd_client = accept(sock_fd_server,
		(struct sockaddr *) &client_addr,
		(socklen_t *) &client_addr_size);

	// Ŭ���̾�Ʈ�� ������ ����
	write(sock_fd_client, message, sizeof(message));
	// Ŭ���̾�Ʈ ���� ����
	close(sock_fd_client);
	// ���� ���� ����
	close(sock_fd_server);
	return 0;
}
