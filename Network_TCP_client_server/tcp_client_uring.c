#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <liburing.h>

#define SERVER_IP "127.0.0.1"  // Change to the server IP
#define SERVER_PORT 8080      // Change to the server port
#define BUF_SIZE 1024
// #define QUEUE_SIZE 4096  // Define the size of the submission queue

int main() {
    int client_fd;
    struct sockaddr_in server_addr;

    // Create a socket for the client
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(server_addr.sin_addr)) <= 0) {
        perror("inet_pton");
        exit(1);
    }

    // Connect to the server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        exit(1);
    }

#if 1
    // Initialize io_uring
    struct io_uring ring;
    if (io_uring_queue_init(BUF_SIZE, &ring, 0) < 0) {
        perror("io_uring_queue_init");
        exit(1);
    }
#else
    #define QUEUE_SIZE 4096
    // Initialization parameters for the queue.
    struct io_uring_params params = {0};
    params.flags |= IORING_SETUP_CQSIZE;
    params.cq_entries = QUEUE_SIZE;

    if (io_uring_queue_init_params(QUEUE_SIZE, &ring, &params) < 0) {
        perror("io_uring_queue_init_params");
        exit(1);
    } = 4096;  // Increase the number of completion queue entries
#endif

    // Prepare buffers for I/O operations
    char send_buffer[BUF_SIZE];
    char recv_buffer[BUF_SIZE];
    struct io_uring_sqe *sqe;
    struct io_uring_cqe *cqe;

    // Prompt user for input
    printf("Enter a message to send to the server (max %d characters): ", BUF_SIZE - 1);
    fgets(send_buffer, BUF_SIZE, stdin);

    // Remove newline character from the input
    send_buffer[strcspn(send_buffer, "\n")] = '\0';

    // Get a new sqe from io_uring
    sqe = io_uring_get_sqe(&ring);

    // Prepare an asynchronous write operation
    io_uring_prep_write_fixed(sqe, client_fd, send_buffer, strlen(send_buffer), 0, 0);
    io_uring_sqe_set_flags(sqe, IOSQE_ASYNC);

    // Submit the write operation to io_uring
    io_uring_submit(&ring);

    // Wait for the completion of the write operation
    io_uring_wait_cqe(&ring, &cqe);

    // Check the result and handle the completion
    if (cqe->res >= 0) {
        // Data written successfully
        printf("Sent data to server: %s\n", send_buffer);

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
            printf("Received echoed data from server: %.*s\n", cqe->res, recv_buffer);
        } else {
            // Error occurred during read operation
            fprintf(stderr, "Read error: %d\n", cqe->res);
        }

        // Clean up the completion queue entry
        io_uring_cqe_seen(&ring, cqe);
    } else {
        // Error occurred during write operation
        fprintf(stderr, "Write error: %d\n", cqe->res);
    }

    // Clean up io_uring
    io_uring_queue_exit(&ring);

    // Close the client socket
    close(client_fd);

    return 0;
}

