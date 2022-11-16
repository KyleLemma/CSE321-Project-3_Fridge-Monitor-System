/*
Code Author: Kyle Lemma
Last Date Modified: 11/10/2022
Code's Purpose: To create a system that will monitor the temperature and humidity of a refrigerated area
and notify the user when the temperature or humidity goes out of that range
Corresponding Assignment: CSE 321 Project 3 Project
Inputs: 
    Martrix Keypad:
    DHT11:
Outputs: 
    Matrix Keypad: 
    1602 LCD: 
    Red LED: 
    Green LED:
    Buzzer Alarm:
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
void systemError();
void checkMetrics();

InterruptIn buttonPushA(PB_8,PullDown);
InterruptIn buttonPushB(PB_8,PullDown);
CSE321_LCD lcd(16,2,LCD_5x10DOTS,PC_1,PC_0);
DHT11 monitor(PB_1);

DigitalOut rLED(PA_3);
DigitalOut gLED(PA_2);
DigitalOut buzzer(PC_8,PullDown);



int main()
{
    //Clock turned on for GPIO B and C
    RCC->AHB2ENR |= 0x7;
    
    lcd.begin();
    bool check = false;
    
    while(1){
        
        rLED = 1;
        checkMetrics();
        //Wait 2 seconds for monitor read to reset
        wait_us(2000000);

        //Waiting for the system to either be enganged or disengaged by the matrix keypad   
        while(check){
            checkMetrics();
            rLED = 0;
            gLED = 1;
        }
            //Monitor the temperature and humidity levels

            //If either the temp or humid gets too high or low go to systemError

    }
}



void checkMetrics(){
        
        monitor.read();
        int temp = monitor.getCelsius();
        string tempString = to_string(temp).c_str();
        string reading = "Temp is: ";
        string result = tempString + " C";
        
        lcd.print(reading.c_str());
        lcd.print(result.c_str());
        lcd.setCursor(0, 1);

        int humidity = monitor.getHumidity();
        string humidString = to_string(humidity).c_str();
        string humReading = "Humidity is: ";
        string humRes = humidString+ "%";

        lcd.print(humReading.c_str());
        lcd.print(humRes.c_str());
        lcd.home();

}


void systemError(){
    //Sound the Alarm
    //Display on LCD that the System is having a problem
    //while(temp or humidity is too high wait till it fixes)
    //Once the temp or humidity corrects itself go back to monitoring the temp or humidity
}

