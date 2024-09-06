/* University of Washington
* ECE/CSE 474,  [11/3/2022]
*
*   [Robyn Jung 1976925]
*   [Margot Enright-Down 1776361]
*      
*   [Lab 2][Task 2c]
*   Traffic light system
*   With external led and switch control
*/

#ifndef TRAFFIC_H_
#define TRAFFIC_H_

/**************************************************************
*                   FUNCTION PROTOTYPES
**************************************************************/
  //Task 1b: traffic system with only timer
//Contains macro initializations for Task 1b
void task1b_setup();
//turns on corresponding LEDs
void RED_on();
void YELLOW_on();
void GREEN_on();
//turns off corresponding LEDs
void RED_off();
void YELLOW_off();
void GREEN_off();
//FSM for traffic light system
void task1b();

  //Task 2c: traffic system with timer and interrupts
void trafficIO_init();
void trafficTimer_init();
//Controls toggling of leds every 5 sec
void Timer1A_Handler();
//Triggered at the falling edge of btn presses
void PortL_Handler();
//Starts 2 sec timer when btn pressed
void task2c();

/**************************************************************
*                        FUNCTIONS
**************************************************************/
void task1b_setup() {
  RCGCGPIO |= 0x400;    // Enable Port L
  delay++; // Delay 2 more cycles before access Timer registers
  delay++; // Refer to Page. 756 of Datasheet for info
  
  GPIOAMSEL_L &= ~0x1F;  //disable analog function of leds and btns
  GPIOAFSEL_L &= ~0x1F;  //set leds and btns to regular port function
  GPIODIR_L = 0x1C;     //set leds to output
  //GPIODIR_L &= ~0x3;      // Set buttons to inputs
  GPIODEN_L |= 0x1F;     //enable digital output on leds and btns
  
  //Timer setups
  RCGCTIMER = 0x3;              //Timer 0 and 1
  //Timer0 A
  //GPTMCTL_0 & 0x0;            //Timer0A disable
  GPTMCFG_0 = 0x0;             //32-bit mode
  GPTMTAMR_0 |= 0x2;            //periodic timer mode
  GPTMTAMR_0 &= 0xFFEF;         //setting to TACDIR to countdown
  GPTMTAILR_0 = 0x1E84800;       //32,000,000   1sec
  GPTMICR_0 |= 0x1;             //clear flag
  GPTMCTL_0 |= 0x1;             //timer 0A enable
  //Timer1 A
  GPTMCTL_1 |= 0x0;           //timer 1A disable
  GPTMCFG_1 = 0x0;             //32-bit mode
  GPTMTAMR_1 |= 0x2;            //periodic timer mode
  GPTMTAMR_1 &= 0xFFEF;         //setting to TACDIR to countdown
  GPTMTAILR_1 = 0x4C4B400;       //80,000,000   5sec
  GPTMICR_1 |= 0x1;             //clear flag
  GPTMCTL_1 |= 0x1;             //timer 1A enable
}

void RED_on() {
  GPIODATA_L = bitsOn(GPIODATA_L, RED);
}

void YELLOW_on() {
  GPIODATA_L = bitsOn(GPIODATA_L, YELLOW);
}

void GREEN_on() {
  GPIODATA_L = bitsOn(GPIODATA_L, GREEN);
}

void RED_off() {
  GPIODATA_L = bitsOff(GPIODATA_L, RED);
}

void YELLOW_off() {
  GPIODATA_L = bitsOff(GPIODATA_L, YELLOW);
}


void GREEN_off() {
  GPIODATA_L = bitsOff(GPIODATA_L, GREEN);
}

/**************************************************************
*                        TASK 1B
**************************************************************/
void task1b() {
  
/********************State Functionality********************/
  switch(currState) {
      case reset:
        //if SYS_BUTTON pressed for 2 sec, go to stop
        if (SysFlag == 1){ // button is pressed, timer has started
          if (((GPTMRIS_0 & 0x1) == 1)&& (GPIODATA_L & SYS_BUTTON) == SYS_BUTTON) { //timed out and button still pressed
            SysFlag = 0;
            GPTMICR_0 |= 0x1; 
            GPTMICR_1 |= 0x1; 
            GPTMICR_1 |= 0x1;
            currState = stop;
            break;
          }else {
            break;
          }
        }
        //When the btn is initially pressed, reset timer.
        else if (((GPIODATA_L & SYS_BUTTON) == SYS_BUTTON)) {
          GPTMICR_0 |= 0x1;
          SysFlag = 1;
          break;
        }
        //When idle, clear flag and do nothing
        else if ((GPIODATA_L & SYS_BUTTON) == 0 ){
          SysFlag = 0;
          break;
        }
        else {
          currState = reset;
        }
        break;
    
      case stop:
        //after 5 sec, go to go
        if ((GPTMRIS_1 & 0x1) == 1){
          GPTMICR_0 |= 0x1;   //clear flag timer0A
          GPTMICR_1 |= 0x1;   //clear flag timer1A
          GPTMICR_1 |= 0x1;   //clear flag timer1A
          SysFlag = 0;
          currState = go;
          break;
        }
        
        //if SYS_BUTTON pressed for 2 sec, go to reset
        else if (SysFlag == 1){ // button is pressed, timer has started
          if (((GPTMRIS_0 & 0x1) == 1) && (GPIODATA_L & SYS_BUTTON) == SYS_BUTTON) { //timed out and button still pressed
            SysFlag = 0;
            GPTMICR_0 |= 0x1; 
            GPTMICR_1 |= 0x1;
            GPTMICR_1 |= 0x1;
            currState = reset;
            break;
          }else {
            break;
          }
        }
        //When the btn is initially pressed, reset timer.
        else if (((GPIODATA_L & SYS_BUTTON) == SYS_BUTTON)) {
          GPTMICR_0 |= 0x1;
          SysFlag = 1;
          break;
        }
        //When idle, clear flag and do nothing
        else if ((GPIODATA_L & SYS_BUTTON) == 0 ){
          SysFlag = 0;
          break;
        }
        else {
          currState = stop;
        }
        break;
    
      case go:
        //if SYS_BUTTON pressed for 2 sec, go to reset
        if (SysFlag == 1){ // button is pressed, timer has started
          if (((GPTMRIS_0 & 0x1) == 1) && (GPIODATA_L & SYS_BUTTON) == SYS_BUTTON) { //timed out and button still pressed
            SysFlag = 0;
            GPTMICR_0 |= 0x1; 
            GPTMICR_1 |= 0x1;
            currState = reset;
            break;
          }else {
            break;
          }
        }
        //When the btn is initially pressed, reset timer.
        else if (((GPIODATA_L & SYS_BUTTON) == SYS_BUTTON)) {
          GPTMICR_0 |= 0x1;
          SysFlag = 1;
          break;
        }
        //if PED_BUTTON pressed for 2 sec, go to warn
        else if (PedFlag == 1){ // ped is pressed, timer has started
          if (((GPTMRIS_0 & 0x1) == 1) && (GPIODATA_L & PED_BUTTON) == PED_BUTTON) { //timed out and button still pressed
            PedFlag = 0;
            GPTMICR_0 |= 0x1; 
            GPTMICR_1 |= 0x1;
            currState = warn;
            break;
          }else {
            break;
          }
        }
        //When the PED btn is initially pressed, reset timer.
        else if ((GPIODATA_L & PED_BUTTON) == PED_BUTTON) {
          GPTMICR_0 |= 0x1;
          PedFlag = 1;
          break;
        }
        
        //after 5 sec, go to stop
        else if ((GPTMRIS_1 & 0x1) == 1){
          GPTMICR_0 |= 0x1;   //clear flag timer1A
          GPTMICR_1 |= 0x1;   //clear flag timer1A
          SysFlag = 0;
          currState = stop;
          break;
        }
        else {
          currState = go;
        }
        break;
    
      case warn:
        //if SYS_BUTTON pressed for 2 sec, go to reset
        if (SysFlag == 1){ // button is pressed, timer has started
          if (((GPTMRIS_0 & 0x1) == 1) && (GPIODATA_L & SYS_BUTTON) == SYS_BUTTON) {
            SysFlag = 0;
            GPTMICR_0 |= 0x1; 
            GPTMICR_1 |= 0x1;
            currState = reset;
            break;
          }else {
            break;
          }
        }
        //initial btn press
        else if (((GPIODATA_L & SYS_BUTTON) == SYS_BUTTON)) {
          GPTMICR_0 |= 0x1;
          SysFlag = 1;
          break;
        }
        
        //after 5 sec, go to stop
        else if((GPTMRIS_1 & 0x1) == 1){
          GPTMICR_0 |= 0x1;   //clear flag timer0A
          GPTMICR_1 |= 0x1;   //clear flag timer1A
          currState = stop;
          break;
        } else {
          currState = warn;
        }
        break;
    
      default:
        currState = reset;
        break;
  }
  
  
  
  
/********************State Behaviors********************/
  switch(currState) {
  case reset:
    RED_off();
    YELLOW_off();
    GREEN_off();
    break;
    
  case stop:
    RED_on();
    YELLOW_off();
    GREEN_off();
    break;
    
  case go:
    GREEN_on();
    RED_off();
    YELLOW_off();
    break;
    
  case warn:
    YELLOW_on();
    RED_off();
    GREEN_off();
    break;
    
  default:
    break;
  }
}

/**************************************************************
*                        TASK 2C
**************************************************************/

void trafficIO_init() {
  RCGCGPIO |= 0x400;    // Enable Port L
  delay++; // Delay 2 more cycles before access Timer registers
  delay++; // Refer to Page. 756 of Datasheet for info
  
  GPIOAMSEL_L &= ~0x1F;  //disable analog function of leds and btns
  GPIOAFSEL_L &= ~0x1F;  //set leds and btns to regular port function
  GPIODIR_L = 0x1C;     //set leds to output
  //GPIODIR_L &= ~0x3;      // Set buttons to inputs
  GPIODEN_L |= 0x1F;     //enable digital output on leds and btns
  
  //setup for interrupts
  GPIOIS_L &= ~0x3;
  GPIOIBE_L &= ~0x3;
  GPIOIEV_L = ~0x3;     //btns are FALLING EDGE triggered
  GPIOICR_L |= 0x3;
  
  //enable port L interrupts
  NVIC_EN1 |= 1<<21u;
  PORTL_IM |= 0x3;
}

void trafficTimer_init() {
  RCGCTIMER = 0x3;              //Timer 0 and 1
  
  //Timer0 A
  GPTMCTL_0 = 0x0;            //Timer0A disable
  GPTMCFG_0 = 0x0;             //32-bit mode
  GPTMTAMR_0 |= 0x2;            //periodic timer mode
  GPTMTAMR_0 &= 0xFFEF;         //setting to TACDIR to countdown
  GPTMTAILR_0 = 0xF42400;       //16,000,000   1sec
  GPTMICR_0 |= 0x1;             //clear flag
  //Timer1 A
  GPTMCTL_1 = 0x0;           //timer 1A disable
  GPTMCFG_1 = 0x0;             //32-bit mode
  GPTMTAMR_1 |= 0x2;            //periodic timer mode
  GPTMTAMR_1 &= 0xFFEF;         //setting to TACDIR to countdown
  GPTMTAILR_1 = 0x4C4B400;       //80,000,000   5sec
  GPTMICR_1 |= 0x1;             //clear flag
  
  //enable timer interrupt
  NVIC_EN0 |= 1<<21u;   //only using interrupt with timer1A
  TIMER1_IMR |= 0x01;
  
  //start timer
  GPTMCTL_1 |= 0x1;             //timer 1A enable
}

void Timer1A_Handler() {
  GPTMICR_1 |= 0x1;
  
  if ((GPIODATA_L & RED) == RED) {
    GREEN_on();
    RED_off();
    YELLOW_off();
  }
  else if ((GPIODATA_L & GREEN) == GREEN) {
    RED_on();
    YELLOW_off();
    GREEN_off();
  }
  else if ((GPIODATA_L & YELLOW) == YELLOW) {
    RED_on();
    YELLOW_off();
    GREEN_off();
  }
  else {
    RED_off();
    YELLOW_off();
    GREEN_off();
  }
}

void PortL_Handler() {
  
  //If SYS_BUTTON is pressed then released after 2 sec
  if ((GPIOMIS_L & SYS_BUTTON) && (GPTMRIS_0 & 0x1)) {
    //if any of the light is on, turn off all
    if ((GPIODATA_L & 0x01C) > 0x3) {
      RED_off();
      YELLOW_off();
      GREEN_off();
    }
    //else if none of the lights are on, turn on red
    else {
      RED_on();
      YELLOW_off();
      GREEN_off();
    }
  }
  
  //If PED_BUTTON is pressed then released after 2 sec
  else if ((GPIOMIS_L & PED_BUTTON) && (GPTMRIS_0 & 0x1)) {
    //if we are in go state, go to warn
    if ((GPIODATA_L & GREEN) == GREEN) {
      YELLOW_on();
      RED_off();
      GREEN_off();
    } //else ignore btn press
  }

  //clear flags
  GPIOICR_L |= 0x3;
  GPTMICR_0 |= 0x1;
  GPTMICR_1 |= 0x1;
  GPTMCTL_0 &= ~0x1;  //disable timer0A
}

void task2c() {
  //if btn is pressed, start timer0A
  if (((GPIODATA_L & SYS_BUTTON) == SYS_BUTTON) || ((GPIODATA_L & PED_BUTTON) == PED_BUTTON)) {
    GPTMCTL_0 |= 0x1;
  } else {
    GPTMCTL_0 &= ~0x1;
  }
}


#endif