/* 
 * File:   sysnchronize.h
 * Author: utida
 *
 * Created on 2022/07/21, 15:43
 */

#include<stdio.h>
#include<stdbool.h>
#ifndef SYSNCHRONIZE_H
#define	SYSNCHRONIZE_H

#define _XTAL_FREQ 1000000
#define slave
//#define master
//#define transmit
#define receive

bool flag=false;
int8_t max_y;//output voltage 
int8_t a,b;//constant
int8_t x;//input voltage 
int c_value=-1,d_value=-1,max_value=100;//ADcon value and threshold
int8_t circle_cnt=1,circle_time=1;
int8_t point,old_point;//data position
int8_t rc,receive_databuf=0,transmit_databuf=13,t2=4;//0b1111;//0;
int8_t old_value=0,msk=0b1000;
int8_t cnt=4,cntb=0;
int8_t receive_data=NULL,transmit_data=20;
int8_t transmit_databuf1=0b1111;
int8_t m,u;//coefficient
float  T,t,y=0;//interrupt period(s)
//time constant(s) 


void synchro_init(){
    x=10;
    T=0.001;
    max_y=2;
    t=0.1;
    a=(T/t)/(1+(T/t));
    b=1/(1+(T/t));
    u=1;
    m=0;
}

void sensor_read(){
     
    ADCON0=0b00001001;//left justified AN2(RA2)adconverter on
    __delay_us(10);
    ADCON0bits.GO=1;
    while(ADCON0bits.GO);
    d_value=ADRESH;//|(ADRESL>>6);
    //if(d_value>=max_value)RC2=1;
    //m+=(c_value>=max_value)?10:0;
    //transmit_data=d_value;
    if(d_value>103)rc=1;
    //receive_databuf=(receive_databuf<<1)|rc;
    //transmit_data=receive_databuf;
    //RB5=1;
    
    ADCON0=0b00010001;// left justfied AN3(RA4) adconverter on 
    __delay_ms(10);
    ADCON0bits.GO=1;
    while(ADCON0bits.GO);
    c_value=ADRESH;//ADRESH+ADRESL;
    //transmit_data=c_value;
    
    if((c_value>115)&&(old_value<115)){
         flag=true;
#ifdef receive
         TMR1IE=1;//enable timer1 intrrupt
#endif
         //receive_data=receive_databuf; 
         //transmit_data=receive_data;
         //receive_databuf=0;
        }
    else{
         //transmit_data=0;
    }
    old_value=c_value;
    //transmit_data=c_value;
    //RB5=1; 
     
     //transmit_data=receive_databuf;
    
    
}


void __interrupt()slave_interrupt(void){
    
    if(SSPIF==1){
        /*while(SSPSTATbits.BF)*/
       
        //RB5=1;
        if(SSPCONbits.SSPOV)SSPCONbits.SSPOV=0;
        //if(SSPSTAT==0b00001100)RB5=1;
        //if(SSPCONbits.SSPEN)
        SSPBUF=transmit_data;
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
      //  transmit_data=0;
        if(flag){
                //receive_data=receive_databuf; 
                //transmit_data=1;
                //receive_databuf=0;
                //flag=false;
            y=max_y;
            circle_cnt=5;
            flag=false;
        }       
        if(y>=max_y){
//#ifdef transmit
  //          RB5=1;//clock led on   
    //        __delay_ms(150);
//#endif
            y=0;
            circle_time=circle_cnt;
            circle_cnt=1;
        //    transmit_data=1;
            
        }
        else{
            //transmit_data=0;
            //RB4=0;//clock led off
        }
       
        m=0;
        old_point=point;
        point=circle_cnt*4/circle_time;
        //transmit_data=circle_cnt;
        if(point!=old_point){
#ifdef receive
            
            receive_databuf=(receive_databuf<<1)|rc;
           
            //transmit_data=receive_databuf;
            if(point!=0){
            //transmit_data=receive_databuf;
            //    transmit_data=0;
            }
            cntb++;
            
        
#endif      
#ifdef transmit
            cnt--;
            
            RC1=(transmit_databuf&msk)>>cnt;//0b1;
            __delay_ms(30);
            
            
            //transmit_data=0;//(transmit_databuf&msk)>>cnt;  
            //transmit_data=(transmit_databuf&msk)>>a;//transmit_databuf&0b1;
            //transmit_databuf=transmit_databuf>>1;
            msk=msk>>1;
#endif
        if(point==0){
#ifdef transmit
#ifdef transmit
            RB5=1;//clock led on   
            __delay_ms(30);
#endif
             //RC1=transmit_databuf&0b1;
             //__delay_ms(150);
            //if(cnt<16)cnt++;
            //else cnt=0;
            //transmit_databuf=t2;
            transmit_data=transmit_databuf;
            msk=0b1000;
            cnt=4;
#endif
            
#ifdef receive
            
            receive_data=receive_databuf; 
            transmit_data=receive_data;//receive_databuf;
            //transmit_data=1;
            receive_databuf=0;
            cntb=0;
             
#endif
        }
             rc=0;
        }

        
        
        TMR1IF=0;
        TMR1ON=1;
    }
         
    
    //SSPIF=0;
    //GIE=1;//permit other interrupt

    
}

#endif
