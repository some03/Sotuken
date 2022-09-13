/* 
 * File:   sysnchronize.h
 * Author: utida
 *
 * Created on 2022/07/21, 15:43
 */

#include<stdio.h>
#ifndef SYSNCHRONIZE_H
#define	SYSNCHRONIZE_H

#define _XTAL_FREQ 1000000
#define slave
//#define master

int8_t max_y;//output voltage 
int8_t a,b;//constant
int8_t x;//input voltage 
int8_t c_value,d_value,max_value=50;//ADcon value and threshold
int8_t circle_cnt,circle_time;
int8_t point,old_point;//data position
int8_t rc,receive_databuf,transmit_databuf;
int8_t receive_data=NULL,transmit_data=20;
int8_t m,u;//coefficient
float  T,t,y;//interrupt period(s)
//time constant(s) 


void synchro_init(){
    x=10;
    T=0.001;
    max_y=1;
    t=0.1;
    a=(T/t)/(1+(T/t));
    b=1/(1+(T/t));
    u=1;
    m=0;
}

void sensor_read(){
    
    ADCON0=0b10001111;// right justfied AN3(RA4) adconverter on 
    __delay_us(10);
    while(ADCON0bits.GO){
        c_value=ADRESH|(ADRESL>>6);
        m+=(c_value>=max_value)?0.1:0;
        RB5=1;
    }
    ADCON0=0b10001011;//right justified AN2(RA2)adconverter on
        __delay_us(10);
        while(ADCON0bits.GO){
            d_value=ADRESH|(ADRESL>>6);
           rc=(d_value>=max_value)?1:0;
           RB5=1;
        }
}
#ifdef slave
void __interrupt()slave_interrupt(void){
    /*
    if(TMR1IF){
        TMR1IF=0;
        RB4=1;
    }*/
    //GIE=0;//restrict other interrupt
    //RB5=1;
    if(SSPIF==1){
        /*while(SSPSTATbits.BF)*/
       
        //RB5=1;
        if(SSPCONbits.SSPOV)SSPCONbits.SSPOV=0;
        //if(SSPSTAT==0b00001100)RB5=1;
        //if(SSPCONbits.SSPEN)
        SSPBUF=20;
        SSPIF=0;
        SSPCONbits.CKP=1;
    }
    
    else if(TMR1IF==1){
        //RB4=1;
        TMR1H=0x0b;//intialize timer1
        TMR1L=0xdc;
        TMR1ON=0;//stop timer1
        
        circle_cnt++; 
        y+=0.5;//a*x+b*y+m*u;//calcrate voltage;
        
        if(y>=max_y){
            RB5=1;//clock led on    
            y=0;
            circle_time=circle_cnt;
            circle_cnt=0;
        }
        else{
            //RB4=0;//clock led off
        }
        m=0;
        old_point=point;
        point=circle_cnt*4/circle_time;
        if(point!=old_point){
            receive_databuf=(receive_databuf<<1)|rc;
        }
        if(point==0){
            receive_data=receive_databuf; 
            receive_databuf=0;
        }
        transmit_data=receive_data;
        TMR1IF=0;
        TMR1ON=1;
    }
         
    
    //SSPIF=0;
    //GIE=1;//permit other interrupt

    
}
#endif
#ifdef master

void __interrupt()master_intrerrupt(void){
    GIE=0;
    if(TMR1IF==1){
        TMR1H=0x0b;
        TMR1L=0xdc;
        TMR1ON=0;
        
        circle_cnt++;
        y+=0.5;
        
        if(y>=max_y){
            RB4=1;
            y=0;
            circle_time=circle_cnt;
            circle_cnt=0;
            }
        else{
            RB4=0;
        }
        m=0;
        old_point=point;
        point=circle_cnt*4/circle_time;
        
        if(point!=old_point){
            
            RB5=transmit_databuf&0b1;
            transmit_databuf=transmit_databuf>>1;
        
        }
        
        TMR1IF=0;
        TMR1ON=1;
    }
    GIE=1;
  
}

#endif

#endif	/* NEWFILE_H */

