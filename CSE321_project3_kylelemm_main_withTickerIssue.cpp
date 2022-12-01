/*
Code Author: Kyle Lemma
Last Date Modified: 11/27/2022
Code's Purpose: To create a system that will monitor the temperature and humidity of a refrigerated area
and notify the user when the temperature or humidity goes out of that range
Corresponding Assignment: CSE 321 Project 3 Project
Inputs: 
    Martrix Keypad:
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
//Flag if the button to engage the system has been pushed or not
//Use a boolean to be the argument of the while loop
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
DigitalOut buzzer(PC_8,PullDown);

EventQueue queue1(32*EVENTS_EVENT_SIZE);
Thread thread;
Ticker tick;

bool check;
int temp = 0;
int humidity = 0;

int main()
{
    //Clock turned on for GPIO B and C
    RCC->AHB2ENR |= 0x7;

    keypadColumn = 1;
    buzzer.write(0);
    rLED = 1;
    lcd.begin();
    lcd.clear();
    check = false;

    buttonPushA.rise(&systemEngage);
    buttonPushA.enable_irq(); 
    buttonPushB.rise(&systemDisengage);
    buttonPushB.enable_irq();
    
    thread.start(callback(&queue1,&EventQueue::dispatch_forever));
    //tick.attach(queue1.event(checkMetrics),2);
    tick.attach(queue1.event(checkMetrics),2s);

    while(1){
        //checkMetrics();
        //tick.attach(queue1.event(checkMetrics,2));
        //Wait 2 seconds for monitor read to reset
        //wait_us(2000000);
        //Waiting for the system to either be enganged or disengaged by the matrix keypad   
        while(check){
        
            checkMetrics();
            wait_us(2000000);
            checkLevels(temp,humidity);
            wait_us(2000000);
        
            
            //Monitor the temperature and humidity levels
            //If either the temp or humid gets too high or low go to systemError
        }
            
    }
}



void checkMetrics(){
        
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
}

void checkLevels(int temp, int humidity){
    buttonPushA.disable_irq();
    buttonPushB.disable_irq();

    if(temp < 1 || temp > 4){
        systemTempError();
    }
    if(humidity > 50 || humidity < 30){
        systemHumidityError();
    }
    
    buttonPushA.enable_irq();
    buttonPushB.enable_irq();
}


void systemTempError(){
    bool tempReturned = true;
    while(tempReturned){
        buzzer = 1;
        lcd.clear();
        lcd.print("Temp Error");

        if(temp < 4 && temp > 1){
            tempReturned = false;
            buzzer = 0;
            lcd.clear();
        }
    
    }
    //Sound the Alarm
    //Display on LCD that the System is having a problem
    //while(temp or humidity is too high wait till it fixes)
    //Once the temp or humidity corrects itself go back to monitoring the temp or humidity
}

void systemHumidityError(){
    bool humidReturned = true;
    while(humidReturned){
        buzzer = 1;
        lcd.print("Humidity Error");
        if(humidity < 50 && humidity > 30){
            humidReturned = false;
            buzzer = 0;
            lcd.clear();
        }
    }
}

void systemEngage(){
    check = true;
    rLED = 0;
    gLED = 1;
}

void systemDisengage(){
    check = false;
    rLED = 1;
    gLED = 0;
}
