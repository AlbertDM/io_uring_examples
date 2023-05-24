project('io_uring-network_tcp_example', 'c')

uring_dep = dependency('liburing', version : '>= 0.1.0')

# Server executable
executable('tcp_server',
            'tcp_server_uring.c',
                       dependencies: uring_dep
)

# Client executable
executable('tcp_client',
           'tcp_client_uring.c',
           #           dependencies: ['uring'])
            dependencies: uring_dep
)
