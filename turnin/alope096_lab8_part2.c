/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 * Demo Link: https://drive.google.com/drive/folders/1CgwR2gvNUYymDPUC98qPOQr6-UoJOLAX?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void set_PWM(double frequency){
    static double current_frequency;
    
    if(frequency != current_frequency){
        if(!frequency){
            TCCR3B &= 0x08;
        }
        else{
            TCCR3B |= 0x03;
        }

        if(frequency < 0.954){
            OCR3A = 0xFFFF;
        }
        else if(frequency > 31250){
            OCR3A = 0x0000;
        }
        else{
            OCR3A = (short)(8000000 / (128 * frequency)) -1;
        }
        TCNT3 = 0;
        current_frequency = frequency;
    } 
}

void PWM_on(){
    TCCR3A = (1 << COM3A0);
    TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
    set_PWM(0);
}
void PWM_off(){
    TCCR3A = 0x00;
    TCCR3B = 0x00;
}

double Notes[8] ={261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
enum States{stateOFF,stateOFF_press,stateON,stateON_press,Play,increment_press,increment,decrement_press,decrement}state;


unsigned char noteNum;

void Tick(){
 
unsigned char button = ~PINA & 0x07;
// 0x01 is on/off 0x04 is decrement 0x02 is increment

    switch(state){
        case stateOFF:
            if (button == 0x01){
                state = stateON_press;
            }
            else{
                state = stateOFF;
            }
        break;
        case stateON_press:
            if (button == 0x01){
                state = stateON_press;
            }
            else{
                state = stateON;
            }
        break;
        case stateON:
            state = Play;
        break;
        case stateOFF_press:
            if (button == 0x01){
                state = stateOFF_press;
            }
            else{
                state = stateOFF;
            }
        break;

        case Play:
           if((button == 0x01)){
             state = stateOFF_press;
           }
           else if((button == 0x02)){
             state = increment_press;
           }
           else if((button == 0x04)){
             state = decrement_press;
           }
           else{
             state = Play;
           }
           
        break;

        case increment_press:
	   if((button == 0x02)){
             state = increment_press;
           }
           else {
             state = increment;
           }
           
        break;

        case increment:
            state = Play;
        break;

         case decrement_press:
	   if((button == 0x04)){
             state = decrement_press;
           }
           else {
             state = decrement;
           }
           
        break;

        case decrement:
            state = Play;
        break;

        /*default:
           state = stateOFF;
        break;*/
    }
    switch(state){
        case stateOFF:
           PWM_off();
           noteNum = 0;
        break;

        case stateOFF_press:
        break;
        case stateON:
        break;
        case stateON_press:
           PWM_on();
           noteNum = 0;
        break;
        case Play:
           set_PWM(Notes[noteNum]);
        break;
        case increment_press:
        break;
        break;
        case increment:
            if(noteNum <7){
                noteNum++;
            }
        break;
        case decrement_press:
        break;
        case decrement:
            if(noteNum >0){
                noteNum--;
            }
        break;
    }
}


int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    PWM_on();
    
    state = stateOFF;
    /* Insert your solution below */
    while (1) {
        Tick();
    }
    return 1;
}
