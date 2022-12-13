
-------------------
About
-------------------
Project Description: Creation of a Fridge Monitoring System to monitor live temperature and humidity to make sure the refrigerated stays ideal for keeping perishable food safe and not wasted

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
 
This file cotains the contents for a refrigeration monitoring system. This system has two states which controls whether the system is engaged and monitoring the temperature and humidity or disengaged which acts as a "maintenance mode". The system pulls in the current temperature and humidity through a DHT11 every 2 seconds and verifys that those metrics are between the ideal range to store perishable foods.

----------
Things Declared
----------

- mbed.h
- lcd1602.h
- DHT11.h
- <cstdio>
- <string>
- <iostream>

- void systemTempError();
- void systemHumidityError();
- void checkMetrics();
- void systemEngage();
- void systemDisengage();
- void checkLevels(int temp,int humidity);

- InterruptIn buttonPushA(PB_8,PullDown)
- InterruptIn buttonPushB(PB_9,PullDown)
- CSE321_LCD lcd(16,2,LCD_5x10DOTS,PC_1,PC_0)
- DHT11 monitor(PB_1)

- DigitalOut rLED(PA_3);
- DigitalOut gLED(PA_2);
- DigitalOut keypadColumn(PC_6);
  
- EventQueue queue1(32*EVENTS_EVENT_SIZE);
- Thread thread;
- Ticker tick;

- Mutex mutex;

- Watchdog &timer = Watchdog::get_instance();
  
- bool check;
- int temp = 0;
- int humidity = 0;

----------
API and Built In Elements Used
----------

- Using Interrupts to control the key presses from the matrix keypad
- Using the LCD drivers to power and use the LCD device
- Using DigitalOut to control the buzzer and LEDs
- Using a Mutex to lock and unlock when changing the global temperature and humidity values
- Using a Thread to call the checkMetrics function to update the LCD screen every 2 seconds
- Using the WatchDog Timer to reset the system if the system has been in a temperature or humidity error for too long and has not corrected

----------
Custom Functions
----------

- systemError: This function will hold the contents for when the temperature or humidity goes out of the ideal range
- checkMetrics: Function reads in the temperature from the DHT11 and displays the metrics on the 1602 LCD
- checkLevels: Function to take the temperature and humidity levels and check them against the ideal range
- getTemp: Function to get the current temperature of the system without updating the LCD
- getHumid: Function to get the current humidity of the system without updating the LCD
- systemTempError: Function that will represent a temperature error that will not release until the temperature has been corrected
- systemHumidityError: Function that will represent a humidity error that will not release until the humidity has been corrected
- systemEngage: Function controlled by the interrupt "A" that will change LEDs states and engage the system
- systemDisengage: Function controlled by the interrupt "B" that will change LEDs states and disengage the system
