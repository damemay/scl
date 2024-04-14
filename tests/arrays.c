#include "../data/array.h"
#include "../utils/helpers.h"
#include <assert.h>

void alog(scl_array* arr) {
    SCL_VLOG("cap:%d size:%d step:%d",
	    arr->capacity, arr->size, arr->step);
    for(size_t i=0; i<arr->size; i++) {
	int el;
	int res = SCL_ARRAY_GET(arr, int, i, el);
	assert(res==0);
	printf("%d ", el);
    }
    printf("\n");
}

int main(int argc, char** argv) {
    scl_array* list = malloc(sizeof(scl_array));
    int res = SCL_ARRAY_INIT(list, int, 5);
    assert(res == 0);
    alog(list);
    res = SCL_ARRAY_ADD(list, int, 5);
    assert(res == 0);
    res = SCL_ARRAY_ADD(list, int, 10);
    assert(res == 0);
    res = SCL_ARRAY_ADD(list, int, 15);
    assert(res == 0);
    res = SCL_ARRAY_ADD(list, int, 20);
    assert(res == 0);
    alog(list);
    res = SCL_ARRAY_ADD(list, int, 5);
    assert(res == 0);
    res = SCL_ARRAY_ADD(list, int, 5);
    assert(res == 0);
    alog(list);
    res = SCL_ARRAY_DEL(list, int, 2);
    assert(res == 0);
    res = SCL_ARRAY_DEL(list, int, 3);
    if(res == -1) SCL_LOG("element deleted, but not reallocated");
    alog(list);

    int v;
    res = SCL_ARRAY_GET(list, int, 100, v);
    if(res == -1) SCL_LOG("wrong index!");
    else SCL_VLOG("%d", v);

    scl_array_free(list);
    free(list);
}
