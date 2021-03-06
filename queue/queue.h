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

 /*
  * Creates queue structure and returns pointer to it.
  * Size param is used to determine size of single element
  */
queue_t* create_queue(int_least8_t size);

/*
 * Deletes created queue structure.
 * param is pointer to queue structure taht needs to be deleted
 */
void delete_queue(queue_t* queue);

/*
 * Chains passed node structure into list
 */
void enqueue(queue_t* queue, node_t* node);

/*
 * Gets element on queues end (list's tail), makes a coppy of that
 * node, and returns it. Returned node pointer fields (next and prev)
 * are set to null
 */
node_t dequeue(queue_t* queue);

/*
 * Returns pointer to the end of queue (First item to be dequeued),
 * but doesen't dequeue that node, it just returns pointer to it
 */
const node_t* const front(const queue_t* queue);

/*
 * Tests if queue structure is empty
 */
int_least8_t is_empty(const queue_t* queue);

/*
 * Empties whole queue structure
 */
void empty_queue(queue_t* queue);

/*
 * Creates queueable node structure which can be passed to, enqueue function
 * Address of structure that needs to be queue needs to be provided,
 * and also size of that structure
 */
node_t* create_node(void* value, int_least8_t size); /*Size of single element*/

#endif
