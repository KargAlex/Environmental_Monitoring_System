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
#define PASSWORD "1234abcd"
#define AEM "10662"
#define MENU  "=== MENU ===\r\na)Increase read and print frequency by 1s (max 2s)\r\nb)Reduce read and print frequency by 1s (max 10s)\r\nc)Print Temperature/Humidity/Both.\r\nd)Print system status and last values.\r\n"


Queue rx_queue; // Queue for storing received characters

// === Global State ===
uint8_t buff_index;						
char buffer[BUFF_SIZE]; 			

// === Global Flags === 
bool password_entered = false;		// Set to True if password is entered, enables password validation
bool AEM_entered = false;			// True if AEM is entered, enables AEM validation
bool password_requested = false; 	// Prevents "Enter password:" from printing repeatedly
bool AEM_requested = false;			// Prevents "Enter AEM:" from printing repeatedly
bool instruction_selected = false;
bool menu_option_entered = false;
bool menu_option_requested = false;


// === Functions ===
void handle_input() {
	uint8_t rx_char = 0;

	// Process input characters
	while (queue_dequeue(&rx_queue, &rx_char)) {
		if (rx_char == 0x7F) {  // Backspace
			if (buff_index > 0) {
				buff_index--;
				uart_tx(rx_char);
			}
		}
		else if (rx_char == '\r') {  // Enter
			uart_print("\r\n");
			// Password
			if (password_requested)
				password_entered = true;
			// AEM
			else if (AEM_requested)
				AEM_entered = true;
			// Menu
			else if (menu_option_requested) {
				menu_option_entered = true;
					
			}
		}
		else if (buff_index < BUFF_SIZE - 1) {
			buffer[buff_index++] = (char)rx_char;
			uart_tx(rx_char);
		}
	}
	
}

void check_password() {
	password_entered = false;
	password_requested = false;

	if (buff_index == (sizeof(PASSWORD) - 1) && strncmp(buffer, PASSWORD, buff_index) == 0) {
		uart_print("Correct password.\r\n");
		// Now transition to AEM input
		AEM_requested = true;
		AEM_entered = false;
		buff_index = 0;
		uart_print("Enter AEM: ");
	} else {
		uart_print("Wrong password.\r\n");
		//buff_index = 0;
	}
}


void check_AEM() {
	AEM_entered = false;
	AEM_requested = false;

	if (buff_index == sizeof(AEM) - 1 && strncmp(buffer, AEM, buff_index) == 0) {
		uart_print("Correct AEM.\r\n");
		menu_option_requested = true;
		buff_index = 0;
		uart_print(MENU);  // Show  main menu
		uart_print("Select an option: ");
		
	} 
	else {
		uart_print("Wrong AEM.\r\n");

		// Re-request AEM, do NOT fall back to password
		AEM_requested = true;
		AEM_entered = false;
		buff_index = 0;
		uart_print("Enter AEM: ");
	}
}

void check_option() {
	menu_option_entered = false;

	// Too many characters
	if (buff_index > 1) {
		uart_print("Input either a, b, c or d.\r\nSelect an option: ");
		buff_index = 0;
	}
	// No character
	else if (buff_index == 0)
		uart_print("No input selected.\r\nSelect an option: ");
	// 1 character
	else
		if(buffer[buff_index - 1] < 'a' && buffer[buff_index - 1] > 'd') {
			uart_print("Input either a, b, c or d.\r\nSelect an option: ");
			buff_index = 0;
		}
		else {
			buff_index = 0;
			if(buffer[buff_index] == 'a') {
				// do_something()
			}
			else if(buffer[buff_index] == 'b') {
				// do_something()
			}
			else if(buffer[buff_index] == 'c') {
				// do_something()
			}
			else { 										 // "d"
				// do_something()
			}
		}
}


void uart_rx_isr(uint8_t rx) {
	if (rx >= 0x00 && rx <= 0x7F) {
		queue_enqueue(&rx_queue, rx);
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

	while (1) {
		// Ask for password
		if (!password_requested && !AEM_requested && !menu_option_requested) {
			if (!password_entered) {
				uart_print("Enter password: ");
				password_requested = true;
				buff_index = 0;
			}
		}	
		handle_input();

		if (password_entered)
			check_password();

		handle_input();
		
		if (AEM_entered) // Only allow AEM check after password
			check_AEM();
		
		if (menu_option_entered)
			check_option();
	}
}
