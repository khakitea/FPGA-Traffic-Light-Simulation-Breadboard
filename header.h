/* University of Washington
 * ECE/CSE 474,  [11/3/2022]
 *
 *   [Robyn Jung 1976925]
 *   [Margot Enright-Down 1776361]
 *      
 *   [Lab 2]
 *   [Header File]
 */

#ifndef __HEADER_H__
#define __HEADER_H__

/**************************************************************
 *              SHARED ACROSS THE PROJECT
 **************************************************************/
volatile unsigned short delay = 0;
#define RCGCGPIO (*((volatile uint32_t *)0x400FE608))
#define RCGCTIMER (*((volatile uint32_t *)0x400FE604)) //p380

/**************************************************************
 *                  REGISTER DEFINITIONS
 **************************************************************/
 // Timer Config
   //Timer 0A
#define GPTMCTL_0 (*((volatile uint32_t *)0x4003000C)) //p986 0 is disable for timer0 A
#define GPTMCFG_0 (*((volatile uint32_t *)0x40030000)) //p976
#define GPTMTAMR_0 (*((volatile uint32_t *)0x40030004)) //p977
#define GPTMTAILR_0 (*((volatile uint32_t *)0x40030028)) //1004
#define GPTMRIS_0 (*((volatile uint32_t *)0x4003001C)) //p996 TATORIS bit is the last bit
#define GPTMICR_0 (*((volatile uint32_t *)0x40030024)) //p1002 TATOCINT is the last bit
   //Timer 1A
#define GPTMCTL_1 (*((volatile uint32_t *)0x4003100C))
#define GPTMCFG_1 (*((volatile uint32_t *)0x40031000))
#define GPTMTAMR_1 (*((volatile uint32_t *)0x40031004))
#define GPTMTAILR_1 (*((volatile uint32_t *)0x40031028))
#define GPTMRIS_1 (*((volatile uint32_t *)0x4003101C))
#define GPTMICR_1 (*((volatile uint32_t *)0x40031024))
   
 // Interrupt Config
#define TIMER0_IMR (*((volatile uint32_t *)0x40030018)) //GPTMIMR (GPTM controller-level interrupts enabler)
#define TIMER1_IMR (*((volatile uint32_t *)0x40031018))
#define NVIC_EN0 (*((volatile uint32_t *)0xE000E100))   //Interrupt enable      Timer0A(19) Timer1A(21)
#define NVIC_EN1 (*((volatile uint32_t *)0xE000E104))   //                      Port J(51) Port L(53)
#define PORTJ_IM (*((volatile uint32_t *)0x40060410))   //Port Interrupt Mask (triggered interrupt can be sent to interrupt controller)
#define PORTL_IM (*((volatile uint32_t *)0x40062410))
#define PRI12 (*((volatile uint32_t *)0xE000E430))      //for interrupt 51 priority
#define PRI13 (*((volatile uint32_t *)0xE000E434))      //for interrupt 53 priority
   
 // Onboard LEDs
   //Port F for LED 3 and 4
#define GPIODIR_F (*((volatile uint32_t *)0x4005D400))
#define GPIODEN_F (*((volatile uint32_t *)0x4005D51C))
#define GPIODATA_F (*((volatile uint32_t *)0x4005D3FC))

   //Port N for LED 1 and 2
#define GPIODIR_N (*((volatile uint32_t *)0x40064400))
#define GPIODEN_N (*((volatile uint32_t *)0x4006451C))
#define GPIODATA_N (*((volatile uint32_t *)0x400643FC))
   
   //Port J for SW 1 and 2
#define GPIODIR_J (*((volatile uint32_t *)0x40060400))
#define GPIODEN_J (*((volatile uint32_t *)0x4006051C))
#define GPIODATA_J (*((volatile uint32_t *)0x400603FC)) 
#define GPIOLOCK_J (*((volatile uint32_t *)0x40060520))         //enables editing GPIOCR
#define GPIOCR_J (*((volatile uint32_t *)0x40060524))           //determines the bits to CR
#define GPIOPUR_J (*((volatile uint32_t *)0x40060510))          //enables pull-up resister
#define GPIOIS_J (*((volatile uint32_t *)0x40060404))           //edge sensitive on
#define GPIOIBE_J (*((volatile uint32_t *)0x40060408))          //trigger is controlled by IEV
#define GPIOIEV_J (*((volatile uint32_t *)0x4006040C))          //falling edge trigger
#define GPIOICR_J (*((volatile uint32_t *) 0x4006041C))         //Clear interrupt
#define GPIOMIS_J (*((volatile uint32_t *) 0x40060418)) //Masked Interrupt Status (checks if the interrupt has triggered)
   
   //Traffic Light System
#define GPIOAMSEL_L (*((volatile uint32_t *)0x40062528))        //analog mode enable
#define GPIODIR_L (*((volatile uint32_t *)0x40062400))
#define GPIODEN_L (*((volatile uint32_t *)0x4006251C))
#define GPIOAFSEL_L (*((volatile uint32_t *)0x40062420))        //port control mode (GPIO or alt hardware)
#define GPIODATA_L (*((volatile uint32_t *)0x400623FC))
#define GPIOLOCK_L (*((volatile uint32_t *)0x40062520))         //enables editing GPIOCR
#define GPIOCR_L (*((volatile uint32_t *)0x40062524))           //determines the bits to CR
#define GPIOPUR_L (*((volatile uint32_t *)0x40062510))          //enables pull-up resister
#define GPIOIS_L (*((volatile uint32_t *)0x40062404))           //761
#define GPIOIBE_L (*((volatile uint32_t *)0x40062408))          //762
#define GPIOIEV_L (*((volatile uint32_t *)0x4006240C))          //763
#define GPIOICR_L (*((volatile uint32_t *) 0x4006241C))         //769
#define GPIOMIS_L (*((volatile uint32_t *) 0x40062418))         //767
   
 /**************************************************************
 *                     ADDITIONAL MACROS
 **************************************************************/
#define LED1 0x2                //PN1
#define LED2 0x1                //PN0
#define LED3 0x10               //PF4
#define LED4 0x1                //PF0
#define SW1  0x1                //PJ0
#define SW2  0x2                //PJ1
   
#define RED 0x4                 //PL2
#define YELLOW 0x8              //PL3
#define GREEN 0x10              //PL4  leds: 1.1100 = 0x1C
#define SYS_BUTTON 0x1          //PL0
#define PED_BUTTON 0x2          //PL1  buttons: 0x3
   
 /**************************************************************
 *                   VARIABLE DECLARATIONS
 **************************************************************/
int SysFlag = 0;        //checks initial press on the SYS_BUTTON
int PedFlag = 0;        //checks initial press on the PED_BUTTON
   
typedef enum lightStates {reset, stop, warn, go}lightState;
typedef enum lightEvents {all_off, red_on, yellow_on, green_on}lightEvent;
lightState currState = reset;

/**************************************************************
 *                   FUNCTION PROTOTYPES
 **************************************************************/
//Turns the GPIODATA on with a mask value
int bitsOn(int portData, int mask);
//Turns the GPIODATA off with a mask value
int bitsOff(int portData, int mask);
//checks if the btn is pressed
int btn_pressed(int GPIODATA, int btn);

/**************************************************************
 *                      FUNCTIONS
 **************************************************************/
int bitsOn(int portData, int mask) {
     return portData |= mask;
}

int bitsOff(int portData, int mask) {
     return portData &= ~mask;
}

int btn_pressed(int GPIODATA, int btn) {
  return ((GPIODATA & btn) == btn);
}

//Ignore please
//Below was an idea to make a timer that records the total elapsed time.
//Pro: computation is all mathematical and easier to think through
//Con: There is a limit to program run time (max val of the variable).
////Timer 0 A setup
//// 1 sec timer
//void timer0B_Init() {
//  RCGCTIMER = 0x1;              //Timer 0
//  GPTMCTL_0 & 0x001;            //Make sure timer B is off when A is still on
//  GPTMCFG_0 = 0x00;             //32-bit mode
//  GPTMTAMR_0 |= 0x2;            //periodic timer mode
//  GPTMTAMR_0 &= 0xFFEF;         //setting to TACDIR to countdown
//  GPTMTAILR_0 = 0xF42400;       //16,000,000
//  GPTMICR_0 |= 0x1;             //clear flag
//  GPTMCTL_0 |= 0x1;             //timer A enable
//}
//
////Keep track of total Elapsed Time
////Useful in the future? computation easier
//void countTime() {
//  if ((GPTMRIS_0 & 0x1) == 1) {
//    totalTime++;
//    //clear flag
//  }
//}


#endif //__HEADER_H__