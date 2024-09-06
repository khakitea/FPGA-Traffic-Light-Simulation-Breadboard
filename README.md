# Check out project list and summary on my [Notion](https://right-thrush-091.notion.site/Robyn-Jung-4c3bc2947eaa4279a296059c6bc376eb?pvs=4) page!

## Hardware:  
TIVA board, leds, buttons, etc.
Software:
IAR Embedded Workbench IDE
Debug extension - TI Stellaris

## Program Requirement:  
1. Reset button triggers the simulation to begin or restart.
2. Once the traffic light simulation begins, it starts at red light. After 5 seconds, it toggles to green light. This is the basic state the simulation resides in, until there is a button press.
3. Pedestrian button press
   - stop > btn press : stays in red
   - warn > btn press : stays in yellow
   - go > btn press : toggles to yellow and then red.

* Button presses are only valid when pressed long enough (1 sec).

## Files & Functions:  
traffic.c :  
 * task2c() : starts 2 sec timer when btn is pressed.
 * trafficIO_init() : initializing leds, buttons, and interrupts. 
 * trafficTimer_init() : initializing Timer 0A and 1A
 * Timer1A_Handler() : controls toggling of leds every 5 sec.
 * PortL_Hander() : triggered at the falling edge of btn presses and validates btn press > 2 sec.
 * red/yellow/green_on/off() : controls the led on/off using masking bits
  
main.c : calls initialization functions and task2c.  

## Approach:  
After structuring the FSM, I read and read the datasheet until it started talking to me. Then, I started coding.

