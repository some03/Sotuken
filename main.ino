#include<Arduino.h>
#include<Wire.h>

void setup(){
   Wire.begin(); 
   Serial.begin(9600);
   pinMode(5,INPUT);
}

int time=0;
void loop(){
   int i=analogRead(5);
   Wire.requestFrom(2,2);
    //Wire.beginTransmission(1);
   time=Wire.read();
   //Serial.print("TRANSMIT");
   //Serial.print(":");
   Serial.println(time);
   Wire.requestFrom(1,2);

   //Serial.print(" ");
    //Wire.beginTransmission(1);
   long data=Wire.read();
   //Serial.print("RECEIVE");
   //Serial.print(":");
   Serial.println(data);
   //Serial.print(Wire.endTransmission());
   
}
