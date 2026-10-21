
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>

int main(void)
{
    // Resolve the server address.
    struct addrinfo hints = {0};
    struct addrinfo *result = NULL;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int error = getaddrinfo("example.com", "80", &hints, &result);

    if (error != 0) {
        fprintf(stderr, "Address lookup failed: %s\n",
                gai_strerror(error));
        return 1;
    }

    // Create a TCP socket and connect to the server.
    int sockfd = socket(result->ai_family,
                        result->ai_socktype,
                        result->ai_protocol);

    if (sockfd == -1) {
        perror("socket");
        freeaddrinfo(result);
        return 1;
    }

    if (connect(sockfd, result->ai_addr, result->ai_addrlen) == -1) {
        perror("connect");
        close(sockfd);
        freeaddrinfo(result);
        return 1;
    }

    freeaddrinfo(result);

    printf("Connected to server. Monitoring socket...\n");

    // Enable non-blocking mode using fcntl().
    int flags = fcntl(sockfd, F_GETFL, 0);

    if (flags == -1 ||
        fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl");
        close(sockfd);
        return 1;
    }

    // Configure pollfd to monitor incoming data.
    struct pollfd pfd;
    pfd.fd = sockfd;
    pfd.events = POLLIN;
    pfd.revents = 0;

    // Monitor the socket until an error or closure occurs.
    while (1) {
        int ready = poll(&pfd, 1, 1000);

        if (ready == -1) {
            if (errno == EINTR) {
                continue;
            }

            perror("poll");
            break;
        }

        if (ready == 0) {
            printf("Status: connected, no events detected\n");
            continue;
        }

        if (pfd.revents & POLLIN) {
            char buffer[1024];

            ssize_t bytes = recv(sockfd, buffer,
                                 sizeof(buffer), 0);

            if (bytes > 0) {
                printf("Status: received %zd bytes\n", bytes);
            } else if (bytes == 0) {
                printf("Status: server closed the connection\n");
                break;
            } else if (errno == EAGAIN ||
                       errno == EWOULDBLOCK) {
                printf("Status: no data currently available\n");
            } else if (errno == EINTR) {
                continue;
            } else {
                perror("recv");
                break;
            }
        }

        if (pfd.revents & (POLLERR | POLLHUP | POLLNVAL)) {
            printf("Status: socket error, hangup, or invalid FD\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}