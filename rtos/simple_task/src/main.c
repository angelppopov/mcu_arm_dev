
#include <asf.h>


void button_listener_task(void *p);

int main (void)
{
	board_init();

	xTaskCreate(
				button_listener_task,
				"button_listener",
				200,
				(void *) 0,
				tskIDLE_PRIORITY,
				NULL
				);
				
	vTaskStartScheduler();
	
	while (1) {
		
	}
}


void button_listener_task(void *p){
	while(1){
		/* Is button pressed? */
		if (ioport_get_pin_level(BUTTON_0_PIN) == BUTTON_0_ACTIVE) {
			/* Yes, so turn LED on. */
			ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE);
			} else {
			/* No, so turn LED off. */
			ioport_set_pin_level(LED_0_PIN, !LED_0_ACTIVE);
		}
	}
}