/* James Warne | jfw222 *
 *    Mini Project 2	*
 *	  CS 4410	*/

#include <stdlib.h>
#include <stdio.h>
#include "alloc.h"
#include <assert.h>
#include <string.h>

/* You must implement these functions according to the specification given in
 * alloc.h. You can add any data structures you like to this file.
 */

/* Struct for memory headers */
struct mem_header {
	int size;			//size of memory block
	struct mem_header * next;	//pointer to next header
	struct mem_header * last;	//pointer to last header
	int available;			//availability indicator
}; typedef struct mem_header meta;	//32 BYTES

/* find the next available memory space */
meta * findFree(void * memarea, meta * prev, int size) {
	/* variable declarations */
	meta * vacant = memarea;		//point to beginning of memory
	
	/* while block doesn't satisfy reqs, move to next */
	assert(vacant != NULL);
	while (vacant != NULL && !(vacant->available == 1 && vacant->size >= size)) {
		prev = vacant;
		vacant = vacant->next;
	}

	/* return meta header */
	return vacant;
}

/* trim unused space from memory block */
void removeExtra(meta * current, int size) {
	/* variable declarations */
	meta * extra;
	
	/* set meta location to start immediately after last block */
	extra = (meta *)((void *)current + sizeof(meta) + size);
	extra->size = (int)(current->size - size - sizeof(meta));
	extra->next = current->next;
	extra->last = current;
	if (current->next != NULL) current->next->last = extra;

	/* adjust header being trimmed */
	current->next = extra;
	current->size = size;
	extra->available = 1;
}

/* with memory area, initialize firp st header */
int alloc_init(void * memarea, int size) {
	/* return failed if allocated space can't hold meta */
	if (size < sizeof(meta)) return -1;

	/* declare variable & set address to memarea */
	meta * start = (void *)memarea;
	start->size = (size - sizeof(meta));
	start->available = 1;
	return 0;
}

void * alloc_get(void * memarea, int size) {
	/* align the address blocks to 8 bytes */
	if (size % 8 != 0) {
		int remainder = size % 8;
		size = size + 8 - remainder;
	}
	/* initialize variable */
	meta * current;
	meta * prev;

	/* find the next free space */
	prev = (meta *)memarea;
	current = findFree(memarea, prev, size);

	/* test to ensure that there are free spaces */
	if (current == NULL) {
		printf("No Accessible Memory");
		return NULL;
	}

	/* trim the additional space if there are more than 8 spaces left */
	if (current->size - size - sizeof(meta) >= 8) {
		removeExtra(current, size);
	}

	/* set meta tag to unavailable */
	current->available = 0;
	return ((void *)current + sizeof(meta));
}

void alloc_release(void * memarea, void * mem) {
	/* Make sure it isn't already null */
	if (mem == NULL) return;

	/* initialize variable */
	meta * released;
	
	/* set the header state to available */
	released = (meta *)((void*)mem - sizeof(meta));
	released->available = 1;

	/* check surrounding blocks to see if they are free and merge */
	/* check previous block */
	if (released->last != NULL && released->last->available
				&& released->available) {
		if (released->next != NULL)
			released->next->last = released->last;
		released->last->size = released->size + sizeof(meta)
						+ released->last->size;
		released->last->next = released->next;
		
	}
	/* check next block */
	if (released->next != NULL && released->next->available
				&& released->available) {
		released->size = released->next->size + sizeof(meta)
						+ released->size;
		released->next = released->next->next;
		if (released->next != NULL)
			released->next->last = released;
	}
}

void * alloc_resize(void * memarea, void * mem, int size) {
	meta * old = (meta *)((void *)mem - sizeof(meta));
	old->available = 1;
	void * newSpace;

	/* check surrounding blocks to see if they are free and merge */
	/* check previous block */
	if (old->last != NULL && old->last->available
				&& old->available) {
		if (size <= old->size + old->last->size) {
			if (old->next != NULL)
				old->next->last = old->last;
			old->last->size = old->size + sizeof(meta)
						+ old->last->size;
			old->last->next = old->next;
			memmove((void *)(old->last), (void *)(old), old->last->size < size
							? old->last->size : size);
			newSpace = alloc_get(memarea, size);
			alloc_release(memarea, mem);
			
			return newSpace;
		} else {
			old->available = 0;
		}
	/* check next block */
	} else if (old->next != NULL && old->next->available
				&& old->available) {
		if (size <= old->size + old->next->size) {
			if (old->next != NULL)
				old->next->last = old;
			old->available = 1;
			old->next->available = 1;
			old->size = old->next->size + sizeof(meta)
						+ old->size;
			old->next = old->next->next;
			alloc_release(memarea, (void *)(old->next));

			newSpace = alloc_get(memarea, size);
			old->available = 0;
			return newSpace;
		} else {
			old->available = 0;
		}
	}

	if (mem == NULL) return newSpace = alloc_get(memarea, size);
	return NULL;
}
