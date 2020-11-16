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

enum States{Start, playC, playD, playE, silent}state;

void Tick(){

    unsigned char button = ~PINA & 0x07;
    switch(state){
        case Start:
            if(button == 0x01){
                state = playC;
            }
            else if(button == 0x02){
                state = playD;
            }
            else if(button == 0x04){
                state = playE;
            }
            else if((button == 0x03) || (button == 0x05) || (button == 0x07)){
                state = silent;
            }
            else{
                state = Start;
            }
        break;
        case playC:
            if(button == 0x01){
                state = playC;
            }
            else if(button == 0x02){
                state = playD;
            }
            else if(button == 0x04){
                state = playE;
            }
            else if((button == 0x03) || (button == 0x05) || (button == 0x07)){
                state = silent;
            }
            else{
                state = Start;
            }

        break;
        case playD:
            if(button == 0x01){
                state = playC;
            }
            else if(button == 0x02){
                state = playD;
            }
            else if(button == 0x04){
                state = playE;
            }
            else if((button == 0x03) || (button == 0x05) || (button == 0x07)){
                state = silent;
            }
            else{
                state = Start;
            }

        break;
        case playE:
            if(button == 0x01){
                state = playC;
            }
            else if(button == 0x02){
                state = playD;
            }
            else if(button == 0x04){
                state = playE;
            }
            else if((button == 0x03) || (button == 0x05) || (button == 0x07)){
                state = silent;
            }
            else{
                state = Start;
            }

        break;
        case silent:
            state = Start;
        break;
        default:
            state = Start;
        break;
    }
    switch(state){
        case Start:
            set_PWM(0);
        break;
        case playC:
            set_PWM(261.63);
        break;
        case playD:
            set_PWM(293.66);
        break;
        case playE:
            set_PWM(329.63);
        break;
        case silent:
            set_PWM(0);
        break;

    }
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    PWM_on();
    
    state = Start;
    /* Insert your solution below */
    while (1) {
        Tick();
    }
    return 1;
}
