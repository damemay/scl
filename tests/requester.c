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
    };
    scl_http_response response;
    int ret = scl_http_request_perform(&request, &response);
    if(ret < 0) {
	char buf[1024];
	scl_http_error_parse(ret, buf, 1024);
	puts(buf);
	return 1;
    }
    
    // printf("HTTP %d\n", response.status_code);
    
    // for(size_t i=0; i<response.headers->size; i++) {
    //     scl_mitem* header = scl_map_get_by_index(response.headers, i);
    //     printf("%s:%s\n", header->key, (char*)header->value);
    // }
    // 
    if(response.data) {
        printf("%.*s\n", (int)response.data_size, (char*)response.data);
    }

    return 0;
}
