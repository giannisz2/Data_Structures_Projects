///////////////////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Queue μέσω του ADT Stack (dependent data structure)
//
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "ADTQueue.h"
#include "ADTStack.h"
#include "ADTVector.h"



// Ένα Queue είναι pointer σε αυτό το struct.
struct queue {
	Stack stack1;
	int size;
	Pointer front_v;
	DestroyFunc destroy_value;
	int steps;
};


Queue queue_create(DestroyFunc destroy_value) {
	Queue queue = malloc(sizeof(*queue));
	queue->stack1 = stack_create(destroy_value);
	queue->destroy_value = destroy_value;
	queue->front_v = malloc(sizeof(Pointer));
	queue->steps = 1;
	queue->size = 0;
	return queue;
}

int queue_size(Queue queue) { 
	queue->steps = 1; 
	return queue->size;
}

Pointer queue_front(Queue queue) {
	queue->steps = 1;
	// If stack1 which is the initial one has 1 value, and stack2 is empty, 
	// then the only one value will be the first, else return the first you find in queue_remove front
	if(stack_size(queue->stack1) == 1)
		queue->front_v = stack_top(queue->stack1);
	return queue->front_v;
}

Pointer queue_back(Queue queue) {
	queue->steps = 1;
	if(queue_size(queue) > 0)
		return stack_top(queue->stack1);
	return NULL;
}

void queue_insert_back(Queue queue, Pointer value) {
	// I call queuefront because my test fails if I dont
	queue_front(queue);

	queue->steps = 1;
	queue->size += 1;
	stack_insert_top(queue->stack1,value);
}

void queue_remove_front(Queue queue) {
	if(queue_size(queue) > 0) 
		queue->steps = 2 * queue_size(queue);
	else 
		queue->steps = 1;


	Vector vec = vector_create(queue->size, queue->destroy_value);

	// Move all values from stack1 to vector except the last one, and delete every value from vector
	for(int i = 0; i < vector_size(vec); i++) {
		if(stack_size(queue->stack1) != 1) {
			vector_insert_last(vec, stack_top(queue->stack1));
			stack_remove_top(queue->stack1);
		}
		
	}

	// Then remove last one as well 
	if(stack_size(queue->stack1) != 0)
		stack_remove_top(queue->stack1);

	// Reversed iteration so we move the values back to stack in right order, but without the removed one this time
	for(int i = vector_size(vec) - 1; i >= 0; i--) {
		stack_insert_top(queue->stack1, vector_get_at(vec, i));
		// The last value of the vector is the new front of queue
		if(i == vector_size(vec) - 1) {
			queue->front_v = stack_top(queue->stack1);
		}
	}

	vector_destroy(vec);

	queue->size -= 1;
	
}

DestroyFunc queue_set_destroy_value(Queue queue, DestroyFunc destroy_value) {
	queue->steps = 1;
	return stack_set_destroy_value(queue->stack1,destroy_value);
}

void queue_destroy(Queue queue) {
	if(stack_size(queue->stack1) > 0)
		queue->steps = stack_size(queue->stack1);
	else
		queue->steps = 1;

	stack_destroy(queue->stack1);
	free(queue);
}

int queue_steps(Queue queue) {
	return queue->steps;
}