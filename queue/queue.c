#include "queue.h"

const int_least8_t TRUE = 1;
const int_least8_t FALSE = 0;

queue_t* create_queue(int_least8_t size)
{
	queue_t* queue = (queue_t*)malloc(sizeof(queue_t));
	queue->head = NULL;
	queue->tail = NULL;
	queue->el_size = size;

	return queue;
}

void delete_queue(queue_t* queue)
{
	if(((queue->head == NULL) && (queue->tail == NULL)) == FALSE)
	{
		node_t* current;
		while(queue->head != NULL)
		{
			current = queue->head;
			queue->head = current->next;
			free(current->el_value);
			free(current);
		}
	}

	free(queue);
	return;
}

void enqueue(queue_t* queue, node_t* node)
{
	if(((queue->head == NULL) && (queue->tail == NULL)) == FALSE)
	{
		node->next = queue->head;
		(queue->head)->prev = node;
		queue->head = node;
	}
	else
	{
		queue->head = node;
		queue->tail = node;
	}
}

node_t dequeue(queue_t* queue)
{
	node_t node; /*Create node for return*/
	node_t* temp = queue->tail; /*Create temp node to hold address of tail*/

	if(queue->head != queue->tail)
	{
		node.el_value = malloc(queue->el_size);
		memcpy(node.el_value, (queue->tail)->el_value, queue->el_size);
		node.next = NULL;
		node.prev = NULL;
		queue->tail = temp->prev; /*Return tail to previous element*/
		(queue->tail)->next = NULL; /*Set tail next to null (New end of list)*/
		free(temp->el_value);
		free(temp); /*Free ex-tail*/
	}
	else /*Last element of queue*/
	{
		node.el_value = malloc(queue->el_size);
		memcpy(node.el_value, (queue->tail)->el_value, queue->el_size);
		node.next = NULL;
		node.prev = NULL;
		queue->head = NULL;
		queue->tail = NULL;
		free(temp->el_value);
		free(temp); /*Free ex-tail*/
	}

	return node;
}

int_least8_t is_empty(const queue_t* queue)
{
	int_least8_t return_val;
	if((queue->head == NULL) && (queue->tail == NULL))
	{
		return_val = TRUE;
	}
	else
	{
		return_val = FALSE;
	}

	return return_val;
}

const node_t* const front(const queue_t* queue)
{
	return queue->tail;
}

void empty_queue(queue_t* queue)
{
	while((queue->tail != NULL) && (queue->head != NULL))
	{
		dequeue(queue);
	}
}

node_t* create_node(void* value, int_least8_t size) /*Size of single element*/
{
	node_t* node = (node_t*)malloc(sizeof(node_t));
	node->next = NULL;
	node->prev = NULL;
	node->el_value = malloc(size);
	memcpy(node->el_value, value, size);

	return node;
}
