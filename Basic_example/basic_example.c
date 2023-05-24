#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <liburing.h>

#define BUFFER_SIZE 4096

int main() {
    struct io_uring ring;
    struct io_uring_sqe *sqe;
    struct io_uring_cqe *cqe;
    int fd, ret;
    char buffer[BUFFER_SIZE];

    // Initialize io_uring instance
    io_uring_queue_init(32, &ring, 0);

    // Open a file for reading
    fd = open("example.txt", O_RDONLY);
    if (fd < 0) {
        perror("Failed to open file");
        return 1;
    }

    // Prepare read operation
    sqe = io_uring_get_sqe(&ring);
    io_uring_prep_read(sqe, fd, buffer, BUFFER_SIZE, 0);

    // Submit the operation
    io_uring_submit(&ring);

    // Wait for the operation to complete
    ret = io_uring_wait_cqe(&ring, &cqe);

    // Check if the operation completed successfully
    if (ret < 0) {
        perror("Error waiting for completion");
        return 1;
    }

    // Process the completed operation
    if (cqe->res >= 0) {
        printf("Read %d bytes: %s\n", cqe->res, buffer);
    } else {
        fprintf(stderr, "Read operation failed\n");
    }

    // Clean up and close the file
    io_uring_cqe_seen(&ring, cqe);
    close(fd);
    io_uring_queue_exit(&ring);

    return 0;
}
