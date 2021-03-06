// BumpInt.c
// Runs on MSP432, interrupt version
// Provide low-level functions that interface bump switches the robot.
// Daniel Valvano and Jonathan Valvano
// July 2, 2017

/* This example accompanies the books
   "Embedded Systems: Introduction to the MSP432 Microcontroller",
       ISBN: 978-1512185676, Jonathan Valvano, copyright (c) 2017
   "Embedded Systems: Real-Time Interfacing to the MSP432 Microcontroller",
       ISBN: 978-1514676585, Jonathan Valvano, copyright (c) 2017
   "Embedded Systems: Real-Time Operating Systems for ARM Cortex-M Microcontrollers",
       ISBN: 978-1466468863, , Jonathan Valvano, copyright (c) 2017
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/

Simplified BSD License (FreeBSD License)
Copyright (c) 2017, Jonathan Valvano, All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are
those of the authors and should not be interpreted as representing official
policies, either expressed or implied, of the FreeBSD Project.
*/

// Negative logic bump sensors
// P4.7 Bump5, left side of robot
// P4.6 Bump4
// P4.5 Bump3
// P4.3 Bump2
// P4.2 Bump1
// P4.0 Bump0, right side of robot

#include <stdint.h>
#include "msp.h"

void (*Port4Task)(uint8_t);   // user function

// Initialize Bump sensors
// Make six Port 4 pins inputs
// Activate interface pullup
// pins 7,6,5,3,2,0
// Interrupt on falling edge (on touch)
void BumpInt_Init(void(*task)(uint8_t)){
    // write this as part of Lab 3 Challenge

    Port4Task = task;

    P4->SEL0 &= ~0xED;
    P4->SEL1 &= ~0xED;
    P4->DIR  &= ~0xED;
    P4->REN  |= 0xED;
    P4->OUT  |= 0xED;

    P4->IES  |= 0xED;
    P4->IFG  &= ~0xED;
    P4->IE   |= 0xED;

    NVIC->IP[9] = (NVIC->IP[9]&0xFF00FFFF)|0x400000; // priority 2
    NVIC->ISER[1] = 0x00000040;

}
// Read current state of 6 switches
// Returns a 6-bit positive logic result (0 to 63)
// bit 5 Bump5
// bit 4 Bump4
// bit 3 Bump3
// bit 2 Bump2
// bit 1 Bump1
// bit 0 Bump0
uint8_t Bump_Read(void){
    // write this as part of Lab 3 Challenge

    uint8_t result = 0x0;
    uint8_t temp = P4->IN;

    uint8_t result1, result2, result3;

    result1 = (P4->IN)&0x01;
    result2 = (P4->IN)&0x0C;
    result2 = result2 >> 1;
    result3 = (P4->IN)&0xE0;
    result3 = result3 >> 2;

    result = result1 + result2 + result3;

//    result += (temp & 0x01);
//    result += 2*(temp & 0x04);
//    result += 4*(temp & 0x08);
//    result += 8*(temp & 0x20);
//    result += 16*(temp & 0x40);
//    result += 32*(temp & 0x80);

    return (result);
}
// we do not care about critical section/race conditions
// triggered on touch, falling edge
void PORT4_IRQHandler(void){
    // write this as part of Lab 3 Challenge
//    uint8_t status;
//    P1->OUT ^= 0x01;                 // toggle LED1
//    P1->OUT ^= 0x01;                 // toggle LED1
//    status = P4->IV;  // 2*(n+1) where n is highest priority
//    // if P1.1, returns 0x04 and clears flag1. Pg695 slau356. //OHL
//    // if P1.4, returns 0x0A and clears flag4
//    if(status == 0x04){
//    P2->OUT ^= 0x01;                 // toggle Red RGB LED
//    status = P4->IV;
//    }
//    if(status == 0x0A){
//    P2->OUT ^= 0x04;                // toggle blue RGB LED
//    }
//    P1->OUT ^= 0x01;                 // toggle LED1

    (*Port4Task)(Bump_Read());
}
//


//// Negative logic bump sensors
//// P4.7 Bump5, left side of robot
//// P4.6 Bump4
//// P4.5 Bump3
//// P4.3 Bump2
//// P4.2 Bump1
//// P4.0 Bump0, right side of robot
//
//#include <stdint.h>
//#include "msp.h"
//
//void (*Port4Task)(uint8_t);   // user function
//
//// Initialize Bump sensors
//// Make six Port 4 pins inputs
//// Activate interface pullup
//// pins 7,6,5,3,2,0
//// Interrupt on falling edge (on touch)
//void BumpInt_Init(void(*task)(uint8_t)){
//    // write this as part of Lab 3 Challenge
//    Port4Task = task;  //assign user task to global function pointer
//
//    P4->SEL0 &= ~0xED;
//    P4->SEL1 &= ~0xED; //init GPIO
//    P4->DIR &= ~0xED; //init the 6 above pins as input
//    P4->REN |= 0xED; //enable pull up/down
//    P4->OUT |= 0xED;//pull up                  // pull-up
//    P4->IES |= 0xED;                   // falling edge event (negative logic)
//    P4->IFG &= ~0xED;                  // clear 0 - 7 (reduce possibility of extra interrupt)
//    P4->IE |= 0xED;                    // arm interrupt on the above pin
//    NVIC->IP[9] = (NVIC->IP[9] & 0xFF00FFFF)|0x00400000; // priority 2
//    NVIC->ISER[1] = 0x00000040;        // enable interrupt 38 in NVIC (I/o port 4 interrupt === IRQ 38)
//
//
//}
//// Read current state of 6 switches
//// Returns a 6-bit positive logic result (0 to 63)
//// bit 5 Bump5
//// bit 4 Bump4
//// bit 3 Bump3
//// bit 2 Bump2
//// bit 1 Bump1
//// bit 0 Bump0
//uint8_t Bump_Read(void){
//    // write this as part of Lab 3 Challenge
//    uint8_t result3, result2, result1;
//    uint8_t result = 0X00;
//    result1 = (P4->IN)&0X01; //bump 0
//    result2 = (P4->IN)&0X0C; //bump 1 and 2
//    result2 = result2 >> 1; //remove bit 1
//    result3 = (P4->IN)&0XE0; //bump 5, 4, 3
//    result3 = result3 >> 2; //remove bit 4
//
//    result = result1 + result2 + result3; //combine all the bumps together
//        // 0000 0001 + 0000 0110 + 0011 1000 = 0011 1111
//
//    return (result);
//}
//// we do not care about critical section/race conditions
//// triggered on touch, falling edge
//void PORT4_IRQHandler(void){
//    // write this as part of Lab 3 Challenge
//    (*Port4Task)(Bump_Read()); //execute user's tasks, who's input is the Sensor reading
//}
