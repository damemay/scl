#include <math.h>
#include "../math/prime.h"

int scl_is_prime(const int x) {
    if(x<2) return -1;
    if(x<4) return 1;
    if(x%2==0) return 0;
    for(int i=3; i<=floor(sqrt((double)x)); i+=2)
	if(x%i==0) return 0;
    return 1;
}

int scl_next_prime(int x) {
    while(scl_is_prime(x) != 1)
	x++;
    return x;
}
