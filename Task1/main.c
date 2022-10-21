/*
 * Task1.c
 *
 * Created: 21/10/2022 17:07:38
 * Author : Eleanor
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

/*
Port register bits to Arduino pin numbers (in C binary number mapping order)
Port D  : 0  0  0  0  0  0  0  0
Arduino : 7  6  5  4  3  2  1  0
Port B  : 0  0  0  0  0  0  0  0
Arduino : -  -  13 12 11 10 9  8
Port C  : 0  0  0  0  0  0  0  0
Arduino : -  R  A5 A4 A3 A2 A1 A0
*/

// Define the binary value to set pin 6 only as output using port D
#define DDRD_PIN_6_OUT 0b01000000

// Flag to indicate a full button press has been registered to main loop.
int button_pressed;

// Store the duty cycle values to switch between
#define N_DUTY_CYCLE_VALUES 4
const float DUTY_CYCLE_VALUES [N_DUTY_CYCLE_VALUES] = {0.0, 25, 62.5, 87.5};

// Program state variable to track next duty cycle output to set
uint8_t next_duty_cycle_idx;

//External interrupt_zero ISR, updates.
ISR (INT0_vect) 
{
	// Add one to the button pressed flag (using addition/subtraction instead
	// of value setting to allow for queue type use)
	++button_pressed;
}

void set_pwm_dutycycle(float percentage)
{
	
}

// Set up pin states and values, and initialize any internal hardware which
// has a consistent use in the program
void init()
{
	// Initialize button press flag to false (0 presses queued)
	button_pressed = 0;
	
	// Only output is motor PWM min, so set port D to output this pin only
	DDRD = DDRD_PIN_6_OUT;
	// Initialize port D output to all zeros (only output pin is off)
	PORTD = 0x00;
	
	// Start outputting the first PWM duty cycle
	next_duty_cycle_idx = 0;
	
	// Set up interrupt for button press (falling-edge)
	EIMSK = (1 << INT0); // Enable INT0 Interrupt
	EICRA = 0x02; // Set interrupt trigger to falling edge on INT0
	sei(); // Enable interrupts
}

int main(void)
{
	// Run initialization function
	init();
	// Set initial PWM output (which starts at 0%) and prepare next cycle index
	set_pwm_dutycycle(next_duty_cycle_idx++);
	// Main Program Loop
    while (1) 
    {
		if (button_pressed)
		{
			set_pwm_dutycycle(next_duty_cycle_idx++);
			if (next_duty_cycle_idx >= N_DUTY_CYCLE_VALUES)
			{
				next_duty_cycle_idx = 0;
			}
			--button_pressed;
		}
    }
}

