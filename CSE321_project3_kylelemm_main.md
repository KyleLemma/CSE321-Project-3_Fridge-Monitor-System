
-------------------
About
-------------------
Project Description: Creation of a Fridge Monitoring System to monitor live temperature and humidity to make sure the refrigerated stays ideal for keeping food safe and not wasted

Contribitor List: Kyle Lemma

--------------------
Features
--------------------
  - The system will constantly read temperature and humidity readings using the DHT11
  - The readings from the DHT11 will be constantly updated on the 1602 LCD Display
  - The system will be engaged and disengaged by using the Matrix Keypad
  - When engaged the Green LED will be illuminated
  - When disengaged the Red LED will be illuminated 
  - When engaged and if the temperature or humidity is outside of the ideal range the buzzer alarm will sound
  
--------------------
Required Materials
--------------------
- Nucleo L552ZE-Q
- Matrix Keypad
- Breadboard
- Red and Green LEDs
- 1602 LCD Screen
- DHT11
- Buzzer Alarm
- Jumper Wires

--------------------
Resources and References
--------------------

Github Repository Link : https://github.com/KyleLemma/CSE321-Project-3_Fridge-Monitor-System.git

--------------------
Getting Started
--------------------

 - For Getting started you are required to properly connect all inputs and output devices to the Nucleo for the keypad, LCD screen , Red and Green LEDs, DHT11, and Buzzer Alarm
 - Build the project in MBed Studio 
 - Run the project from MBed Studio
 - Test the functionality by monitoring the LCD updating with metrics when the system is disengaged
 - Test the functionality by monitoring the LCD update with metrics when the system is engaged
 - Test the engage and disengage function using the matrix keypad
 - Test the alarm functionality by heating up the surrounding environment so the temperature or humidity goes out of the desired range
  
--------------------
CSE321_project3_kylelemm_main.cpp:
--------------------
 
This file cotains the contents for a 4-digit combination lock. This 4-digit code will be entered by a user using a Matrix Keypad, an LED will turn on each time a button on the keypad is pressed. If the correct 4-digit code was inputted, the LCD Screen will display "Unlocked" upon the 4th button press, but if the wrong code was inputted, the LCD will display a phrase, "Incorrect Code" upon the fourth button press. 

----------
Things Declared
----------

mbed.h
lcd1602.h
DHT11.h
<cstdio>
<string>
<iostream>

- void systemError()
- void checkMetrics()

- InterruptIn buttonPushA(PB_8,PullDown)
- InterruptIn buttonPushB(PB_9,PullDown)
- CSE321_LCD lcd(16,2,LCD_5x10DOTS,PC_1,PC_0)
- DHT11 monitor(PB_1)

- DigitalOut rLED(PA_3)
- DigitalOut gLED(PA_2)
- DigitalOut buzzer(PC_8,PullDown)

----------
API and Built In Elements Used
----------

Using Interrupts to control the key presses from the matrix keypad
Using the LCD drivers to power and use the LCD device
Using DigitalOut to control the buzzer and LEDs

----------
Custom Functions
----------

- systemError: This function will hold the contents for when the temperature or humidity goes out of the ideal range
- checkMetrics: Function reads in the temperature from the DHT11 and displays the metrics on the 1602 LCD

