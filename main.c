/*
 * File:   main.cpp
 * Author: utida
 *
 * Created on 2022/07/19, 16:53
 */

#include <xc.h>
#include<pic16f690.h>
#include "synchronize.h"
#pragma config WDTE=OFF
#pragma config MCLRE = ON


void main(void) {
    
    OSCCONbits.IRCF=0b110;//internal oscillator 4MH operating clock 1MH
    OSCCONbits.SCS=0b11;//use internal oscillator
    INTCONbits.GIE=1;
    INTCONbits.PEIE=1;//interrupt setting
    
    ANSEL=0x12;//RA4(AN3) RA2(AN2 analog input
    ANSELH=0x00;
    TRISA=0b00010100;//0x05;//RA4 RA2 input
    TRISB=0x50;//i2c:RB4,RB6 
    TRISC=0x00;
    synchro_init();
    
    //i2c(debug)settings--------------------------
    SSPSTAT=0x00;//400kHz
    SSPCON=0b00110110;//i2c slave mode 7-bit adress
    SSPADD=0x01;
    
    //adconverter settings------------------------
    ADCON0=0x00;//initalize adconverter
    ADCON1=0x00;//fosc/2  
    
    //clock led interrupt setting----------------- 
    //PIE1=1;
    T1CON=0b00001001;//prescarlar1:1 period 1us
    TMR1H=0xD8;//init timer count 55536
    TMR1L=0xF0;
    //timer period 1ms
    TMR1IF=0;//timer1 flag 0
    TMR1IE=1;//enable timer1 intrrupt
    //TMR1ON=1;//start timer1 interrupt
   
    while(1){
       RB5=0;
       sensor_read(); 
        
    }
   
   
}
