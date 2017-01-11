/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */

// ID : cs20150112
// Name : Kim Dongseong

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

void * heap_listp;
void * exp_listp;
/*void * exp_listp1 = NULL;
void * exp_listp2 = NULL;
void * exp_listp3 = NULL;
void * exp_listp4 = NULL;
void * exp_listp5 = NULL;
void * exp_listp6 = NULL;
void * exp_listp7 = NULL;
void * exp_listp8 = NULL;
void * exp_listp9 = NULL;
void * exp_listp10 = NULL;
void * exp_listp11 = NULL;
void * exp_listp12 = NULL;
void * exp_listp13 = NULL;
void * exp_listp14 = NULL;
void * exp_listp15 = NULL;
void * exp_listp16 = NULL;
void * exp_listp17 = NULL;
void * exp_listp18 = NULL;
void * exp_listp19 = NULL;
void * exp_listp20 = NULL;
*/
/* 
 * macro list from textbook
 */
/* Basic constants and macros */
#define WSIZE 4				/* Word and header/footer size (bytes) */
#define DSIZE 8				/* Double word size (bytes) */
#define CHUNKSIZE (1<<13)	/* Extend heap by this amount (bytes) */

#define MAX(x, y) ((x) > (y)? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))
/*
 * end of macro lists
 */

/*	
	void *mem_sbrk(int incr): Expands the heap by incr bytes, where incr is a positive non-zero integer and returns a generic pointer to the first byte of
	the newly allocated heap area. The semantics are identical to the Unix sbrk function, except that mem sbrk accepts only a positive non-zero integer
	argument.
 */

static void *coalesce(void *bp);
static void *find_fit(size_t asize);
static void place(void *bp, size_t asize);
static void *extend_heap(size_t words);
//static void *select_seglist(size_t size, int mode, void *bp);

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{	
	exp_listp = NULL;
	if ((heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1)
		return -1;
	PUT(heap_listp, 0);
	PUT(heap_listp + (1*WSIZE), PACK(DSIZE, 1));
	PUT(heap_listp + (2*WSIZE), PACK(DSIZE, 1));
	PUT(heap_listp + (3*WSIZE), PACK(0, 1));
	heap_listp += (2*WSIZE);

	if (extend_heap((CHUNKSIZE>>7)/WSIZE) == NULL)
		return -1;

	return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
	size_t asize;				// n * DSIZE
	size_t extendsize;
	char *bp;
	if (size == 0)
		return NULL;

	if (size <= DSIZE)
		asize = 2*DSIZE;
	else
		asize = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE);

	if ((bp = find_fit(asize)) != NULL){
		//printf("mm_malloc...bp %p\n", bp);
		place(bp, asize);
		return bp;
	}
	
	extendsize = MAX(asize, CHUNKSIZE);
	if ((bp = extend_heap(extendsize/WSIZE)) == NULL)
		return NULL;
	//printf("mm_malloc...bp %p\n",bp);
	place(bp, asize);
	return bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr){
	size_t size = GET_SIZE(HDRP(ptr));
	PUT(HDRP(ptr), PACK(size, 0));
	PUT(FTRP(ptr), PACK(size, 0));
	PUT(ptr, (unsigned int)NULL);
	PUT(ptr + WSIZE, (unsigned int)NULL);
	//printf("FREE! : bp %p, next %p, prev %p\n", ptr, (void *)GET(ptr), (void *)GET(ptr + WSIZE));
	coalesce(ptr);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
	void *oldptr = ptr;
	void *newptr;
	size_t asize;
	size_t copySize = GET_SIZE(HDRP(oldptr));
	size_t temp;
	if (ptr == NULL)
		return mm_malloc(size);
	else if (size == 0){
		mm_free(ptr);
		return NULL;
	}
	
	if (size <= DSIZE)
		asize = 2*DSIZE;
	else
		asize = DSIZE * ((size + DSIZE + DSIZE - 1) / DSIZE);
//	printf("realloc...oldptr %p, size 0x%x\n", ptr, size);i
	if(copySize >= asize){
//		prev = PREV_BLKP(oldptr);
//		temp = GET_SIZE(HDRP(prev));
			if ((copySize - asize) > 2*DSIZE){
//				printf("realloc : case 1\n");
				PUT(HDRP(oldptr), PACK(asize, 1));
				PUT(FTRP(oldptr), PACK(asize, 1));
				PUT(HDRP(NEXT_BLKP(oldptr)), PACK(copySize - asize, 0));
				PUT(FTRP(NEXT_BLKP(oldptr)), PACK(copySize - asize, 0));
				newptr = oldptr;
				mm_free(NEXT_BLKP(oldptr));
//				printf("oldptr %p, allocated block %p size 0x%x, following free block %p size 0x%x\n", oldptr, newptr, GET_SIZE(HDRP(newptr)), NEXT_BLKP(newptr), GET_SIZE(HDRP(NEXT_BLKP(newptr))));
				return newptr;
			}
			else{
//				printf("realloc : case 5\n");
				//PUT(HDRP(oldptr), PACK(asize, 1));
				//PUT(FTRP(oldptr), PACK(asize, 1));
//				printf("oldptr %p, size 0x%x, newptr %p, size 0x%x\n", oldptr, copySize, oldptr, asize);
				return oldptr;
			}
	}
	else{
		if (GET_ALLOC(HDRP(NEXT_BLKP(oldptr)))){
//			printf("realloc : case 2...GET(HDRP(NEXT_BLKP(oldptr))) 0x%x\n", GET(HDRP(NEXT_BLKP(oldptr))));
			newptr = mm_malloc(size);
			if (newptr == NULL)
				return NULL;
			if (size < copySize)
				copySize = size;
//			printf("oldptr %p, allocated block %p size 0x%x\n", oldptr, newptr, GET_SIZE(HDRP(newptr)));
			memcpy(newptr, oldptr, copySize);
			mm_free(oldptr);
			return newptr;
		}
		else{
			if (copySize + GET_SIZE(HDRP(NEXT_BLKP(oldptr))) > asize){
				temp = GET_SIZE(HDRP(NEXT_BLKP(oldptr)));
				if (copySize + GET_SIZE(HDRP(NEXT_BLKP(oldptr))) - asize >= 2*DSIZE){
//					printf("realloc : case 3\n");
//					printf("case 3 check : GET(NEXT_BLKP(oldptr)) 0x%x\n", GET(NEXT_BLKP(oldptr)));
					if ((void *)GET(NEXT_BLKP(oldptr)) != NULL){
					//	printf("NEXT_BLKP(oldptr) %p, GET(NEXT_BLKP(oldptr)) 0x%x\n", NEXT_BLKP(oldptr), GET(NEXT_BLKP(oldptr)));
						PUT(GET(NEXT_BLKP(oldptr)) + WSIZE, GET(NEXT_BLKP(oldptr) + WSIZE));
					}
					if ((void *)GET(NEXT_BLKP(oldptr) + WSIZE) != NULL){
						PUT(GET(NEXT_BLKP(oldptr) + WSIZE), GET(NEXT_BLKP(oldptr)));
					}
					else
						exp_listp = (void *)GET(NEXT_BLKP(oldptr));
					PUT(HDRP(oldptr), PACK(asize, 1));
					PUT(FTRP(oldptr), PACK(asize, 1));
					PUT(HDRP(NEXT_BLKP(oldptr)), PACK(copySize + temp - asize, 0));
					PUT(FTRP(NEXT_BLKP(oldptr)), PACK(copySize + temp - asize, 0));
					newptr = oldptr;

//						printf("oldptr %p, allocated block %p size 0x%x, following free block %p size 0x%x\n", oldptr, newptr, GET_SIZE(HDRP(newptr)), NEXT_BLKP(newptr), GET_SIZE(HDRP(NEXT_BLKP(newptr))));
					mm_free(NEXT_BLKP(oldptr));
					return newptr;
				}
				else{
//					printf("realloc : case 6\n");
					newptr = mm_malloc(size);
					if (newptr == NULL)
						return NULL;
					if (size < copySize)
						copySize = size;
					memcpy(newptr, oldptr, copySize);
					mm_free(oldptr);
					return newptr;
				}
			}
			else{
//				printf("realloc : case 4\n");
				newptr = mm_malloc(size);
				if(newptr == NULL)
					return NULL;
				if (size < copySize)
					copySize = size;
//				printf("oldptr %p, allocated block %p size 0x%x\n", oldptr, newptr, GET_SIZE(HDRP(newptr)));
				memcpy(newptr, oldptr, copySize);
				mm_free(oldptr);
				return newptr;
			}
			
		}
	}
}
/*
static void *select_seglist(size_t size, int mode, void *bp){
	int index = 1, i;

	for (i = 0; i < 19; i++){
		size = size >> 1;
		if (size == 0)
			break;
		index++;
	}
	if (index == 1){
		printf("exp_listp%d!!\n", index);
		if (mode == 0)
			return exp_listp1;
		else
			exp_listp1 = bp;
		printf("new exp_listp : exp_listp%d = %p", index, bp);
	}
	else if (index == 2){
		printf("exp_listp%d!!\n", index);
		if (mode == 0)
			return exp_listp2;
		else
			exp_listp2 = bp;
		printf("new exp_listp : exp_listp%d = %p\n", index, bp);
	}
	else if (index == 3){
		printf("exp_listp%d!!\n", index);
		if(mode == 0)
			return exp_listp3;
		else
			exp_listp3 = bp;
		printf("new exp_listp : exp_listp%d = %p\n", index, bp);
	}
	else if (index == 4){
		printf("exp_listp%d!!\n", index);
		if (mode == 0)
			return exp_listp4;
		else
			exp_listp4 = bp;
		printf("new exp_listp : exp_listp%d = %p\n", index, bp);
	}
	else if (index == 5){
		printf("exp_listp%d!!\n", index);
		if (mode == 0)
			return exp_listp5;
		else
			exp_listp5 = bp;
		printf("new exp_listp : exp_listp%d = %p\n", index, bp);
	}
	else if (index == 6){
		printf("exp_listp%d!!\n", index);
		if (mode == 0)
			return exp_listp6;
		else
			exp_listp6 = bp;
		printf("new exp_listp : exp_listp%d = %p\n", index, bp);
	}
	else if (index == 7){
		printf("exp_listp%d!!\n", index);
		if (mode == 0)
			return exp_listp7;
		else
			exp_listp7 = bp;
		printf("new exp_listp : exp_listp%d = %p\n", index, bp);
	}
	else if (index == 8){
		printf("exp_listp%d!!\n", index);
		if (mode == 0)
			return exp_listp8;
		else
			exp_listp8 = bp;
		printf("new exp_listp : exp_listp%d = %p\n", index, bp);
	}
	else if (index == 9){
		printf("exp_listp%d!!\n", index);
		if (mode == 0)
			return exp_listp9;
		else
			exp_listp9 = bp;
		printf("new exp_listp : exp_listp%d = %p\n", index, bp);
	}
	else if (index == 10){
		printf("exp_listp%d!!\n", index);
		if (mode == 0)
			return exp_listp10;
		else
			exp_listp10 = bp;
		printf("new exp_listp : exp_listp%d = %p\n", index, bp);
	}
	else if (index == 11){
		printf("exp_listp%d!!\n", index);
		if (mode == 0)
			return exp_listp11;
		else
			exp_listp11 = bp;
		printf("new exp_listp : exp_listp%d = %p\n", index, bp);
	}
	else if (index == 12){
		printf("exp_listp%d!!\n", index);
		if (mode == 0)
			return exp_listp12;
		else
			exp_listp12 = bp;
		printf("new exp_listp : exp_listp%d = %p", index, bp);
		if (bp == NULL)
			printf("\n");
		else
			printf(", next = %p\n", (void *)GET(bp));
	}
	else if (index ==13){
		printf("exp_listp%d!!\n", index);
		if (mode == 0)
			return exp_listp13;
		else
			exp_listp13 = bp;
		printf("new exp_listp : exp_listp%d = %p\n", index, bp);
	}
	else if (index == 14){
		printf("exp_listp%d!!\n", index);
		if (mode == 0)
			return exp_listp14;
		else
			exp_listp14 = bp;
		printf("new exp_listp : exp_listp%d = %p\n", index, bp);
	}
	else if (index == 15){
		printf("exp_listp%d!!\n", index);
		if (mode == 0)
			return exp_listp15;
		else
			exp_listp15 = bp;
		printf("new exp_listp : exp_listp%d = %p\n", index, bp);
	}
	else if (index == 16){
		printf("exp_listp%d!!\n", index);
		if (mode == 0)
			return exp_listp16;
		else
			exp_listp16 = bp;
		printf("new exp_listp : exp_listp%d = %p\n", index, bp);
	}
	else if (index == 17){
		printf("exp_listp%d!!\n", index);
		if (mode == 0)
			return exp_listp17;
		else
			exp_listp17 = bp;
		printf("new exp_listp : exp_listp%d = %p\n", index, bp);
	}
	else if (index == 18){
		printf("exp_listp%d!!\n", index);
		if (mode == 0)
			return exp_listp18;
		else
			exp_listp18 = bp;
		printf("new exp_listp : exp_listp%d = %p\n", index, bp);
	}
	else if (index == 19){
		printf("exp_listp%d!!\n", index);
		if (mode == 0)
			return exp_listp19;
		else
			exp_listp19 = bp;
		printf("new exp_listp : exp_listp%d = %p\n", index, bp);
	}
	else if (index == 20){
		printf("exp_listp%d!!\n", index);
		if (mode == 0)
			return exp_listp20;
		else
			exp_listp20 = bp;
		printf("new exp_listp : exp_listp%d = %p\n", index, bp);
	}
	else{
		printf("Error from select_seglist : index...out of range\n");
		return NULL;
	}
	return NULL;
}
*/
static void *coalesce(void *bp){
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

//	void * exp_listp;
//	printf("coalesce START : bp %p, next %p, prev %p\n", bp, (void *)GET(bp), (void *)GET(bp+WSIZE));

    if (prev_alloc && next_alloc){
//		printf("coalesce : case 1\n");

/*		exp_listp = select_seglist(GET_SIZE(HDRP(bp)), 0, NULL);
		PUT(bp, (unsigned int)exp_listp);
		PUT(bp + WSIZE, (unsigned int)NULL);
		if (exp_listp != NULL)
			PUT(exp_listp + WSIZE, (unsigned int)bp);
		select_seglist(GET_SIZE(HDRP(bp)), 1, bp);*/

//		exp_listp = select_seglist(GET_SIZE(HDRP(bp)), 0, NULL);
//		printf("coalesce : case 1...exp_listp %p\n",exp_listp);
		if (exp_listp != NULL){
				PUT(bp, (unsigned int)exp_listp);
				PUT(exp_listp + WSIZE, (unsigned int)bp);
				PUT(bp + WSIZE, (unsigned int)NULL);
//				select_seglist(GET_SIZE(HDRP(bp)), 1, bp);
				exp_listp = bp;
		}
		else{
			PUT(bp, (unsigned int)NULL);
			PUT(bp + WSIZE, (unsigned int)NULL);
//			select_seglist(GET_SIZE(HDRP(bp)), 1, bp);
			exp_listp = bp;
		}


		//printf("case 1 FINISHED\n");
//		printf("coalesce FINISHED : bp %p, next %p, prev %p\n", bp, (void *)GET(bp), (void *)GET(bp + WSIZE));
      	return bp;
    }
	
	else if (prev_alloc && !next_alloc){
//		printf("coalesce : case 2\n");
		if ((void *)GET(NEXT_BLKP(bp) + WSIZE) != NULL)
			PUT((void *)GET(NEXT_BLKP(bp) + WSIZE), GET(NEXT_BLKP(bp)));
		else{
//			select_seglist(GET_SIZE(HDRP(NEXT_BLKP(bp))), 1, (void *)GET(NEXT_BLKP(bp)));
			exp_listp = (void *)GET(NEXT_BLKP(bp));
		}
		if ((void *)GET(NEXT_BLKP(bp)) != NULL)
			PUT((void *)GET(NEXT_BLKP(bp)) + WSIZE, GET(NEXT_BLKP(bp) + WSIZE));

//		if ((void *)GET(NEXT_BLKP(bp)) != NULL){
//			PUT(GET(NEXT_BLKP(bp)) + WSIZE, (unsigned int)NULL);
//			select_seglist(GET_SIZE(HDRP(NEXT_BLKP(bp))), 1, (void *)GET(NEXT_BLKP(bp)));
//		}
//		else
//			select_seglist(GET_SIZE(HDRP(NEXT_BLKP(bp))), 1, NULL);
//
       	size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
		PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
		
		/*
		exp_listp = select_seglist(GET_SIZE(HDRP(bp)), 0, NULL);
		PUT(bp, (unsigned int)exp_listp);
		PUT(bp + WSIZE, (unsigned int)NULL);
		if (exp_listp != NULL)
			PUT(exp_listp + WSIZE, (unsigned int)bp);
		select_seglist(GET_SIZE(HDRP(bp)), 1, bp);
		*/
//		exp_listp = select_seglist(GET_SIZE(HDRP(bp)), 0, NULL);
		if (exp_listp != NULL){
//				if ((void *)GET(exp_listp) != bp){
					PUT(bp, (unsigned int)exp_listp);
					PUT(exp_listp + WSIZE, (unsigned int)bp);
					PUT(bp + WSIZE, (unsigned int)NULL);
//					select_seglist(GET_SIZE(HDRP(bp)), 1, bp);
					exp_listp = bp;
//				}
		}
		else{
			PUT(bp, (unsigned int)NULL);
			PUT(bp + WSIZE, (unsigned int)NULL);
//			select_seglist(GET_SIZE(HDRP(bp)), 1, bp);
			exp_listp = bp;
		}

		//printf("case 2 FINISHED\n");
  	}

    else if(!prev_alloc && next_alloc){
//		printf("coalesce : case 3\n");
		
		if ((void *)GET(PREV_BLKP(bp) + WSIZE) != NULL)
			PUT((void *)GET(PREV_BLKP(bp) + WSIZE), GET(PREV_BLKP(bp)));
		else{
			//select_seglist(GET_SIZE(HDRP(PREV_BLKP(bp))), 1, (void *)GET(PREV_BLKP(bp)));
			exp_listp = (void *)GET(PREV_BLKP(bp));
		}
		//printf("here?\n");	
		if ((void *)GET(PREV_BLKP(bp)) != NULL)
			PUT((void *)GET(PREV_BLKP(bp)) + WSIZE, GET(PREV_BLKP(bp)+WSIZE));
		//printf("Or here?\n"); 
		
		

//		if ((void *)GET(PREV_BLKP(bp)) != NULL){
//			PUT(GET(PREV_BLKP(bp)) + WSIZE, (unsigned int)NULL);
//			select_seglist(GET_SIZE(HDRP(PREV_BLKP(bp))), 1, (void *)GET(PREV_BLKP(bp)));
//		}
//		else
//			select_seglist(GET_SIZE(HDRP(PREV_BLKP(bp))), 1, NULL);

        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
       	PUT(FTRP(bp), PACK(size, 0));
		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
		
		/*
		exp_listp = select_seglist(GET_SIZE(HDRP(bp)), 0, NULL);
		PUT(bp, (unsigned int)exp_listp);
		PUT(bp + WSIZE, (unsigned int)NULL);
		//printf("exp_listp %p\n", exp_listp);
		if (exp_listp != NULL)
			PUT(exp_listp + WSIZE, (unsigned int)bp);
		select_seglist(GET_SIZE(HDRP(bp)), 1, bp);
		*/
		//exp_listp = bp;

//		exp_listp = select_seglist(GET_SIZE(HDRP(bp)), 0, NULL);
		if (exp_listp != NULL){
//				if ((void *)GET(exp_listp) != bp){

					//printf("How did you come here? exp_listp : %p, bp : %p\n", exp_listp, bp);
					PUT(bp, (unsigned int)exp_listp);	
					PUT(exp_listp + WSIZE, (unsigned int)bp);
					PUT(bp + WSIZE, (unsigned int)NULL);
					//select_seglist(GET_SIZE(HDRP(bp)), 1, bp);
					exp_listp = bp;
//				}
		}
		else{
			PUT(bp, (unsigned int)NULL);
			PUT(bp + WSIZE, (unsigned int)NULL);
			//select_seglist(GET_SIZE(HDRP(bp)), 1, bp);
			exp_listp = bp;
		}
		//printf("case 3 FINISHED\n");
	}

	else{
//		printf("coalesce : case 4\n");
		
		if ((void *)GET(PREV_BLKP(bp) + WSIZE) != NULL)
			PUT((void *)GET(PREV_BLKP(bp) + WSIZE), GET(PREV_BLKP(bp)));
		else{
//			select_seglist(GET_SIZE(HDRP(PREV_BLKP(bp))), 1, (void *)GET(PREV_BLKP(bp)));
			exp_listp = (void *)GET(PREV_BLKP(bp));
		}
		if ((void *)GET(PREV_BLKP(bp)) != NULL)
			PUT((void *)GET(PREV_BLKP(bp)) + WSIZE, GET(PREV_BLKP(bp)+WSIZE));


		if ((void *)GET(NEXT_BLKP(bp) + WSIZE) != NULL)
			PUT((void *)GET(NEXT_BLKP(bp) + WSIZE), GET(NEXT_BLKP(bp)));
		else{
//			select_seglist(GET_SIZE(HDRP(NEXT_BLKP(bp))), 1, (void *)GET(NEXT_BLKP(bp)));
			exp_listp = (void *)GET(NEXT_BLKP(bp));
		}
		if ((void *)GET(NEXT_BLKP(bp)) != NULL)
			PUT((void *)GET(NEXT_BLKP(bp)) + WSIZE, GET(NEXT_BLKP(bp)+WSIZE));
	    
/*
		if ((void *)GET(NEXT_BLKP(bp)) != NULL){
			PUT(GET(NEXT_BLKP(bp)) + WSIZE, (unsigned int)NULL);
			select_seglist(GET_SIZE(HDRP(NEXT_BLKP(bp))), 1, (void *)GET(NEXT_BLKP(bp)));
		}
		else
			select_seglist(GET_SIZE(HDRP(NEXT_BLKP(bp))), 1, NULL);

		if ((void *)GET(PREV_BLKP(bp)) != NULL){
			PUT(GET(PREV_BLKP(bp)) + WSIZE, (unsigned int)NULL);
			select_seglist(GET_SIZE(HDRP(PREV_BLKP(bp))), 1, (void *)GET(PREV_BLKP(bp)));
		}
		else
			select_seglist(GET_SIZE(HDRP(PREV_BLKP(bp))), 1, NULL);
*/
		size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp)));
	    PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
		PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
		bp = PREV_BLKP(bp);
		/*
//		exp_listp = select_seglist(GET_SIZE(HDRP(bp)), 0, NULL);
		PUT(bp, (unsigned int)exp_listp);
		PUT(bp + WSIZE, (unsigned int)NULL);
		if (exp_listp != NULL)
			PUT(exp_listp + WSIZE, (unsigned int)bp);
		//select_seglist(GET_SIZE(HDRP(bp)), 1, bp);
		exp_listp = bp;
*/
		//exp_listp = select_seglist(GET_SIZE(HDRP(bp)), 0, NULL);
		if (exp_listp != NULL){
//				if ((void *)GET(exp_listp) != bp){
					PUT(bp, (unsigned int)exp_listp);
					PUT(exp_listp + WSIZE, (unsigned int)bp);
					PUT(bp + WSIZE, (unsigned int)NULL);
//					select_seglist(GET_SIZE(HDRP(bp)), 1, bp);
					exp_listp = bp;
//				}
		}
		else{
			PUT(bp, (unsigned int)NULL);
			PUT(bp+WSIZE, (unsigned int)NULL);
//			select_seglist(GET_SIZE(HDRP(bp)), 1, bp);
			exp_listp = bp;
		}

		//printf("case 4 FINISHED\n");
	}

//	printf("coalesce FINISHED : bp %p, next %p, prev %p\n", bp, (void *)GET(bp), (void *)GET(bp + WSIZE));

    return bp;
}

static void *find_fit(size_t asize){
	void *bp;

	for (bp = exp_listp ; bp != NULL; bp = (void *)GET(bp)){
		if (!GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp))))
			return bp;
	}
	return NULL;
}

static void place(void *bp, size_t asize){
    size_t csize = GET_SIZE(HDRP(bp));
//	printf("place START : bp %p, next %p, prev %p\n", bp, (void *)GET(bp), (void *)GET(bp + WSIZE));
	if ((csize - asize) >= (2*DSIZE)){
//		printf("place : case 1\n");
		if ((void *)GET(bp) != NULL){
//			PUT(GET(bp) + WSIZE, (unsigned int)NULL);
//			select_seglist(GET_SIZE(HDRP(bp)), 1, (void *)GET(bp));
			PUT(GET(bp) + WSIZE, GET(bp + WSIZE));
		}
//		else{
//			select_seglist(GET_SIZE(HDRP(bp)), 1, NULL);
//		}
		if ((void *)GET(bp + WSIZE) != NULL){
			PUT(GET(bp + WSIZE), GET(bp));
		}
		else
			exp_listp = (void *)GET(bp);
//		printf("case 1 : deleted current free block...bp %p, next %p, prev %p\n", bp, (void *)GET(bp), (void *)GET(bp + WSIZE));
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        	//exp_listp = (void *)GET(bp);
		//printf("case 1 : changed block which pointer bp points to allocated block...bp %p, next %p, prev %p\n", bp, (void *)GET(bp), (void *)GET(bp + WSIZE));
      	//if ((void *)GET(bp) != NULL)
        	//PUT((void *)GET(bp) + WSIZE, GET(bp+WSIZE));
		bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(csize-asize, 0));
        PUT(FTRP(bp), PACK(csize-asize, 0));
	//	printf("case 1 : move to coalescing...bp %p, next %p, prev %p\n", bp, (void *)GET(bp), (void *)GET(bp + WSIZE));
		coalesce(bp);
		//printf("case 1 FINISHED\n");
	}
    else{
//		printf("place : case 2\n");
//		if ((void *)GET(bp) != NULL){
//			printf("HERE!!!HERE!!!! GET(bp) %p, GET(bp) next %p\n", (void *)GET(bp), (void *)GET((void *)GET(bp)));
//			PUT(GET(bp) + WSIZE, (unsigned int)NULL);
//			select_seglist(GET_SIZE(HDRP(bp)), 1, (void *)GET(bp));
//		}
//		else
//			select_seglist(GET_SIZE(HDRP(bp)), 1, NULL);
		if ((void *)GET(bp) != NULL)
			PUT(GET(bp) + WSIZE, GET(bp + WSIZE));
		if ((void *)GET(bp + WSIZE) != NULL)
			PUT(GET(bp + WSIZE), GET(bp));
		else
			exp_listp = (void *)GET(bp);
       	PUT(HDRP(bp), PACK(csize, 1));
        PUT(FTRP(bp), PACK(csize, 1));
//			exp_listp = (void *)GET(bp);
		
		//if ((void *)GET(bp) != NULL)
			//PUT((void *)GET(bp) + WSIZE, GET(bp+WSIZE));
		//printf("case 2 FINISHED\n");
  	}
//	printf("place FINISHED : bp %p, next %p, prev %p\n", bp, (void *)GET(bp), (void *)GET(bp+WSIZE));
}

static void *extend_heap(size_t words){
    char *bp;
    size_t size;
	//printf("EXTEND!! : words %u\n", words);
	size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
    if ((long)(bp = mem_sbrk(size)) == -1)
        return NULL;

    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));
//	printf("extend_heap result : size %u, bp %p\n", size, bp);
    return coalesce(bp);
}

