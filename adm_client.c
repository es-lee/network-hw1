#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int main (int argc, char **argv) {
	int sock_fd = 0;
	// ������ ���� �ּ� ������ ���� ����ü
	struct sockaddr_in server_addr;
	// �������� �޾ƿ� ������ ���� ����
	char *message = malloc(1024);
	int message_len = 0;

	// ���� ������ ���� ����(TCP) ����
	sock_fd = socket(PF_INET, SOCK_STREAM, 0);
	// �ʱ�ȭ
	memset(&server_addr, 0, sizeof(server_addr));
	// TCP/IP ������ ���� �ּ� ����(Address Family)���� ����
	server_addr.sin_family = AF_INET;
	// ������ ������ �ּҴ� ���� ȣ��Ʈ(127.0.0.1)�� ����
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	// ������ ������ ��Ʈ ��ȣ�� 2000��
	server_addr.sin_port = htons(2000);

	connect(sock_fd, (struct sockaddr*) &server_addr,
		sizeof(server_addr));

	message_len = read(sock_fd, message, 1024 - 1);

	printf("Message from server: %s \n", message);

	free(message);

	// ���� ����
	close(sock_fd);
	return 0;
}
