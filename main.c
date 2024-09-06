/* University of Washington
 * ECE/CSE 474,  [11/3/2022]
 *
 *   [Robyn Jung 1976925]
 *   [Margot Enright-Down 1776361]
 *      
 *   [Lab 2]
 *   Using Timers and Interrupts
 */

#include <stdint.h>
#include "header.h"
#include "blink.h"
#include "traffic.h"

int main(void) { 
  
  //uncomment to run
  //task1a_setup();
  //task1b_setup();
  
  //uncomment below 2 lines to run Task 2a and b
  //there are some lines to uncomment internally in TimeHandler function.
//  blinkIO_init();
//  blinkTimer_init();
  
  //uncomment below lines to run Task 2c
  trafficIO_init();
  trafficTimer_init();
  
  while (1) {
    
    //uncoment to run
    //task1a();
    //task1b();
    task2c();
    
  }
  
  return 0;
}