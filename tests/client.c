#include "../net/socket.h"
#include "../utils/helpers.h"
#include <assert.h>

int main(int argc, char** argv) {
    scl_socket_client client = {
	.host = "127.0.0.1",
	.port = "5000"
    };
    int res = scl_socket_client_init(&client);
    assert(res == 0);
    char buffer[10];
    scl_recv(client.fd, buffer, 7);
    SCL_VLOG("%s", buffer);
    return 0;
}
