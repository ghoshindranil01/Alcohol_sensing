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
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 
*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
// LCD Driver Variables
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// MQ-3 Alcohol Sensor Driver Variables
const int AOUTpin=0;//the AOUT pin of the alcohol sensor goes into analog pin A0 of the arduino
const int DOUTpin=8;//the DOUT pin of the alcohol sensor goes into digital pin D8 of the arduino
const int ledPin=13;//the anode of the LED connects to digital pin D13 of the arduino
int limit;
int value;

//GSM Module Setup
#include <GSM.h>

#define PINNUMBER ""

// initialize the library instance
GSM gsmAccess; // include a 'true' parameter for debug enabled
GSM_SMS sms;

// char array of the telephone number to send SMS
// change the number 1-212-555-1212 to a number
// you have access to

char remoteNumber[20]= "09999999999";  

// char array of the message
char txtMsg[200]="Warning Your child is drunk right now and is trying to drive a vehicle in this state. Please contact him/her to prevent any accident and save his/her life.";

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  //MQ-3 Alcohol Sensor Setup
  Serial.begin(115200);//sets the baud rate
  pinMode(DOUTpin, INPUT);//sets the pin as an input to the arduino
  pinMode(ledPin, OUTPUT);//sets the pin as an output of the arduino
//GSM Module Setup
Serial.println("SMS Messages Sender");

  // connection state
  boolean notConnected = true;

  // Start GSM shield
  // If your SIM has PIN, pass it as a parameter of begin() in quotes
  while(notConnected)
  {
    if(gsmAccess.begin(PINNUMBER)==GSM_READY)
      notConnected = false;
    else
    {
      Serial.println("Not connected");
      delay(1000);
    }
  }
  Serial.println("GSM initialized");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);

//MQ-3 Alcohol Sensor Driver

value= analogRead(AOUTpin);//reads the analaog value from the alcohol sensor's AOUT pin
limit= digitalRead(DOUTpin);//reads the digital value from the alcohol sensor's DOUT pin
Serial.print("Alcohol value: ");
Serial.println(value);//prints the alcohol value
Serial.print("Limit: ");
Serial.print(limit);//prints the limit reached as either LOW or HIGH (above or underneath)
delay(100);
if (limit == HIGH){
  // Print a message to the LCD.
  lcd.print("Warning! You are Drunk!");
}
else if(limit == LOW){
  lcd.print("Congratulations! You are not Drunk!"); 
}
if (limit == HIGH){
sendSMS();
}

void sendSMS(){

  Serial.print("Message to mobile number: ");
  Serial.println(remoteNumber);

  // sms text
  Serial.println("SENDING");
  Serial.println();
  Serial.println("Message:");
  Serial.println(txtMsg);

  // send the message
  sms.beginSMS(remoteNumber);
  sms.print(txtMsg);
  sms.endSMS();
  Serial.println("\nCOMPLETE!\n");  
}
