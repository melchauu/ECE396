   

//int pwrPin = A0;     // analog input pin 0  for the power available setting ( voltg division)
double pwrAvl = 0;      // Power Available = stores the value from pwrPin
double pwr1,pwr2,pwr3,e1,e2,e3;

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
 
 /** SWITCHES **/
    //configure pin2 as an input and enable the internal pull-up resistor
  pinMode(2, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  pinMode(4, INPUT_PULLUP);
  pinMode(12, OUTPUT);

}



void loop()

{
/* analogWrite(aplcSrt[1].lb, 200);
analogWrite(aplcSrt[2].lb, 200);
analogWrite(aplcSrt[3].lb, 200);*/

/**SWITCHES*/

 //read the pushbutton value into a variable
  int sensorVal1 = digitalRead(2);
  int sensorVal2 = digitalRead(4);
  
  //print out the value of the pushbutton
  Serial.println(sensorVal1);
  Serial.println(sensorVal2);

  // Keep in mind the pullup means the pushbutton's
  // logic is inverted. It goes HIGH when it's open,
  // and LOW when it's pressed. Turn on pin 13 when the
  // button's pressed, and off when it's not:
  if (sensorVal1 == HIGH && sensorVal2 == LOW) {
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW); // green on
    pwrAvl = 2; // medium  mid peak (red)
      Serial.println("at midpeak see red");
      delay(2000);               // wait for 2 second
  }

  else if (sensorVal2 == HIGH && sensorVal1 == LOW) {
    //sensorval1 = sw2  // sensorval2 = sw4
    digitalWrite(13, LOW);
    digitalWrite(12, HIGH);
    pwrAvl = 3;  // low   off peak( green)
    Serial.println("low peak see green"); 
    delay(2000);               // wait for 2 second
    
    
  }

  else if (sensorVal1 == HIGH && sensorVal2 == HIGH) {
  digitalWrite(12, LOW);  
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
  Serial.println("at very high peak  red blink");   
  
   pwrAvl = 1;  // very high ( red blink  on peak)

  }
  else {
   pwrAvl = 4; //descide what this level is
  }
  
    /* SWITCHES */
    
  Serial.println("start"); 
  Serial.println("");
  
  if(threshSetup == 0)
    for (int i=0; i< 3; i++){
    double threshTmp = aplcSrt[i].thresh*100;
   // Serial.println ("threshtmp");
   // Serial.println (threshTmp);
   // Serial.println ("aplc thresh");
  //  Serial.println (aplcSrt[i].thresh);   
    threshTmp = map(threshTmp, 0, 293, 0, 500); // 2.93 Volt is our expected max voltage, we map this to 5 because 5 is a nice round number
    
    //Serial.println ("aplc thresh final");
    //Serial.println (threshTmp); 
    aplcSrt[i].thresh = threshTmp/100;
    
    
    threshSetup=1;
  }  

  
  if (pwrAvl!=4){
      pwrAvl =  pwrAvl * 7;       
//      pwrAvl =  pwrAvl * 5;       
      //analogRead(A0); // Analog read expects something from 1 to 0, we're only getting 4V max after voltg division
      //pwrAvl = map(pwrAvl, 0, 1023, 0, 30);
    
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
             aplcSrt[i].pwm = 100;
             //Serial.println("aplc thresh");
             //Serial.println(aplcSrt[i].thresh);
             pwrAvl = pwrAvl - aplcSrt[i].thresh;
             //Serial.println("pwrAvl - setting everyone to thresh");
             //Serial.println(pwrAvl);         
         }    
      } 
    
     
      
     // let 5 = max voltage of every appliance 
     for ( int i=0; i<3; i++){
       if ( digitalRead(aplcSrt[i].ui) == HIGH )
       {
           if ((pwrAvl - (5 - aplcSrt[i].thresh)) >= 0 )
           {  
             aplcSrt[i].pwm = 250; //high bright
             pwrAvl = pwrAvl - (5 - aplcSrt[i].thresh);
             //Serial.println("pwrAvl enough to give MAX");
             //Serial.println(pwrAvl);       
           }
           
           else if ( pwrAvl >0.6 ) 
           {
             aplcSrt[i].pwm = aplcSrt[i].pwm  + map (pwrAvl, 0, 5, 0, 255 ); /// medium  brught
             aplcSrt[i].pwm = 160;
             
            
             i=3; // break out of loop
             pwrAvl = 0;
           //  Serial.println("pwrAvl lt MAX therefore give med");
            // Serial.println(pwrAvl);
           }
           else{
             pwrAvl=0;
             
           }  
       }
       else {
         aplcSrt[i].pwm = 0; 
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
  
  energyCalculator ( aplcSrt[0].pwm,aplcSrt[1].pwm,aplcSrt[2].pwm);

}

void energyCalculator(double pwmrd1, double pwmrd2, double pwmrd3){

  if (pwmrd1 == 100){
    pwr1=0.43*0.15;
  }
  else if(pwmrd1 == 160){
    pwr1 = 1.4*0.35;
  }
  else if (pwmrd1 == 250){
    pwr1 = 2.93*0.6;
  }

  if (pwmrd2 == 100){
    pwr2=0.43*0.15;
  }
  else if(pwmrd2 == 160){
    pwr2 = 1.4*0.35;
  }
  else if (pwmrd2 == 250){
    pwr2 = 2.93*0.6;
  }  

  if (pwmrd3 == 100){
    pwr3=0.43*0.15;
  }
  else if(pwmrd3 == 160){
    pwr3 = 1.4*0.35;
  }
  else if (pwmrd3 == 250){
    pwr3 = 2.93*0.6;
  }

  //Get energy
  /*  e1 = pwr1*0.1+e1;
    e2 = pwr2*0.1+e2;
    e3 = pwr3*0.1+e3;
    */
    e1 = pwr1*2+e1;
    e2 = pwr2*2+e2;
    e3 = pwr3*2+e3;
    Serial.println("energy of appliance 1");
    Serial.println(e1, 8);
    //Serial.print(" ");
    Serial.println("energy of appliance 2");
    Serial.println(e2, 8);
    //Serial.print(" ");
    Serial.println("energy of appliance 3");
    Serial.println(e3, 8);
    //Serial.print(" ");
    Serial.print("\n");
  
}
