#include "../net/http.h"

int main(int argc, char** argv) {
    if(argc < 2) return 1;
    scl_http_request request = {
	.url = argv[1],
	.method = scl_http_request_head,
    };
    scl_http_response* response = scl_http_request_perform(&request);
    free(response);

    return 0;
}
