///////////////////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Queue μέσω του ADT Stack (dependent data structure)
//
///////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>

#include "ADTQueue.h"
#include "ADTStack.h"
#include "ADTVector.h"


// Ένα Queue είναι pointer σε αυτό το struct.
struct queue {
	Stack stack1;
	Stack stack2;
	int size;
	Pointer front_v;
	DestroyFunc destroy_value;
	int steps;
};


Queue queue_create(DestroyFunc destroy_value) {
	Queue queue = malloc(sizeof(*queue));
	queue->destroy_value = destroy_value;
	queue->front_v = malloc(sizeof(Pointer));
	queue->size = 0;
	queue->stack1 = stack_create(destroy_value);
	queue->stack2 = stack_create(destroy_value);
	queue->steps = 1;
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
	if(stack_size(queue->stack1) == 1 && stack_size(queue->stack2) == 0)
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
	// If stack2 is empty, move all values from stack1 to stack2 so we can remove values more easily, 
	// because they will be reversed, else remove the top value of stack2
	// which will be the front value of the queue since the values are reversed in this stack
	if(stack_size(queue->stack2) == 0) {
		queue->steps = stack_size(queue->stack1);
		while(stack_size(queue->stack1) != 0) {
			stack_insert_top(queue->stack2, stack_top(queue->stack1));
			stack_remove_top(queue->stack1);
		}
		if(stack_size(queue->stack2) != 0) {
			stack_remove_top(queue->stack2);
			// New front value, top of stack2
			queue->front_v = stack_top(queue->stack2);
		}

	}else {
		stack_remove_top(queue->stack2);

		if(stack_size(queue->stack2) != 0) 
			//New front value, top of stack2
			queue->front_v = stack_top(queue->stack2);

		queue->steps = 1;
		
	}

	queue->size -= 1;
}

DestroyFunc queue_set_destroy_value(Queue queue, DestroyFunc destroy_value) {
	queue->steps = 1;
	return stack_set_destroy_value(queue->stack1,destroy_value);
}

void queue_destroy(Queue queue) {
	if(stack_size(queue->stack1) > 0 || stack_size(queue->stack2) > 0 ) 
		queue->steps = stack_size(queue->stack1) + stack_size(queue->stack2);
	else
		queue->steps = 1;
	
	stack_destroy(queue->stack1);
	stack_destroy(queue->stack2);
	free(queue);
}

int queue_steps(Queue queue) {
	return queue->steps;
}