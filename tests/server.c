#include "../net/socket.h"
#include <assert.h>

void callback(int fd) {
    int sent = scl_send(fd, "Hello!", 7);
    assert(sent == 7);
    close(fd);
}

int main(int argc, char** argv) {
    scl_socket_server server = { .port = "5000" };
    int res = scl_socket_server_init(&server);
    assert(res == 0);
    while(1) {
	res = scl_socket_server_accept(&server, callback);
	assert(res == 0);
    }
    scl_socket_server_free(&server);
    return 0;
}
