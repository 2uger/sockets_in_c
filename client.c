#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <strings.h>
#include <unistd.h>

static const char     SERVER_IP[] = "127.0.0.1";
static const uint16_t SERVER_PORT = 8080;

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

    close(client_socket);

    printf("Close client socket.\n");

    return EXIT_SUCCESS;
}

