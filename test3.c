#include <stdio.h>
#include <stdlib.h>
#include "alloc.h"

#define NUM_INTS 5000

int main() {
    int i;
    int* ptrs[NUM_INTS];
	void* memarea = malloc(sizeof(int) * NUM_INTS * (NUM_INTS+1));

	alloc_init(memarea, sizeof(int) * NUM_INTS * (NUM_INTS+1));

    for (i = 0; i < NUM_INTS; i++) {
        ptrs[i] = (int*) alloc_get(memarea, sizeof(int) * (i + 1));
        ptrs[i][i] = i;
    }

    for (i = 0; i < NUM_INTS; i++) {
        alloc_release(memarea, ptrs[i]);
    }

	free(memarea);
    return 0;
}
