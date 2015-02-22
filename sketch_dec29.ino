//manual dimming
const int transistorPin = 9;    // connected to the base of the transistor
 
 void setup() {
   // set  the transistor pin as output:
   pinMode(transistorPin, OUTPUT);
 }
 
 void loop() {
   // read the potentiometer:
   int sensorValue = analogRead(A0);
   // map the sensor value to a range from 0 - 255:
   int outputValue = map(sensorValue, 0, 1023, 0, 255);
   // use that to control the transistor:
   
   //analogWrite(transistorPin, outputValue);
   
   // LEVEL 0 = OFF //
   if (outputValue == 0)
     analogWrite(transistorPin, 0);
   // LEVEL 1 = dim //     0.43V  || 0.15A
   else if (outputValue < 84 && outputValue > 0)
     analogWrite(transistorPin, 42);
   // LEVEL 2 = medium bright // 1.4V    || 0.35A
   else if (outputValue < 169 && outputValue > 85)
     analogWrite(transistorPin, 127);  
   // LEVEL 3 = high beam //      2.93V  || 0.6A   o.o for some reason voltage across the battery 3.9 when we hookup the lightbulb.... and its 4.3 without the lightbulb
   else if (outputValue <= 255 && outputValue > 170)
     analogWrite(transistorPin, 255);    
     
 }

