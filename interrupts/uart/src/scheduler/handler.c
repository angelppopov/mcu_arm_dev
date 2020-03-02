/*
 * handler.c
 *
 * Created: 2/20/2020 10:03:16 AM
 *  Author: Angel Popov
 */

#include <asf.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "handler.h"
#include "scheduler.h"

#define locked	!ioport_get_pin_level(LED_0_PIN)
#define unlock	ioport_set_pin_level(LED_0_PIN, !LED_0_ACTIVE)
#define lock	ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE)

static struct object objects[6];
static int status = SUCCESS;

/* 
	This function appends to the event buffer
*/

void event_register(struct object *obj){
	/* Assign an event read function */
	objects[obj->id].input = obj->input;
	/* Assign an event output function */
	objects[obj->id].output = obj->output;
	printf("Object registered id: %d\n", obj->id);
}

volatile bool event_trigger(int id, int data_size){
	if(locked)
		return false;
	/* Lock event trigger */
	lock;
	/* Request buffer for the data */
	char *buffer = (char *)malloc(sizeof(char) * data_size);
	/* Get the event result */
	objects[id].input(buffer, data_size);
	/* Process the event */
	event_processing(id, buffer, data_size);
	/* Free up memory */
	free(buffer);
	/* Release the flag */
	unlock;
	/* Return true to other event triggers */
	return true;
}

static inline void event_processing(int id, char *data, int size){
	
	printf("event processing id %d\n", id);
	switch(id){
		case SERIAL_RECEIVE:
		mcu_receive_handler(data, size);
		break;
		
	}
}


static void mcu_receive_handler(char *data, int size){
	
}