/*
Code Author: Kyle Lemma
Last Date Modified: 12/5/2022
Code's Purpose: To create a system that will monitor the temperature and humidity of a refrigerated area
and notify the user via a buzzer and the LCD screen when the temperature or humidity goes out of that range
Corresponding Assignment: CSE 321 Project 3 Project
Inputs: 
    Martrix Keypad: PC6
    DHT11: PB1
Outputs: 
    Matrix Keypad: PB8, PB9
    1602 LCD: PC1, PC0
    Red LED: PA3
    Green LED: PA2
    Buzzer Alarm: PC8
Constraints: 
    - When the temperature or humidity goes out of the desired range to keep refrigerated food safe to eat an alarm should sound
    - When the system is disengaged the Red LED will be on and the Green LED will be off
    - When the system is disengaged the humidity and temperature sensor should still be monitoring and updating
    - When the system is disengaged the alarm should never sound
References:
*/
#include "mbed.h"
#include "lcd1602.h"
#include "DHT11.h"
#include <cstdio>
#include <string>
#include <iostream>

using namespace std;

void systemTempError();
void systemHumidityError();
void checkMetrics();
void systemEngage();
void systemDisengage();
void checkLevels(int temp,int humidity);

InterruptIn buttonPushA(PB_8,PullDown);
InterruptIn buttonPushB(PB_9,PullDown);
CSE321_LCD lcd(16,2,LCD_5x10DOTS,PC_1,PC_0);
DHT11 monitor(PB_1);

DigitalOut rLED(PA_3);
DigitalOut gLED(PA_2);
DigitalOut keypadColumn(PC_6);

EventQueue queue1(32*EVENTS_EVENT_SIZE);
Thread thread;
Ticker tick;

Mutex mutex;

Watchdog &timer = Watchdog::get_instance();

bool check;
int temp = 0;
int humidity = 0;


/*
**********************************************************************************************************
main is the function that houses all initlizations and other function calls in one location
    - Initializations for Inputs and Ouputs are made here
    - Intializations for the clock is also made in this function
    - Initializations for the Interrupts, pushButtonA and pushButtonB along with the ISR
    - Initializations for the LCD
    - Initializations for the Buzzer Alarm
    - Initializations for the DHT11
    - Initializations for WatchDog Timer
    - Initializations for Threads and Ticker
    - While loop for the function to run "forever"
    - Nested while loop to represent the system being "Engaged"
**********************************************************************************************************
*/

int main()
{
    //Clock turned on for GPIO A,B and C
    RCC->AHB2ENR |= 0x7;

    //Bitwise Setup for PC8 for the Buzzer Activation
    GPIOC->MODER |= 0x10000;
    GPIOC->MODER &= ~(0x20000);
    GPIOC -> ODR &= ~(0x100);
    
    keypadColumn = 1;
    rLED = 1;
    lcd.begin();
    lcd.clear();
    check = false;

    buttonPushA.rise(&systemEngage);
    buttonPushA.enable_irq(); 
    buttonPushB.rise(&systemDisengage);
    buttonPushB.enable_irq();
    
    thread.start(callback(&queue1,&EventQueue::dispatch_forever));
    tick.attach(queue1.event(checkMetrics),2s);
    timer.start(10000);

    while(1){
        timer.kick();

        while(check){
            timer.kick();
            checkLevels(temp,humidity);
            wait_us(5000000);
             
        }
            
    }
}

//Additional Functions:

/*
**********************************************************************************************************
checkMetrics is the function that is used to read in data from the DHT11 and output the data onto the LCD
    - Uses the mutex to lock all global variables so they cannot be accessed by another other thread
    - Takes in temperature and humidity from the DHT11 and write them to the LCD display in form of 
    char arrays
**********************************************************************************************************
*/

void checkMetrics(){
        mutex.lock();
        lcd.clear();
        monitor.read();
        temp = monitor.getCelsius();
        string tempString = to_string(temp).c_str();
        string reading = "Temp is: ";
        string result = tempString + " C ";
        
        lcd.print(reading.c_str());
        lcd.print(result.c_str());
        lcd.setCursor(0, 1);

        humidity = monitor.getHumidity();
        string humidString = to_string(humidity).c_str();
        string humReading = "Humidity is: ";
        string humRes = humidString+ "% ";

        lcd.print(humReading.c_str());
        lcd.print(humRes.c_str());
      
        lcd.home();
        mutex.unlock();
}

/*
**********************************************************************************************************
getTemp is the function that is used to read in data from the DHT11 without the use of the LCD
    - Uses the mutex to lock all global variables so they cannot be accessed by another other thread
    - Takes in temperature reading in Celcius from the DHT11 and returns the value as an integer
**********************************************************************************************************
*/

int getTemp(){
        mutex.lock();
        monitor.read();
        int tempRead = monitor.getCelsius();
        mutex.unlock();
        return tempRead;
}

/*
**********************************************************************************************************
getHumid is the function that is used to read in data from the DHT11 without the use of the LCD
    - Uses the mutex to lock all global variables so they cannot be accessed by another other thread
    - Takes in humidity reading from the DHT11 and returns the value as an integer
**********************************************************************************************************
*/

int getHumid(){
        mutex.lock();
        monitor.read();
        int humidRead = monitor.getHumidity();
        mutex.unlock();
        return humidRead;
}

/*
**********************************************************************************************************
checkLevels is the function that is used to take the data found from checkMetrics and compare the values
to what the expected values should be for a refrigeration unit
    - Disables interrupts so the state cannot be changed while checking the values
    - Checks if the temperature is in the range from 1 to 4 degrees Celcius
    - If not calls systemTempError
    - Checks if the humidity is in the range from 30 to 50 percent
    - If not calls systemHumidityError
**********************************************************************************************************
*/

void checkLevels(int temp, int humidity){
    tick.detach();
    buttonPushA.disable_irq();
    buttonPushB.disable_irq();

    if(temp < 1 || temp > 4){
        systemTempError();
    }
    else if(humidity > 50 || humidity < 30){
        systemHumidityError();
    }
    
    buttonPushA.enable_irq();
    buttonPushB.enable_irq();
    tick.attach(queue1.event(checkMetrics),2s);
}

/*
**********************************************************************************************************
systemTempError is the function that is used to hold the system in a state of error until the temperature
has gone back to a suitable level
    - While loop that will loop while the temperature is not at a suitable level
        -LCD will display that there is a temperature error
    - Buzzer will sound while in while loop
    - If block to check if the temperature has returned to a suitable level
        -Buzzer will turn off
**********************************************************************************************************
*/

void systemTempError(){
    bool tempReturned = true;
    lcd.clear();
    while(tempReturned){
        GPIOC -> ODR |= (0x100);
        lcd.print("Temp Error");
        wait_us(2000000);
        lcd.clear();

        int tempFix = getTemp();
        if(tempFix < 4 && tempFix > 1){
            tempReturned = false;
            GPIOC -> ODR &= ~(0x100);
            lcd.clear();
        }
    }
    lcd.clear();
}

/*
**********************************************************************************************************
systemHumidityError is the function that is used to hold the system in a state of error until the humidity
has gone back to a suitable level
    - While loop that will loop while the humidity is not at a suitable level
        -LCD will display that there is a humidity error
    - Buzzer will sound while in while loop
    - If block to check if the humidity has returned to a suitable level
        -Buzzer will turn off
**********************************************************************************************************
*/

void systemHumidityError(){
    bool humidReturned = true;
    while(humidReturned){
        GPIOC -> ODR |= (0x100);
        lcd.print("Humidity Error");
        wait_us(2000000);
        lcd.clear();
        int humidFix = getHumid();
        if(humidFix < 50 && humidFix > 30){
            humidReturned = false;
            GPIOC -> ODR &= ~(0x100);
            lcd.clear();
        }
    }
    lcd.clear();
}

/*
**********************************************************************************************************
systemEngage is the function that is used to for the pushButtonA interrupt to activate the system
    - Turns the flag for the nested while loop in main to true
    - Switches the red LED to off
    - Switches the green LED to on
**********************************************************************************************************
*/

void systemEngage(){
    check = true;
    rLED = 0;
    gLED = 1;
}

/*
**********************************************************************************************************
systemDisengage is the function that is used to for the pushButtonB interrupt to deactivate the system
    - Turns the flag for the nested while loop in main to false
    - Switches the red LED to on
    - Switches the green LED to off
**********************************************************************************************************
*/

void systemDisengage(){
    check = false;
    rLED = 1;
    gLED = 0;
}
