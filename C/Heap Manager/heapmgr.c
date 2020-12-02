#include "memorymgr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ */
int nexteight(int current);
void setlength(int *block, int size);
int *lastB;
int *firstB;
/* ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ */
int *firstBlock() {
     return firstB;
}

int *lastBlock() {
     return lastB;
}

int nexteight(int current){
     if (current%8==0) return current;
     else{
	int next = current%8;
	return current + (8-next);
     }
}

void setlength(int *block, int size){
     *block = size;
}

int *nextBlock(int *p) {
    int size = *p;
    if (isAllocated(p)==1){
	 size--;}
    int *nextBlock;
    nextBlock = p + (size)/4;
    return nextBlock;
}

int  isAllocated(int *p){
     return ((*p)%2);
}

int isMarked(int *p) {
     return ((*p)%4);
}

void initmemory(int size) {
     int *first = 0;
     int *last = 0;
     int heaplength = nexteight(size + 12); //adds unused, sentinel and header
     int *heap = malloc(heaplength); //allocate the entire space
     first = heap+1; //guide the pointer to the first block's header
     setlength(first, heaplength-8);
     last = heap + (heaplength/4) - 1; //go all the way to the end and come back one
     setlength(last, 0);
     firstB = first;
     lastB = last;
}


void *myalloc(int length) {
     int *second;
     int size = nexteight(length+4);
     int *checkblock = firstBlock();
     int found = 0;
     while (*checkblock!=0 && found!=1 ){
	if (isAllocated(checkblock)==0 && (*checkblock)>=size){ //if allocated and memory is enough
	     found = 1;
	     second = checkblock + (size/4); //set pointer to new unalloc to current + size of malloced
	     *second = *checkblock - size; //set the size of this to the old length - size of malloced
	     *checkblock = size+1; 
	     return (void *) checkblock;
	}
	else {
	    checkblock = nextBlock(checkblock); //check the next block
	}
     }
     if (found == 0) return NULL;

}

void  myfree(void *ptr){
     int *freeptr = ptr;
     if ((*freeptr)%4 == 3) *freeptr = (*freeptr)-3;
     else *freeptr = (*freeptr)-1;
}

void  coalesce(){
     int *checkNext;
     int *checkBlock = firstBlock();
     while (*(nextBlock(checkBlock))!=0) {
	if (isAllocated(checkBlock)==1) {
	     checkBlock = nextBlock(checkBlock);
	}
	else if (isAllocated(checkBlock)==0) {
	     checkNext = nextBlock(checkBlock);
	     if (isAllocated(checkNext) == 0) {
		*checkBlock = *checkBlock + *checkNext;
	     }
	     else{
		checkBlock = nextBlock(checkNext);
	     }
	}
     }
}

void  printallocation(){
     int i = 0;
     int size;
     int alloc = 0;
     int *block = firstBlock();
     int *last = lastBlock();
     while ((*block)!=0){
	char s[15];
	if (isAllocated(block)==0) {
	     size = *block;
	     strcpy(s, "unallocated");
	}
     	else if (isAllocated(block) == 1){
             size = (*block) - 1;
             if ((*block)%4==3) size = size - 2;
             strcpy(s, "allocated");
	    
	}
	printf("Block %d: size %d \t %s \n", i, size, s);
	block = nextBlock(block);
	i++;
     }
     printf("\n");
}

