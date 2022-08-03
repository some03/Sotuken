/*
 * File:   main.cpp
 * Author: utida
 *
 * Created on 2022/07/19, 16:53
 */

#include <xc.h>
#include<pic16f690.h>
#include<stdint.h>
#include "synchronize.h"

void main(void) {
    
    OSCCONbits.IRCF=0b011;//internal oscillator 1MH operating clock 250kH
    OSCCONbits.SCS=0b11;//use internal oscillator
    INTCONbits.GIE=1;
    INTCONbits.PEIE=1;//interrupt setting
    
    ANSEL=0x05;//RA0(AN0) RA2(AN2 analog input
    ANSELH=0x00;
    TRISA=0x05;//RA0 RA2 input
    TRISB=0x00;
    TRISC=0x00;
    synchro_init();
    
    //adconverter settings------------------------
    ADCON0=0x00;//initalize adconverter
    ADCON1=0x00;//fosc/2  
    
    //clock led interrupt setting----------------- 
    PIE1=1;
    T1CON=0b01101000;//prescarlar1:4 period 16us
    TMR1H=0x0b;//init timer count 3036
    TMR1L=0xdc;
    //timer period 1ms
    TMR1IF=0;//timer1 flag 0
    TMR1IE=1;//enable timer1 intrrupt
    TMR1ON=1;//start timer1 interrupt
   
    while(1){
       sensor_read(); 
    }
    return;
}
