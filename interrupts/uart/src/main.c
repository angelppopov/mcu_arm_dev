#include <asf.h>
#include <delay.h>
#include "serial/serial_port.h"
#include "scheduler/scheduler.h"

void wtd_enable(void);
extern event_scheduler scheduler;

int main (void)
{
	/* 
		Note: In board_init function wtd_disable is commented out
	*/	
	board_init();						// Used for dev board
	sysclk_init();						// Initialize System Clock
	scheduler_init();                   /*
	                                       Initialize the scheduler functions.
	                                    */
	serial_init();                      // Initialize UART on port C
	wtd_enable();						// Enable Watchdog timer
    
	printf("Working \n\n");
	
	while (1) {
        scheduler.process();
        wdt_restart(WDT);
	}
}


void wtd_enable(void){
	//Calculate count value for 3s (3,000,000 us) Watchdog
	uint32_t wdtCount = wdt_get_timeout_value(3000000UL, BOARD_FREQ_SLCK_XTAL);
	
	//Initialize Watchdog Timer
	wdt_init(WDT, WDT_MR_WDRSTEN |      // Watchdog fault or underflow causes reset,
	WDT_MR_WDDBGHLT,                    // Debug pauses watchdog
	wdtCount,                           // Restart value of Watchdog Counter
	wdtCount);                          // Delta value, used to prevent reset loops, unused here
	
}