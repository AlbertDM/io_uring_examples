# io_uring_examples
Some examples for io_uring library


## What is io_uring?

`io_uring` is an asynchronous I/O interface for the Linux kernel. An io_uring is a pair of ring buffers in shared memory that are used as queues between user space and the kernel:

* Submission queue (SQ): A user space process uses the submission queue to send asynchronous I/O requests to the kernel.
* Completion queue (CQ): The kernel uses the completion queue to send the results of asynchronous I/O operations back to user space.

## References

https://developers.redhat.com/articles/2023/04/12/why-you-should-use-iouring-network-io
https://kernel.dk/io_uring.pdf
