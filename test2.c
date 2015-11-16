#include <stdio.h>
#include <stdlib.h>
#include "alloc.h"


int main() {

	void* memarea = malloc(200 * sizeof(int));
	
	alloc_init(memarea, 200 * sizeof(int));

    int* ptr = (int*) alloc_get(memarea, sizeof(int));
	ptr = alloc_resize(memarea, ptr, 2 * sizeof(int));
	if(ptr == 0)
		printf("alloc_resize failed!\n");
    alloc_release(memarea, ptr);

    ptr = (int*) alloc_get(memarea, sizeof(int) * 100);
	if(ptr == 0)
		printf("alloc_get failed!\n");
    alloc_release(memarea, ptr);

	free(memarea);

    return 0;
}
