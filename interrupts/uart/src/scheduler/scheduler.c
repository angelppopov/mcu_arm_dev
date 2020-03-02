/*
 * scheduler.c
 *
 * Created: 2/20/2020 10:02:40 AM
 *  Author: Angel Popov
 */ 
#include "scheduler.h"
#include "handler.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <delay.h>

/* Dynamically allocated buffer of events to be executed */
static q_event *event_queue;
static uint8_t q_size;

volatile event_scheduler scheduler;

/* Get first structure from the buffer */
#define queue_is_empty (q_size == 0)

void scheduler_init(){
	scheduler.add = append;
	scheduler.process = execute;
}

static volatile void append(int id, int data_size){
	printf("Event is added to scheduler with id: %d\n", id);
	q_size++;
	if(q_size <= 1){
		/* Allocate memory if it has not been previously allocated */
		event_queue = (q_event *)malloc(sizeof(q_event));
		event_queue->id = id;
		event_queue->data_size = data_size;
	}else{
		/* Resize the memory block if has been allocated */
		event_queue = (q_event *)realloc(event_queue, sizeof(q_event) * q_size);
		/* Assign to data to last memory address */
		q_event *next_in_queue = (q_event *) (event_queue + q_size - 1);
		next_in_queue->id = id;
		next_in_queue->data_size = data_size;
	}
}

static volatile void execute(){
	if(!queue_is_empty){
		cpu_relax();
		/* Process the first event in the event queue */
		bool success = event_trigger(event_queue->id, event_queue->data_size);
		if(success){
			if(q_size > 1){
				/* Pop the first event when executed */
				event_queue = remove_from_queue();
			}else{
				q_size = 0;
			}
		}
	}else{
		cpu_relax();
	}
}

static q_event* remove_from_queue(){
	/* Allocate with a size less than the current one */
	q_event *tmp = (q_event *)malloc((q_size - 1) * sizeof(q_event));
	/* Copy everything after the first index */
	memcpy( tmp, (event_queue + 1), (q_size - 1) * sizeof(q_event));
	/* Free event queue when copied to tmp  */
	free(event_queue);
	/* Return tmp to be assigned to event_queue */
	q_size--;
	return tmp;
}

void cpu_relax(){
	delay_ms(1);
}