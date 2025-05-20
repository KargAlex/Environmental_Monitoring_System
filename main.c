#include "platform.h"
#include <stdio.h>
#include <stdint.h>
#include "uart.h"
#include <string.h>
#include "queue.h"
#include "leds.h"
#include "delay.h"
#include "timer.h"
#include "gpio.h"
#include <stdbool.h>

// === Constants ===
#define BUFF_SIZE 64
#define PASSWORD   "1234abcd"
#define AEM        "10662"
#define MENU_TEXT  "======= MENU =======\r\na)Increase read and print frequency by 1s (max 2s)\r\n" \
                   "b)Reduce read and print frequency by 1s (max 10s)\r\n" \
                   "c)Print Temperature/Humidity/Both.\r\n" \
                   "d)Print system status and last values.\r\n"

// === Globals ===
 Queue   rx_queue;
 uint8_t buff_index;
 char    buffer[BUFF_SIZE];

// State flags
 bool password_entered = false;  		// set when user hits Enter after typing
 bool AEM_entered = false;
 bool menu_option_entered = false;

 bool password_requested = false;  // Show relevant promt
 bool AEM_requested = false;
 bool menu_option_requested = false;

 bool password_validated = false;  		// did we pass password?
 bool AEM_validated = false;  				// did we pass AEM?
 bool menu_option_validated = false;	// is option input correct?


// === ISR & Input ===
void uart_rx_isr(uint8_t rx) {
	if(rx <= 0x7F) {
		queue_enqueue(&rx_queue, rx);
	}
}

void handle_input(void) {
	uint8_t rx;
	// Decide which prompt to show
	if(!password_requested && !AEM_requested && !menu_option_requested) {
		if(!password_validated) {
			buff_index = 0;
			uart_print("Enter password: ");
			password_requested = true;
			//return;
		}
		else if(!AEM_validated) {
			buff_index = 0;
			uart_print("Enter AEM: ");
			AEM_requested = true;
			//return;
		}
		else {
			if(!menu_option_validated) {
				buff_index = 0;
				uart_print(MENU_TEXT);
				uart_print("Select an option: ");
				menu_option_requested = true;
				//return;
			}
		}
	}

	// Process characters
	while(queue_dequeue(&rx_queue, &rx)) {
		if(rx == 0x7F) {  // backspace
			if(buff_index > 0) {
				buff_index--;
				uart_tx(rx);
				}
			}
			else if(rx == '\r') {  // enter
				uart_print("\r\n");
				if(password_requested)      
					password_entered = true;
				else if(AEM_requested)      
					AEM_entered = true;
				else if(menu_option_requested) 
					menu_option_entered = true;
			}
			else if (buff_index < BUFF_SIZE - 1) {
				buffer[buff_index++] = (char)rx;
				uart_tx(rx);
			}
	}
}

// === Check functions ===
void check_password() {
	password_requested = false;
	if(buff_index == (sizeof(PASSWORD)-1) && strncmp(buffer, PASSWORD, buff_index) == 0 )
	{
		uart_print("Correct password.\r\n");
		password_validated = true;
	} else {
		uart_print("Wrong password.\r\n");
	}
}

void check_AEM() {
	AEM_requested = false;
	if(buff_index == (sizeof(AEM)-1) && strncmp(buffer, AEM, buff_index) == 0 )
	{
		uart_print("Correct AEM.\r\n");
		AEM_validated = true;
	} 
	else {
		uart_print("Wrong AEM.\r\n");
	}
}

void check_option() {
	menu_option_requested = false;
	if(buff_index != 1 || buffer[0] < 'a' || buffer[0] > 'd') {
		uart_print("Invalid selection (a-d).\r\n\r\n");
	} 
	else {
		switch(buffer[0]) {
			case 'a': 
				menu_option_validated = true;
				// do_something()
				break;
			case 'b': 
				menu_option_validated = true;
				// do_something()
				break;
			case 'c':
				menu_option_validated = true;
				// do_something()
				break;
			case 'd': 
				menu_option_validated = true;
				// do_something()
				break;
		}
	}
}

// === Main ===
int main() {
	queue_init(&rx_queue, 64);
	uart_init(115200);
	uart_set_rx_callback(uart_rx_isr);
	uart_enable();
	__enable_irq();
	uart_print("\r\n");

	while(1) {
		handle_input();

		if(password_entered) {
			password_entered = false;
			check_password();
		}
		else if(AEM_entered) {
			AEM_entered = false;
			check_AEM();
		}
		else if(menu_option_entered) {
			menu_option_entered = false;
			check_option();
		}
	}
}
