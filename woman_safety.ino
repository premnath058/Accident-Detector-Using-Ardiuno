/*
  Serial Event example
 
 When new serial data arrives, this sketch adds it to a String.
 When a newline is received, the loop prints the string and 
 clears it.
 
 A good test for this is to try it with a GPS receiver 
 that sends out NMEA 0183 sentences. 
 
 Created 9 May 2011
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/SerialEvent
 
 */
 
 /*
  LiquidCrystal Library - Hello World
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch prints "Hello World!" to the LCD
 and shows the time.
 
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 
 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>

#include <EEPROM.h>



//const int analogInPin1 = A0;  // Analog input pin that the potentiometer is attached to


unsigned int load = 0,gas=0;        // value read from the pot
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

 
/*const int analogInPin2 = A1;  // Analog input pin that the potentiometer is attached to

const int analogInPin3 = A2;
const int analogInPin4 = A3;
const int analogInPin5 = A4;
const int analogInPin6 = A5;
*/






const int LED = 13;
unsigned long x,y,z;
float load_weight=0;

const int fire = 10;


char a[73];
unsigned int m=0;
unsigned char mm,tt=0,load_cell=0,key=0;
char lat[11],lon[11];




boolean stringComplete = false;  // whether the string is complete
boolean started=false;


unsigned int sec=0;

void keypad();
void received();

void Msg_Send();
void send_msg();
unsigned long ping();

unsigned int temp,humi,ff=0,vib;


void configure();
void read_weight();





void send_msg()
{
  
  unsigned int kk=0;

  Serial.print('*');
  if(key < 100){Serial.print("100");}
  else{Serial.print("000");}

 if(vib > 900){Serial.println("100");}
  else{Serial.println("000");}
 // Serial.println(vib);
  
}

void receive()
{
  unsigned int i=0;
 if(m > 51)
 {
     for(i=0;i<51;i++)
     {
        //Serial.print(a[i]);
        
      
     }
     for(i=19;i<28;i++)
     {
        //Serial.print(a[m]);
        lat[i-20] = a[i];
       // Serial.print(lat[i-20]);
     }

     for(i=32;i<41;i++)
     {
        //Serial.print(a[m]);
        lon[i-33] = a[i];
       // Serial.print(lon[i-33]);
     }
     

    m =0;
  
 }

}

void setup() {
  // initialize serial:
  
  
   // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("   ARDUINO BASED");
  
  pinMode(1,INPUT_PULLUP);
 // pinMode(1,INPUT_PULLUP);
  pinMode(0,INPUT_PULLUP);

   pinMode(A0,INPUT_PULLUP);
    pinMode(A2,INPUT_PULLUP);
 

  
  
  pinMode(LED, OUTPUT);   
  digitalWrite(LED, HIGH);

 
  
  
  
  
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  //inputString.reserve(200);
  delay(1000);
  sec=170;
 
  
  lcd.clear();

 
   started=true;
}


void loop() {

 
      key = analogRead(A0);
      vib = analogRead(A2);

      
      if(vib > 900 || key < 100){sec=200;}
      //send_msg();
    
      sec++;
     delay(50);

      if(sec > 200)
      {
           sec = 0;
         send_msg();
         delay(2000);
        
         sec=0;
      }


     // if(vib > 500){digitalWrite(LED,HIGH);delay(3000);}
     // else{digitalWrite(LED,LOW);}
     
    
    
      
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar;
    
    
    //a[m] = (char)Serial.read(); 
    inChar = (char)Serial.read(); 
    
    
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
     if(started == true)
     {
      a[m] = inChar;
     }
      
    
    // add it to the inputString:
    //inputString += inChar;
             // if the incoming character is a newline, set a flag
                // so the main loop can do something about it:
                
       // Serial.print(a[m]);

         if(a[0]!='$'){m=0; goto last;}
   	 else if(a[0]=='$'&&a[1]!='G'){m=1; goto last;}
   	 else if(a[0]=='$'&&a[1]=='G'&&a[2]!='P'){m=2; goto last;}
   	 else if(a[0]=='$'&&a[1]=='G'&&a[2]=='P'&&a[3]!='R'){m=3; goto last;}
   	 else if(a[0]=='$'&&a[1]=='G'&&a[2]=='P'&&a[3]=='R'&&a[4]!='M'){m=4; goto last;}
   	 else if(a[0]=='$'&&a[1]=='G'&&a[2]=='P'&&a[3]=='R'&&a[4]=='M'&&a[5]!='C')
         {
              if(m < 50)
              {
                m++;
                goto last;
              }
         }
   	 last:; 
        
         //Serial.print(m);
        // inputString += inChar;
         
        /* if(inChar == '\n')
         {
           	
           stringComplete = true;
         }
         */
   
     
  }
}








