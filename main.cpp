/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "TextLCD/TextLCD.h"
#include "Keypad/Keypad.h"
#include "Buzzer/Buzzer.h"
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

void slaveReceive();
/*
 ___         _               _   _          
|   \ ___ __| |__ _ _ _ __ _| |_(_)___ _ _  
| |) / -_) _| / _` | '_/ _` |  _| / _ \ ' \ 
|___/\___\__|_\__,_|_| \__,_|\__|_\___/_||_|
*/
// LEDS
DigitalOut led(LED1);
// Limit Switches
DigitalIn lswitch_Y (PTB0);
DigitalIn lswitch_X (PTB0);
// IR sensors
DigitalIn IR_sensor (PTB0);
// Needs to enable pull up
// Buzzer 
Buzzer buzzer (PTB1);
float freq;
// LCD display
    // (rs, rw, e, d0, d1, d2, d3, LCDType type=LCD16x2)
TextLCD lcd (PTA2, PTA4, PTA5, PTD4, PTD5, PTD6, PTD7);
// Keypad
Keypad kpad (PTC12, PTC10, PTC6, PTC5, PTC4, PTC3, PTC0, PTC7);

int main(){
/* Global 
 _____       _ _   _       _ _          _   _             
|_   _|     (_) | (_)     | (_)        | | (_)            
  | |  _ __  _| |_ _  __ _| |_ ______ _| |_ _  ___  _ __  
  | | | '_ \| | __| |/ _` | | |_  / _` | __| |/ _ \| '_ \ 
 _| |_| | | | | |_| | (_| | | |/ / (_| | |_| | (_) | | | |
|_____|_| |_|_|\__|_|\__,_|_|_/___\__,_|\__|_|\___/|_| |_|                                                                                                                
*/
    IR_sensor.mode(PullUp);



    int i;
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


    slaveReceive();

    char key;
    int released = 1;
    
    // CS -> Chip Select
    //MFRC522 RfChip (PTD2, PTD3, PTD1, SPI_CS, PTD8);

    //I2C *i2cPort = new I2C(I2C_SDA, I2C_SDA);
    
}

/*
  ___ _        _          
 / __| |_ __ _| |_ ___ ___
 \__ \  _/ _` |  _/ -_|_-<
 |___/\__\__,_|\__\___/__/
*/

void idle_State (){
    char key;
    int released = 1;

    while (true) {
        key = kpad.ReadKey();                   //read the current key pressed

        if(key == '\0')
            released = 1;                       //set the flag when all keys are released
        
        if((key != '\0') && (released == 1)) {  //if a key is pressed AND previous key was released
            lcd.printf("%c", key);           
            released = 0;                       //clear the flag to indicate that key is still pressed
            state++;
            break;
        }
    }

}
void menu1_State (){
    char key;
    int released = 1;

    lcd.printf("%c", key); 
    lcd.cls();
    //i2c_byte_read(i2c_t *obj, int last)

    while (true) {
        key = kpad.ReadKey();                   //read the current key pressed
 
        if(key == '\0')
            released = 1;                       //set the flag when all keys are released
        
        if((key != '1') && (released == 1)) {  //if a key is pressed AND previous key was released
                      
            released = 0;                       //clear the flag to indicate that key is still pressed
            state++;
            break;
        }

        if((key != '2') && (released == 1)) {  //if a key is pressed AND previous key was released
                     
            released = 0;                       //clear the flag to indicate that key is still pressed
            state++;
            break;
        }


    }
}
void menu2_a_State (){

}
void menu2_b_State (){

}
void printing_State (){
    
    // Wait for sheet
    do {
        lcd.cls();
        lcd.printf(" Place sheet to begin");
        lcd.printf(" with print"); 
        
    }while (!IR_sensor);


    // Set motors to initial position
    do {
        //send("Motor 1 left")
        //send("Motor 2 left")
    }while (!lswitch_X | !lswitch_Y);

    int released = 1;
    char key;
    bool print = true;

    while (true) {
        if (receive () ==  "Finished"){
            state = STATE_SUCC;
            break;
        }
        
        key = kpad.ReadKey();                   //read the current key pressed
 
        if(key == '\0')
            released = 1;                       //set the flag when all keys are released
        
        if((key != '*') && (released == 1) ) {  //if a key is pressed AND previous key was released
                      
            released = 0;                       //clear the flag to indicate that key is still pressed
            send("Pause, !print");
            state++;
            break;
        }
        if (IR_sensor){
            send("continue print");
        }

    }
}
void pause_State (){
    lcd.cls();
    lcd.printf(" Print paused");
     
}
void succ_pro_State (){
    lcd.cls();
    lcd.printf(" Print compleated");
}


void slaveReceive(){

                    //SDA, SCL
    I2CSlave slave(PTC2, PTC1);

    char buf[10];
    char msg[] = "Slave!";

    slave.address(0xA0);


    while (1) {
        int i = slave.receive();
        switch (i) {
            case I2CSlave::ReadAddressed:
                slave.write(msg, strlen(msg) + 1); // Includes null char
                break;
            case I2CSlave::WriteGeneral:
                slave.read(buf, 10);
                printf("Read G: %s\n", buf);
                break;
            case I2CSlave::WriteAddressed:
                slave.read(buf, 10);
                printf("Read A: %s\n", buf);
                break;
        }
        for (int i = 0; i < 10; i++) {
            buf[i] = 0;    // Clear buffer
        }
    }
}