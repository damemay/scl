#include "../data/array.h"
#include "../utils/helpers.h"
#include <assert.h>

void alog(scl_array* arr) {
    SCL_VLOG("cap:%d size:%d step:%d",
	    arr->capacity, arr->size, arr->step);
    for(size_t i=0; i<arr->size; i++) {
	int el = SCL_ARRAY_GET(arr, i, int);
	printf("%d ", el);
    }
    printf("\n");
}

int main(int argc, char** argv) {
    scl_array* list = malloc(sizeof(scl_array));
    int res = scl_array_init(list, 5, sizeof(int));
    assert(res == 0);
    alog(list);
    res = SCL_ARRAY_ADD(list, 5, int);
    assert(res == 0);
    res = SCL_ARRAY_ADD(list, 10, int);
    assert(res == 0);
    res = SCL_ARRAY_ADD(list, 15, int);
    assert(res == 0);
    res = SCL_ARRAY_ADD(list, 20, int);
    assert(res == 0);
    alog(list);
    res = SCL_ARRAY_ADD(list, 2, int);
    assert(res == 0);
    res = SCL_ARRAY_ADD(list, 3, int);
    assert(res == 0);
    alog(list);
    res = SCL_ARRAY_DEL(list, 2, int);
    assert(res == 0);
    res = SCL_ARRAY_DEL(list, 3, int);
    if(res == -1) SCL_LOG("element deleted, but not reallocated");
    alog(list);

    int v = SCL_ARRAY_GET(list, 100, int);
    if(!v) SCL_LOG("wrong index!");
    else SCL_VLOG("%d", v);

    scl_array_free(list);
    free(list);
}
