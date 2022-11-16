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
#include "DHT.h"

//Flag if the button to engage the system has been pushed or not
//Use a boolean to be the argument of the while loop
void systemError();

int main()
{
    while(1){
        //Waiting for the system to either be enganged or disengaged by the matrix keypad   
        //while(button has been pressed to arm){}
            //Monitor the temperature and humidity levels

            //If either the temp or humid gets too high or low go to systemError

    }
}

void systemError(){
    //Sound the Alarm
    //Display on LCD that the System is having a problem
    //while(temp or humidity is too high wait till it fixes)
    //Once the temp or humidity corrects itself go back to monitoring the temp or humidity
}


