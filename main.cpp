/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "TextLCD/TextLCD.h"
#include "Keypad/Keypad.h"
#include "Buzzer/Buzzer.h"
#include "Transmitter/Transmitter.h"
//#include "RFID-RC522/MFRC522.h"

// Jorge Pérez
// Blinking rate in milliseconds
#define BLINKING_RATE     500ms

// States:
// GLOBAL
int state = 0; 

#define STATE_IDLE      0x00
#define STATE_MENU1     0x01
#define STATE_MENU2A    0x02
#define STATE_MENU2B    0x03
#define STATE_PRINTING  0x04
#define STATE_SAVE      0x05
#define STATE_PAUSE     0x06
#define STATE_SUCC      0x07

void idle_State ();
void menu1_State ();
void menu2_a_State ();
void menu2_b_State ();
void printing_State ();
void pause_State ();
void succ_pro_State ();

/*
 ___         _               _   _          
|   \ ___ __| |__ _ _ _ __ _| |_(_)___ _ _  
| |) / -_) _| / _` | '_/ _` |  _| / _ \ ' \ 
|___/\___\__|_\__,_|_| \__,_|\__|_\___/_||_|
*/
// LEDS
DigitalOut led(LED1);
DigitalOut led2(LED2);
// Limit Switches
DigitalIn lswitch_Y (PTB0); // FIX
DigitalIn lswitch_X (PTB1); // FIX
// IR sensors
DigitalIn IR_sensor (PTC1); // FIX
// Needs to enable pull up
// Buzzer 
Buzzer buzzer (PTB3); // FIX
// LCD display
            // (rs, rw, e,       d0, d1, d2, d3, LCDType type=LCD16x2)
TextLCD lcd (D0, D4, D5, PTD4, PTD5, PTD6, PTD7);
// Keypad
Keypad kpad (PTC12, PTC10, PTC6, PTC5, PTC4, PTC3, PTC0, PTC7);
// Transmission
                // MSB -> LSB, EN
Transmitter trans (PTE21, PTE22, PTE23, PTE29, PTE30, PTE20);

int main(){
/* Global 
 _____       _ _   _       _ _          _   _             
|_   _|     (_) | (_)     | (_)        | | (_)            
  | |  _ __  _| |_ _  __ _| |_ ______ _| |_ _  ___  _ __  
  | | | '_ \| | __| |/ _` | | |_  / _` | __| |/ _ \| '_ \ 
 _| |_| | | | | |_| | (_| | | |/ / (_| | |_| | (_) | | | |
|_____|_| |_|_|\__|_|\__,_|_|_/___\__,_|\__|_|\___/|_| |_|                                                                                                                
*/
    //IR_sensor.mode(PullUp);
    
    //lcd.cls();

/*
 _  
| |   ___  ___ _ __ 
| |__/ _ \/ _ \ '_ \
|____\___/\___/ .__/
            |_|   
*/
    while (true) {
        switch (state){
            case (STATE_IDLE):
                idle_State ();
                break;
            case (STATE_MENU1):
                menu1_State ();
                break;
            case (STATE_MENU2A ):
                menu2_a_State ();
                break;
            case (STATE_MENU2B ):
                menu2_b_State ();
                break;
            case (STATE_PRINTING ):
                printing_State ();
                break;
            case (STATE_PAUSE  ):
                pause_State ();
                break;
            case (STATE_SUCC ):
                succ_pro_State ();
                break;
        }
    }

}

/*
  ___ _        _          
 / __| |_ __ _| |_ ___ ___
 \__ \  _/ _` |  _/ -_|_-<
 |___/\__\__,_|\__\___/__/
*/

void idle_State (){
    
    // Clear dislpay output
    lcd.cls();

    while (true) {
        // If a key is pressed, then we change state
        if (kpad.ReadKey() != '\0'){
            state++;
            break;
        }
        // Toggle LED to inidicate user the machine is ON and IDLE
        led = !led;
        // LEB blink effect
        ThisThread::sleep_for(500ms);   
    }

}
void menu1_State (){
    char key;
    int released = 1;

    // Clear previous ouputs
    lcd.cls();
    // Display state's menu
    lcd.printf("Choose file: ");
    lcd.printf("         A ->");  

    while (true) {
        key = kpad.ReadKey();                   //read the current key pressed
 
        if(key == '\0')
            released = 1;                       //set the flag when all keys are released
        
        // Change screen until A is pressed
        if((key == 'A') && (released == 1)) {   
            released = 0;                       
            state++;
            break;
        }
    }
}
void menu2_a_State (){
    
    // Clear previous outputs
    lcd.cls();
    // Display state's menu
    lcd.printf("1.- Square ");
    lcd.printf("2.- Triangle");  
    
    char key;
    int released = 1;

    while (true) {
        key = kpad.ReadKey();                   //read the current key pressed
 
        if(key == '\0')
            released = 1;                       //set the flag when all keys are released
        
        if((key == '1') && (released == 1)) {   //if key 1 is pressed, then send file 1 to print
            released = 0;
            // To enable rising edge
            trans.enable_data();
            // Actual bits
            trans.send(0, 0, 0, 0, 1);
            // Give some time for the rPi to register the input
            wait_ns(50);
            // Turn off enable
            trans.disable_data();                       
            state++;
            break;
        }

        if((key == '2') && (released == 1)) {  //if key 2 is pressed, then send file 2 to print
            released = 0;
            trans.enable_data();
            trans.send(0, 0, 0, 0, 0);
            wait_ns(50);
            trans.disable_data();                       
            state++;
            break;
        }
    }
}

void printing_State (){
    
    // Clear previous screen
    lcd.cls();
    // Display state's menu
    lcd.printf(" Place sheet to begin");
    lcd.printf(" with print"); 
    // Wait for sheet
    do {
        // Make sound while no sheet
        buzzer.makeBeep(1000);
    }while (!IR_sensor);

    // Stop sound
    buzzer.silence();

    // Set motors to initial position
    do {
        trans.enable_data();
        // Send wich motor to move, or keep static
        trans.send(1, 0, 0, lswitch_X, lswitch_Y);
        wait_ns(50);
        trans.disable_data();  

    }while (!lswitch_X | !lswitch_Y);

    int released = 1;
    char key;
    bool print = true;

    while (true) {
        if (/*receive ()*/ "HOLA" ==  "Finished"){
            state = STATE_SUCC;
            break;
        }
        
        key = kpad.ReadKey();                   
 
        if(key == '\0')
            released = 1;                       
        
        if(  ((key == '*') && (released == 1)) || IR_sensor ) {  
            released = 0;                       
            trans.enable_data();
            // Send stop message
            trans.send(1, 0, 0, 0, 0);
            wait_ns(50);
            trans.disable_data(); 

            state++;
            break;
        }
    }
}
void pause_State (){

    // Clear previous screen
    lcd.cls();
    // Display state's menu
    lcd.printf("Print paused");
    
    int released = 1;
        char key;
    
    while (true) {
        
        key = kpad.ReadKey();                   
 
        if(key == '\0')
            released = 1;                       
        
        if(  ((key != '*') && (released == 1)) || IR_sensor ) {  
            released = 0;                       
            trans.enable_data();
            // Send resume message TODO
            trans.send(1, 0, 0, 0, 0);
            wait_ns(50);
            trans.disable_data(); 
            // Restart print
            state = STATE_PRINTING;
            break;
        }
    }
     
}
void succ_pro_State (){

    // Clear previous screen
    lcd.cls();
    // Display state's menu
    lcd.printf("Print compleated");
    
    // Make final sound
    buzzer.makeBeep(10000);
    ThisThread::sleep_for(50ms);
    buzzer.silence();
}

