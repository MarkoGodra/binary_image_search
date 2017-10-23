#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <inttypes.h>
#include <stdlib.h>
#include <memory.h>

extern const int_least8_t FALSE;
extern const int_least8_t TRUE;

typedef struct node_st
{
	void* el_value;
	struct node_st* next;
	struct node_st* prev;
}node_t;

typedef struct
 {
	node_t* head;
	node_t* tail;
	int_least8_t el_size;
 }queue_t;

queue_t* create_queue(int_least8_t size);
void delete_queue(queue_t* queue);
void enqueue(queue_t* queue, node_t* node);
node_t dequeue(queue_t* queue);
const node_t* const front(const queue_t* queue); /*Peak at front (First inserted element)*/
int_least8_t is_empty(const queue_t* queue);
void empty_queue(queue_t* queue);
node_t* create_node(void* value, int_least8_t size); /*Size of single element*/

#endif
