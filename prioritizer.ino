   

int pwrPin = A0;     // analog input pin 0  for the power available setting ( voltg division)
int pwrAvl = 0;      // Power Available = stores the value from pwrPin

typedef struct
 {
     int ui ;          // DIGITAL INPUT: user input for appliance ( on/off)
     int pr;           // priority  lower number = highest priority ie 1 = highest, 2 = med , 3 = low
     int lb;           // lightbulb ( pwm Pin used to drive lightbulb ) 
     int thresh;       // min voltg threshold
     int pwm; 
      
 }  applc_str;
 
   applc_str aplcSrt[3]; // applianceSort  this is an array of appliances sorted by priority
  

  

void setup()

{
  aplcSrt[0].ui = 6;         // DIGITAL INPUTS: user input for appliance # 1  ( on/off) 
  aplcSrt[1].ui = 7;        
  aplcSrt[2].ui = 8;        
  
  aplcSrt[0].pr = 1;        // set the priority of each appliance 
  aplcSrt[1].pr = 2;        // highest(1) priority, ui2 has med(2) priority , ui3 has lowest(3) priority
  aplcSrt[2].pr = 3;
  
  aplcSrt[0].lb = 9;              // pwm are pins 9,10,11
  aplcSrt[1].lb = 10;             // lb lightbulb        
  aplcSrt[2].lb = 11;         
  
  aplcSrt[0].thresh = 0.43;   // threshold voltages range from 0 to 5 volts
  aplcSrt[1].thresh = 0.43;
  aplcSrt[2].thresh = 0.43; 
  //sort the struct array here! 
  
 
  
/* set appliance output (lightbulb)  to PWM voltg*/
  pinMode(9, OUTPUT);   
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);

}



void loop()

{
  delay(2000);
  pwrAvl = analogRead(A0);
  pwrAvl = map(pwrAvl, 0, 1023, 0, 18);
  Serial.print(pwrAvl);  
  Serial.print("");  
  // map pwrAvl to val btwn 0 - 18  ( each applc is allowed 5 volts max) 
  
  // everyone at bare min gets their thresholds met 
  for (int i=0; i<3;i++){
     if ( digitalRead(aplcSrt[i].ui) == HIGH ){
         aplcSrt[i].pwm = map (aplcSrt[i].thresh, 0, 5, 0, 255 ); // super dim
         aplcSrt[i].pwm = 42;
         pwrAvl = pwrAvl - aplcSrt[i].thresh;
     }    
  } 

 
  
 // let 5 = max voltage of every appliance 
 for ( int i=0; i<3; i++){
   if ( digitalRead(aplcSrt[i].ui) == HIGH )
   {
       if ((pwrAvl - (5 - aplcSrt[i].thresh)) >= 0 )
       {  
         aplcSrt[i].pwm = 255; //high bright
         pwrAvl = pwrAvl - (5 - aplcSrt[i].thresh);      
       }
       
       else if ( pwrAvl >0 ) 
       {
         aplcSrt[i].pwm = aplcSrt[i].pwm  + map (pwrAvl, 0, 5, 0, 255 ); /// medium  brught
         aplcSrt[i].pwm = 169;
         i=3; // break out of loop
       }
   }  
 }

for (int i=0; i<3;i++)
   analogWrite(aplcSrt[i].lb, aplcSrt[i].pwm);
 


}
