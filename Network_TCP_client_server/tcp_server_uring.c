#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <liburing.h>

#define BUF_SIZE 1024
#define BACKLOG 10

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;

    // Create a socket for the server
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);  // Change to the desired port
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the server address
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_fd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    // Initialize io_uring
    struct io_uring ring;
    if (io_uring_queue_init(BUF_SIZE, &ring, 0) < 0) {
        perror("io_uring_queue_init");
        exit(1);
    }

    // Prepare buffers for I/O operations
    char recv_buffer[BUF_SIZE];
    // char send_buffer[BUF_SIZE];
    struct io_uring_sqe *sqe;
    struct io_uring_cqe *cqe;

    while (1) {
        // Accept incoming connections
        client_addr_len = sizeof(client_addr);
        if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) {
            perror("accept");
            exit(1);
        }

        // Get a new sqe from io_uring
        sqe = io_uring_get_sqe(&ring);

        // Prepare an asynchronous read operation
        io_uring_prep_read_fixed(sqe, client_fd, recv_buffer, BUF_SIZE, 0, 0);
        io_uring_sqe_set_flags(sqe, IOSQE_ASYNC);

        // Submit the read operation to io_uring
        io_uring_submit(&ring);

        // Wait for the completion of the read operation
        io_uring_wait_cqe(&ring, &cqe);

        // Check the result and handle the received data
        if (cqe->res >= 0) {
            // Data read successfully
            printf("Received data from client: %.*s\n", cqe->res, recv_buffer);

            // Prepare an asynchronous write operation
            io_uring_prep_write_fixed(sqe, client_fd, recv_buffer, cqe->res, 0, 0);
            io_uring_sqe_set_flags(sqe, IOSQE_ASYNC);

            // Submit the write operation to io_uring
            io_uring_submit(&ring);

            // Wait for the completion of the write operation
            io_uring_wait_cqe(&ring, &cqe);

            if (cqe->res >= 0) {
                // Data written successfully
                printf("Echoed data back to client\n");
            } else {
                // Error occurred during write operation
                fprintf(stderr, "Write error: %d\n", cqe->res);
            }
        } else {
            // Error occurred during read operation
            fprintf(stderr, "Read error: %d\n", cqe->res);
        }

        // Clean up the completion queue entry
        io_uring_cqe_seen(&ring, cqe);

        // Close the client socket
        close(client_fd);
    }

    // Clean up io_uring
    io_uring_queue_exit(&ring);

    // Close the server socket
    close(server_fd);

    return 0;
}

