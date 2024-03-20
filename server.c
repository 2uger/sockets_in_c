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
#define MAX_LISTEN_CONN (5)

static const char     BIND_IP[] = "0.0.0.0";
static const uint16_t BIND_PORT = 8080;

void echo_chat(int peer_socket_fd)
{
    char buf[MAX_BUFFER_SIZE];
    ssize_t n;

    while (1) {
        n = read(peer_socket_fd, buf, MAX_BUFFER_SIZE);
        printf("Read %ld bytes from peer socket.\n", n);
        if (n < 1) {
            perror("Something goes wrong while read from peer socket");
            return;
        }

        if (strncmp("close", buf, 5) == 0) {
            printf("Recv close command from client.\n");
            return;
        }

        write(peer_socket_fd, buf, n);
    }
}

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

    /* Avoid socket already in use error. */
    if (setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) == -1) {
        perror("Error setup socket option on server socket");
        return EXIT_FAILURE;
    }

    bzero(&server_addr, sizeof(struct sockaddr_in));

    /* Setup server address and port. */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = BIND_PORT;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* Bind server socket. */
    if (bind(server_socket_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in)) == -1) {
        perror("Error binding socket");
        return EXIT_FAILURE;
    }
    printf("Server socket successfully binded..\n");

    /* Start listening for new connections. */
    if (listen(server_socket_fd, MAX_LISTEN_CONN) == -1) {
        perror("Error start listening");
        return EXIT_FAILURE;
    }

    /* Start accept and handle connections. */
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

        echo_chat(peer_socket_fd);

        close(peer_socket_fd);
        printf("Close peer connection.\n");
    }

    close(server_socket_fd);

    return EXIT_SUCCESS;
}

