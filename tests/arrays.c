#include "../data/array.h"
#include "../utils/helpers.h"
#include <assert.h>

void alog(scl_array* arr) {
    SCL_VLOG("cap:%d size:%d step:%d",
	    arr->capacity, arr->size, arr->step);
    for(size_t i=0; i<arr->size; i++)
	printf("%d ", SCL_ARRAY_GET(arr, int, i));
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
    assert(res == 0);
    alog(list);
    scl_array_free(list);
    free(list);
}
