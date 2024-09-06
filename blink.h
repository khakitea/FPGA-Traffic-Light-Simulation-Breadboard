/* University of Washington
* ECE/CSE 474,  [11/3/2022]
*
*   [Robyn Jung 1976925]
*   [Margot Enright-Down 1776361]
*      
*   [Lab 2][Task 1a & 2a & 2b]
*   Blinking Onboard LEDs in a Sequential Order
*   Task 1a: Only using timer
*   Task 2a: Using timer and interrupt
*   Task 2b: Switch manipulation and blinking using timer and interrupt
*/

#ifndef BLINK_H_
#define BLINK_H_

/**************************************************************
*                   FUNCTION PROTOTYPES
**************************************************************/
 //blinking onboard LED4 to LED1 in sequential order for 1Hz each
void task1a_setup();
void task1a();
 //Sequential Blinking of Onboard LEDs using Interrupt
void blinkIO_init();
void blinkTimer_init();
void Timer0A_Handler();
 //Task 2b switch manipulation handler
void PortJ_Handler();

/**************************************************************
*                        FUNCTIONS
**************************************************************/
void task1a_setup() {
  
  RCGCGPIO |= 0x1120;   // Enable PortF J N
  delay++; // Delay 2 more cycles before access Timer registers
  delay++; // Refer to Page. 756 of Datasheet for info
  
  GPIODIR_F = 0x1; // Set PF0 to output
  GPIODEN_F = 0x1; // Set PF0 to digital port
  
  GPIODIR_F |= 0x10; // Set PF4 to output
  GPIODEN_F |= 0x10; // Set PF4 to digital port
  
  GPIODIR_N = 0x1;   // Set PN0 to output 
  GPIODEN_N = 0x1;   // Set PN0 to digital port 
  
  GPIODIR_N |= 0x2;  // Set PN1 to output
  GPIODEN_N |= 0x2;  // Set PN1 to digital port

  RCGCTIMER = 0x1;              //Timer 0
  //GPTMCTL_0 & 0x0;            //disable timer A
  GPTMCFG_0 = 0x00;             //32-bit mode
  GPTMTAMR_0 |= 0x2;            //periodic timer mode
  GPTMTAMR_0 &= 0xFFEF;         //setting to TACDIR to countdown
  GPTMTAILR_0 = 0xF42400;       //16,000,000    1 sec
  GPTMICR_0 |= 0x1;             //clear flag
  GPTMCTL_0 |= 0x1;             //timer A enable
}


/**************************************************************
*                        TASK 1A
**************************************************************/
void task1a() {
  
  if (((GPTMRIS_0 & 0x1) == 1) && (GPIODATA_F == 0x1)) {
    GPIODATA_F = 0x10;
    GPTMICR_0 |= 0x1;
  } else if (((GPTMRIS_0 & 0x1) == 1) && (GPIODATA_F == 0x10)) {
    GPIODATA_F = 0x0;
    GPIODATA_N = 0x1;
    GPTMICR_0 |= 0x1;
  } else if (((GPTMRIS_0 & 0x1) == 1) && (GPIODATA_N == 0x1)) {
    GPIODATA_N = 0x2;
    GPTMICR_0 |= 0x1;
  } else if (((GPTMRIS_0 & 0x1) == 1) && (GPIODATA_N == 0x2)) {
    GPIODATA_N = 0x0;
    GPIODATA_F = 0x1;
    GPTMICR_0 |= 0x1;
  } else {
    GPIODATA_F = 0x1;
  }
  
}

/**************************************************************
*                        TASK 2 A & B
**************************************************************/
void blinkIO_init() {

  //port init
  RCGCGPIO |= 0x1120;   // Enable PortF J N
  delay++; // Delay 2 more cycles before access Timer registers
  delay++; // Refer to Page. 756 of Datasheet for info
  
  GPIODIR_F = 0x1; // Set PF0 to output
  GPIODEN_F = 0x1; // Set PF0 to digital port
  
  GPIODIR_F |= 0x10; // Set PF4 to output
  GPIODEN_F |= 0x10; // Set PF4 to digital port
  
  GPIODIR_N = 0x1;   // Set PN0 to output 
  GPIODEN_N = 0x1;   // Set PN0 to digital port 
  
  GPIODIR_N |= 0x2;  // Set PN1 to output
  GPIODEN_N |= 0x2;  // Set PN1 to digital port
  
  GPIODIR_J = 0x0;      // Set PJ0, 1 to inputs
  GPIODEN_J = 0x3;      // Set PJ0, 1 to digital port
  GPIOLOCK_J = 0x4C4F434B;     //unlock key
  GPIOCR_J = 0x3;       // Enable commit reg
  GPIOPUR_J = 0x3;      // Set pull up reg
  
  //for interrupts
  GPIOIS_J &= ~0x3;
  GPIOIBE_J &= ~0x3;
  GPIOIEV_J = ~0x3;
  GPIOICR_J |= 0x3;
  
  //enable portJ interrupts
  NVIC_EN1 |= 1<<19u;
  PORTJ_IM |= 0x3;      //Enable SW1,2 interrupt mask
  
}

void blinkTimer_init() {
  RCGCTIMER = 0x1;              //Timer 0
    //Timer0 A
  GPTMCTL_0 |= 0x0;             //Timer0A disable
  GPTMCFG_0 |= 0x0;             //32-bit mode
  GPTMTAMR_0 |= 0x2;            //periodic timer mode
  GPTMTAMR_0 &= 0xFFEF;         //setting to TACDIR to countdown
  GPTMTAILR_0 = 0xF42400;       //16,000,000 1Hz 62.5ns
  
    //enable timer for interrupt
  NVIC_EN0 |= 1<<19u; //timer0
  TIMER0_IMR = 0x01;
  
    //start timer
  GPTMCTL_0 |= 0x1;
}


void Timer0A_Handler() {
  // clear flag
  GPTMICR_0 |= 0x1;
  
  //uncomment for appropriate task
    //Task 2a
//  if ((GPIODATA_F == 0x1)) {
//    GPIODATA_F = 0x10;
//  } else if ((GPIODATA_F == 0x10)) {
//    GPIODATA_F = 0x0;
//    GPIODATA_N = 0x1;
//  } else if ((GPIODATA_N == 0x1)) {
//    GPIODATA_N = 0x2;
//  } else if ((GPIODATA_N == 0x2)) {
//    GPIODATA_N = 0x0;
//    GPIODATA_F = 0x1;
//  } else {
//    GPIODATA_F = 0x1;
//  }
  
    //Task 2b
  GPIODATA_N ^= LED1;
  
}

/**************************************************************
*                        TASK 2B
**************************************************************/
 //SW1 pressed > LED2 on (solid)
 //SW2 pressed > LED1 blinking
void PortJ_Handler() {
  
  if ((GPIOMIS_J & SW1)) {
    GPIOICR_J |= SW1;   //clear interrupt for SW1
    GPTMCTL_0 &= ~0x1;  //disable timer0A
    GPIODATA_N = LED2;
  }
  if ((GPIOMIS_J & SW2)) {
    GPIOICR_J |= SW2;   //clear interrupt for SW2
    GPTMCTL_0 |= 0x1;   //enable timer0A
    GPIODATA_N = LED1;
  }
  
  GPIOICR_J |= 0x3;
  
}

#endif