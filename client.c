#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE (128)

static const char     SERVER_IP[] = "127.0.0.1";
static const uint16_t SERVER_PORT = 8080;

void echo_chat(int client_socket_fd)
{
    char buf[MAX_BUFFER_SIZE];
    ssize_t n;

    while (1) {
        printf("Input message: ");
        fgets(buf, MAX_BUFFER_SIZE, stdin);

        buf[strlen(buf) - 1] = 0;

        n = write(client_socket_fd, buf, strlen(buf));
        n = read(client_socket_fd, buf, n);
        if (n < 1) {
            perror("Something goes wrong while read from peer socket");
            return;
        }

        printf("Message from server: %s\n", buf);
    }
}

int main()
{
    /* File descriptor for socket. */
    int client_socket;
    struct sockaddr_in server_addr;

    /* Socket creation. */
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating client socket");
        return EXIT_FAILURE;
    }
    printf("Successfully create client socket.\n");
    bzero(&server_addr, sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = SERVER_PORT;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error while connecting to server");
        return EXIT_FAILURE;
    }

    printf("Start echo chat.\n");
    echo_chat(client_socket);

    close(client_socket);

    printf("Close client socket.\n");

    return EXIT_SUCCESS;
}

