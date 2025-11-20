/*
SOVA: Smart Over-The-Counter Vending Apparatus

-A vending machine for OTC medicines.

Date: November 21, 2025
*/

#include <Arduino.h>
#include "Variables.h"

void setup(){
    Motor_Setup();
}

void loop(){
    Motor8_On();
    delay(1000);
    Motor8_Off();
    delay(1000);
    Motor7_On();
    delay(1000);
    Motor7_Off();
    delay(1000);
    Motor6_On();
    delay(1000);
    Motor6_Off();
    delay(1000);
    Motor5_On();
    delay(1000);
    Motor5_Off();
    delay(1000);
    Motor4_On();
    delay(1000);
    Motor4_Off();
    delay(1000);
    Motor3_On();
    delay(1000);
    Motor3_Off();
    delay(1000);
    Motor2_On();
    delay(1000);
    Motor2_Off();
    delay(1000);
    Motor1_On();
    delay(1000);
    Motor1_Off();
    delay(1000);
}