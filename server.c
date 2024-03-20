#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <strings.h>
#include <unistd.h>

#define MAX_LISTEN_CONN (5)

static const char     BIND_IP[] = "0.0.0.0";
static const uint16_t BIND_PORT = 8080;

int main()
{
    /* File descriptor for socket. */
    int server_socket_fd, peer_socket_fd;
    struct sockaddr_in server_addr, peer_addr;
    socklen_t peer_addr_size;
    char client_ip_addr[INET_ADDRSTRLEN];

    /* Socket creation. */
    server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_fd == -1) {
        perror("Error creating server socket");
        return EXIT_FAILURE;
    }
    printf("Successfully create server socket.\n");
    bzero(&server_addr, sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = BIND_PORT;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_socket_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in)) == -1) {
        perror("Error binding socket");
        return EXIT_FAILURE;
    }
    printf("Server socket successfully binded..\n");

    if (listen(server_socket_fd, MAX_LISTEN_CONN) == -1) {
        perror("Error start listening");
        return EXIT_FAILURE;
    }

    while (1) {
        bzero(client_ip_addr, INET_ADDRSTRLEN);

        peer_addr_size = sizeof(peer_addr_size);
        printf("Server start accepting.\n");
        peer_socket_fd = accept(server_socket_fd, (struct sockaddr*)&peer_addr, &peer_addr_size);

        if (peer_socket_fd == -1) {
            perror("Error accepting new client");
            close(server_socket_fd);
            return EXIT_FAILURE;
        }

        inet_ntop(AF_INET, &peer_addr.sin_addr, client_ip_addr, INET_ADDRSTRLEN);
        printf("Server accept client: %s addr, %d port.\n", client_ip_addr, peer_addr.sin_port);

        close(peer_socket_fd);
        printf("Close peer connection.\n");
    }

    close(server_socket_fd);

    return EXIT_SUCCESS;
}

