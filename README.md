# CS362 Biometric Panic Pox Project

## Follow the google doc below for the write up!
https://docs.google.com/document/d/1CqpJ3DZOT1RPUYsXs-lYOuN5pGzEKE2jhI6wpRIRNlM/edit?usp=sharing

## Fritzing Diagram  
https://drive.google.com/file/d/1bR8FQA8x-4r1Xh5NvOd1ZHFXg3vDMiYy/view?usp=sharing


## See the final prototype presentation here 
https://www.youtube.com/watch?v=XGO43ttiWr4

## Files in repo description: 
enroll/enroll.ino  -- the adafruit code to enroll users onto your arduino for use of the fingerprint scanner

BiometricPanicBox/BiometricPanicBox.ino -- runs on arduino inside box, relays distress signal messages as tweets

Python_Email_Code/tweepy_streamer.py    -- catches tweets meeting a specific criteria, relays tweet in form of email using SMTP
Python_Email_Code/tweepy_credentials.py -- Twitter API project credentials

Biometric_Panic_Box.fzz -- fritzing diagram of the arduino circuit

sendemail.sh -- The bash script to automate execution of tweepy_streamer.py on startup of raspberry pi  


## Hardware Overview
Hardware required   
    1x Arduino Uno  
    1x 16x2 LCD Screen  
    3x Arcade Style Buttons  
    1x breadboard  
    1x Fingerprint scanner  
    1x ESP8266 Wifi module  
    1x Potentiometer  
    1x Green LED  
    3x 10k Ohm Resistor  
    2x 220 Ohm Resistor  
    1x Raspberry Pi 3 B+  
    1x Box cut to size (any material)  
    Jumper wires - solid core  


## Total Cost Breakdown of Final Implementation  
Arduino Starter Kit:       $40.13  
ESP 8266:                  $10.99  
Jumper Cables:             $5.41  
Buttons:                   $10.84  
Fingerprint Scanner:       $20.48  
Wood + Glue:               $10.00  
Raspberry Pi 3 B+:         $35.00  

Total cost of final implementation = $132.85  
