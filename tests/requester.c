#include "../net/http.h"
#include <string.h>

int main(int argc, char** argv) {
    if(argc < 3) return 1;
    scl_http_request_method method;
    if(strstr(argv[1], "get")) method = scl_http_request_get;
    else if(strstr(argv[1], "post")) method = scl_http_request_post;
    else if(strstr(argv[1], "head")) method = scl_http_request_head;
    scl_http_request request = {
	.url = argv[2],
	.method = method,
	.parse_headers = 1,
    };
    scl_http_response response;
    scl_http_request_perform(&request, &response);

    return 0;
}
