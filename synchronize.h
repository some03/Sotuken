/* 
 * File:   sysnchronize.h
 * Author: utida
 *
 * Created on 2022/07/21, 15:43
 */

#ifndef SYSNCHRONIZE_H
#define	SYSNCHRONIZE_H

#define _XTAL_FREQ 1000000

int y,max_y;//output voltage 
int a,b;//constant
int x;//input voltage 
int c_value,d_value,max_value=50;//ADcon value and threshold
int circle_cnt,circle_time;
int point,old_point;//data position
int8_t rc,receive_databuf,transmit_databuf;
int8_t receive_data,transmit_data;
float m,u;//coefficient
float T;//interrupt period(s)
float t;//time constant(s) 


void synchro_init(){
    T=0.001;
    max_y=10;
    t=0.1;
    a=(T/t)/(1+(T/t));
    b=1/(1+(T/t));
    u=1;
}

void sensor_read(){
    
    ADCON0=0b10000011;// right justfied AN0(RA0) adconverter on 
    __delay_us(10);
    while(ADCON0bits.GO){
        c_value=ADRESH|(ADRESL>>6);
        m+=(c_value>=max_value)?0.1:0;
    }
    ADCON0=0b10001011;//right justified AN2(RA2)adconverter on
        __delay_us(10);
        while(ADCON0bits.GO){
            d_value=ADRESH|(ADRESL>>6);
           rc=(d_value>=max_value)?1:0;
        }
}

void __interrupt()slave_interrupt(){
    GIE=0;//restrict other interrupt
    if(TMR1IF==1){
        
        TMR1H=0x0b;//intialize timer1
        TMR1L=0xdc;
        TMR1ON=0;//stop timer1
        
        circle_cnt++; 
        y=a*x+b*y+m*u;//calcrate voltage
        
        if(y>=max_y){
            RA3=1;//clock led on    
            y=0;
            circle_time=circle_cnt;
            circle_cnt=0;
        }
        else{
            RA3=0;//clock led off
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

        TMR1IF=0;
        TMR1ON=1;
    }
    GIE=1;//permit other interrupt
}


#endif	/* NEWFILE_H */

