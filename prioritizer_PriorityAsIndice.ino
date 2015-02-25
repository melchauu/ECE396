   

int pwrPin = A0;     // analog input pin 0  for the power available setting ( voltg division)
double pwrAvl = 0;      // Power Available = stores the value from pwrPin

typedef struct
 {
     int ui ;          // DIGITAL INPUT: user input for appliance ( on/off)
     int pr;           // priority  lower number = highest priority ie 1 = highest, 2 = med , 3 = low
     int lb;           // lightbulb ( pwm Pin used to drive lightbulb ) 
     double thresh;       // min voltg threshold
     int pwm; 
      
 }  applc_str;
 
   applc_str aplcSrt[3]; // applianceSort  this is an array of appliances sorted by priority
  
int threshSetup = 0;
  

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
  
  
 // open serial port!
 Serial.begin(9600); 

}



void loop()

{
/* analogWrite(aplcSrt[1].lb, 200);
analogWrite(aplcSrt[2].lb, 200);
analogWrite(aplcSrt[3].lb, 200);*/

  Serial.println("start"); 
  Serial.println("");
  
  if(threshSetup == 0)
    for (int i=0; i< 3; i++){
    double threshTmp = aplcSrt[i].thresh*100;
    Serial.println ("threshtmp");
    Serial.println (threshTmp);
    Serial.println ("aplc thresh");
    Serial.println (aplcSrt[i].thresh);   
    threshTmp = map(threshTmp, 0, 293, 0, 500); // 2.93 Volt is our expected max voltage, we map this to 5 because 5 is a nice round number
    
    Serial.println ("aplc thresh final");
    Serial.println (threshTmp); 
    aplcSrt[i].thresh = threshTmp/100;
    
    
    threshSetup=1;
  }  

  delay(2000);
  pwrAvl = analogRead(A0); // Analog read expects something from 1 to 0, we're only getting 4V max after voltg division
  pwrAvl = map(pwrAvl, 0, 1023, 0, 30);
  Serial.println(pwrAvl);  
  Serial.println("poweravailable");  
  // map pwrAvl to val btwn 0 - 18  ( each applc is allowed 5 volts max) 
  
  // everyone at bare min gets their thresholds met 
  for (int i=0; i<3;i++){
        Serial.println("digitalread");
        Serial.println(digitalRead(aplcSrt[i].ui));
        Serial.println(i);
        Serial.println("");
                
     if ( digitalRead(aplcSrt[i].ui) == HIGH ){
         aplcSrt[i].pwm = map (aplcSrt[i].thresh, 0, 5, 0, 255 ); // super dim
         aplcSrt[i].pwm = 42;
         Serial.println("aplc thresh");
         Serial.println(aplcSrt[i].thresh);
         pwrAvl = pwrAvl - aplcSrt[i].thresh;
         Serial.println("pwrAvl - setting everyone to thresh");
         Serial.println(pwrAvl);         
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
         Serial.println("pwrAvl enough to give MAX");
         Serial.println(pwrAvl);       
       }
       
       else if ( pwrAvl >0.6 ) 
       {
         aplcSrt[i].pwm = aplcSrt[i].pwm  + map (pwrAvl, 0, 5, 0, 255 ); /// medium  brught
         aplcSrt[i].pwm = 169;
         
        
         i=3; // break out of loop
         pwrAvl = 0;
         Serial.println("pwrAvl lt MAX therefore give med");
         Serial.println(pwrAvl);
       }
       else
         pwrAvl=0;
   }  
 }

for (int i=0; i<3;i++){
   analogWrite(aplcSrt[i].lb, aplcSrt[i].pwm);
   Serial.println("aplc pwm");
   Serial.println(aplcSrt[i].pwm);
   Serial.println("aplc pri");
   Serial.println(aplcSrt[i].pr);
   Serial.println("appliance indice");
   Serial.println(i);
 }
}
