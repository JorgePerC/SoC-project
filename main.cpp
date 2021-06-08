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

void button();
void slaveReceive();

int main(){
    // Initialise the digital pin LED1 as an output
    DigitalOut led(LED1);
    DigitalIn lswitchY (PTB0);
    Buzzer buzzer (PTB1);

    int i;
    float freq;

    while (true) {

        if (!lswitchY){
            buzzer.makeBeep(1.5f, 1000ms);  
        }else {
            buzzer.silence();
        }
        
    }


    slaveReceive();

    
            // (rs, rw, e, d0, d1, d2, d3, LCDType type=LCD16x2)
    TextLCD lcd (PTA2, PTA4, PTA5, PTD4, PTD5, PTD6, PTD7);

    char key;
    int released = 1;
    Keypad kpad ( PTC12, PTC10, PTC6, PTC5, PTC4, PTC3, PTC0, PTC7);

    char h [] = " Hola";
    lcd.printf("%s", h);
    // CS -> Chip Select
    //MFRC522 RfChip (PTD2, PTD3, PTD1, SPI_CS, PTD8);

    while (true) {
         key = kpad.ReadKey();                   //read the current key pressed
 
         if(key == '\0')
             released = 1;                       //set the flag when all keys are released
            
         if((key != '\0') && (released == 1)) {  //if a key is pressed AND previous key was released
             lcd.printf("%c", key);           
             released = 0;                       //clear the flag to indicate that key is still pressed
         }

        led = !led;
        ThisThread::sleep_for(BLINKING_RATE);
    }

    //I2C *i2cPort = new I2C(I2C_SDA, I2C_SDA);
    
    
    

}

void button(){
    //https://os.mbed.com/users/4180_1/notebook/pushbuttons/
    DigitalIn but (PTA0);
    but.mode(PullUp);

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