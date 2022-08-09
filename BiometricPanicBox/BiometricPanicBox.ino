/*************************************************** 
  CODE FOR FINGERPRINT AUTHETICATION AND ENROLLMENT ATTRIBUTED BELOW

  Designed specifically to work with the Adafruit BMP085 Breakout 
  ----> http://www.adafruit.com/products/751

  These displays use TTL Serial to communicate, 2 pins are required to 
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/
 /*
  Authors - Robert Gage Goodwin (Rgoodw3@uic.edu) , Momin Hasson (mhasso2@uic.edu), Carson Goodwin (cgoodw3@uic.edu)
  Description:
              Biometric Panic Box CS 362 Project implemenation code - Code needs the arduino adafruit finger enrollment code to enroll profile for 
              the biometric fingerprint scanner.

              The code presented here connectes the following components
              ESP 8266 Wifi Module 
              3x Arcade Buttons
              16x2 LCD screen
              Green LED
              Biometric Fingerprint scanner 

              The code enables the arduino uno to send out a message in the form of a tweet based on a specific button press.
              The tweet that is sent out can be sent out without an identity attached if no identity is provided,
              if the identity is provided pre-button press, the latest entered identity will be sent out in tweet form via 
              the ESP 8266 module. 

              The user has to wait 30 seconds between button presses and message transmission. There is delays used and the LCD prompts the user 
              for when a button press has been detected, an identity has been paired, and when the message has been successfully transmitted.

  The circuit:
 * LCD RS pin to digital pin 2
 * LCD Enable pin to digital pin 3
 * LCD D4 pin to digital pin 4
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 6
 * LCD D7 pin to digital pin 7
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 " - documentation for building the circuit is found here https://www.arduino.cc/en/Tutorial/LibraryExamples/LiquidCrystalDisplay

 ESP 8266 RX PIN = 10
 ESP 8266 TX PIN = 11

 Fingerprint RX PIN = 12 
 Fingerprint TX PIN = 13

 blackButton = A2 with 10k ohm resistor
 redButton = A1 with 10k ohm resistor
 yellowButton = A0 with 10k ohm resistor
 
 Green LED = pin 8 with 220 ohm resistor

 everything in the circuit is wired to 5v aside from the esp 8266 - the esp8266 uses 3.3v

 References:
  https://www.arduino.cc/en/Tutorial/LibraryExamples/LiquidCrystalDisplay                       - arduino LCD set up documentation
  https://www.arduino.cc/reference/en/language/variables/data-types/stringobject/               - arduino string documentation
  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Button                                     - arduino button documentation
  https://create.arduino.cc/projecthub/rowan07/make-a-simple-led-circuit-ce8308                 - arduino led documentation
  https://readthedocs.org/projects/adafruit-circuitpython-fingerprint/downloads/pdf/latest/     - fingerprint documentation
  https://randomnerdtutorials.com/posting-a-tweet-with-the-esp8266/                             - ESP 8266 Tweet documentation
  */

#include <LiquidCrystal.h>             // Library code for the LCD Display methods.
#include <Adafruit_Fingerprint.h>      // Adafruit Fingerprint Library
#include <SoftwareSerial.h>            // Software serial library for ESP and Fingerprint scanner

// LCD Display pins
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Button logic
int blackButton = A2;
int redButton = A1;
int yellowButton = A0;
int blackState = 0;
int redState = 0;
int yellowState = 0;
int greenLED = 8;


// Software Serial Device Set up
SoftwareSerial ESPserial(10, 11); // RX | TX for ESP
SoftwareSerial mySerial(12, 13);  // RX | TX for Fingerprint Scanner
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()  
{
  // LCD, BUTTON,and LED setup
  lcd.begin(16, 2);
  pinMode(blackButton, INPUT); 
  pinMode(redButton, INPUT); 
  pinMode(yellowButton, INPUT); 
  pinMode(greenLED, OUTPUT); 

  // set up for the ESP8266
  ESPserial.begin(9600);
  ESPserial.println("AT+RST");
  delay(2000);
  ESPserial.println("AT+CWMODE=1");
  delay(2000);
  ESPserial.println("AT+CWJAP=\"GOODWIN\",\"19551975\"");
  delay(4000);
  ESPserial.println("AT+CIPMODE=0");
  delay(2000);
  ESPserial.println("AT+CIPMUX=1");
  delay(2000);
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  
  Serial.println("\n\nAdafruit Finger Print Entry");

  // set the data rate for the sensor serial port
  finger.begin(57600);

  // wait for fingerprint set up verification
  if (finger.verifyPassword()) {
   // Serial.println("Found fingerprint sensor!");
  } else {
   // Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
  finger.getTemplateCount();

}


// Code to send a string in the form of a tweet using an account specific API Key
void sendTweet(String x){
  ESPserial.println("AT+CIPSTART=0,\"TCP\",\"api.thingspeak.com\",80");
  delay(2000);
  int byteCount=71+x.length()+4;
  String sendBytes = "AT+CIPSEND=0,";
  sendBytes+=String(byteCount);
  ESPserial.println(sendBytes);
  delay(2000);
  String fullTweet="GET /apps/thingtweet/1/statuses/update?api_key=GVGHRT2K8ID6OBDI&status=";
  fullTweet+=x;
  fullTweet+="\n\n";
  ESPserial.println(fullTweet);
  delay(2000);
  ESPserial.println("AT+CIPCLOSE");
}


String identity = ""; // holds the identity of the user using fingerprint scanner
int count = 10;       // count to uniquely identify new distress message


void loop()
{
   // use of inner while loop for "continue" loop flow control
   while(true){ 
    int id = getFingerprintIDez(); // the biometric scanner returns an ID of the current user if in system

    // based off of enroll code and the identity's enrolled, 
    // you can edit this code with the corresponding IDs
    // of people that are using the box 
    if(id == 1){
      Serial.println("Gage");        // display name to serial monitor
      identity = "Gage";             // change the identity
      lcd.clear();                   // reset the LCD before new ID added            
      lcd.setCursor(0,1);        
      lcd.print("ID:" + identity);   // display the current ID to the LCD
    }
    if(id == 2){
      identity = "Momin";
      Serial.println("Momin");
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("ID:" + identity);
    }
    if(id == 3){
      identity = "Carson";
      Serial.println("Carson");
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("ID:" + identity);
    }
    
    blackState = digitalRead(blackButton);
    redState = digitalRead(redButton);
    yellowState = digitalRead(yellowButton);
    digitalWrite(greenLED, LOW);

    // black button is commented and the rest of the code follows the same comments
    // refer back to this function if confused about red state and yellow state
    if(blackState == HIGH){ // if the red button is pressed, send a Burglar distress signal out
      count++;
      lcd.setCursor(0,0);
      lcd.print("Signal: Burglar");                                       // print to the screen the signal
      sendTweet(identity + " Burglar Emergency 0BWtARjU "+String(count)); // send the tweet with the corresponding message attached
      identity = "";                                                      // reset identity
      delay(2000);
      digitalWrite(greenLED, HIGH);                                       // Send a green signal on the LED to denote proper message transmission
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Message Sent");                                          // Display that the message was sent
      digitalWrite(greenLED, HIGH);
      delay(2000);
      lcd.clear();
      lcd.print("waiting");                                               // Display the box is waiting 30 seconds until next tweet
      delay(26000);
      lcd.clear();
      continue;
    }
    if(redState == HIGH){ // if the red button is pressed, send a Fire distress signal out
      count++;
      lcd.setCursor(0,0);
      lcd.print("Signal: Fire"); 
      sendTweet(identity + " Fire Emergency 0BWtARjU "+String(count));
      identity = "";
      delay(2000);
      digitalWrite(greenLED, HIGH);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Message Sent");
      digitalWrite(greenLED, HIGH);
      delay(2000);
      lcd.clear();
      lcd.print("waiting");
      delay(26000);
      lcd.clear();
      continue;
    }
    if(yellowState == HIGH){ // if the yellow button is pressed, send a medical distress signal out
      count++;
      lcd.setCursor(0,0);
      lcd.print("Signal: Medical"); 
      sendTweet(identity + " Medical Emergency 0BWtARjU "+String(count));
      identity = "";
      delay(2000);
      digitalWrite(greenLED, HIGH);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Message Sent");
      digitalWrite(greenLED, HIGH);
      delay(2000);
      lcd.clear();
      lcd.print("waiting");
      delay(26000);
      lcd.clear();
      continue;
    }
  } // inner while loop
}


// Fingerprint scanner support functions written by adafruit below 

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      //Serial.println("Imaging error");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
     // Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
     // Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
     // Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
     // Serial.println("Could not find fingerprint features");
      return p;
    default:
      //Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    //Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    //Serial.println("Did not find a match");
    return p;
  } else {
   // Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  //Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  //Serial.print(" with confidence of "); Serial.println(finger.confidence); 

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  //Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  //Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}
