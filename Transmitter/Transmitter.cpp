#include "Transmitter.h"

Transmitter::Transmitter (){
    this -> En = (new DigitalOut (PTA0));
    this -> P4 = (new DigitalOut(PTA0));
    this -> P3 = (new DigitalOut(PTA1));
    this -> P2 = (new DigitalOut(PTA2));
    this -> P1 = (new DigitalOut(PTA3));
    this -> P0 = (new DigitalOut(PTA4));
}
/*
Transmitter::Transmitter (PinName * _MSB, PinName _En){
    this -> En = DigitalOut (_En);
    
    
    
    for (int i = 0; i < sizeof(_MSB)/sizeof(PinName); i ++){
        this -> word = new DigitalOut ( (_MSB + i));
        DigitalOut();
    }
  
    // TODO: fix
    this -> MSB = DigitalOut(PTA1);
    this -> LSB = DigitalOut(PTA2);

}
*/

Transmitter::Transmitter (PinName _P4, PinName _P3, PinName _P2, PinName _P1, PinName _P0, PinName _En){
    this -> En = (new DigitalOut (_En));

    this -> P4 = (new DigitalOut(_P4));
    this -> P3 = (new DigitalOut(_P3));
    this -> P2 = (new DigitalOut(_P2));
    this -> P1 = (new DigitalOut(_P1));
    this -> P0 = (new DigitalOut(_P0));

}


void Transmitter::disable_data(){
    this -> En ->write(0);
}

void Transmitter::enable_data(){
    this -> En -> write(1);
}

void Transmitter::send(int _P4, int _P3, int _P2, int _P1, int _P0){
    this -> P4 ->write(_P4);
    this -> P3 ->write(_P3);
    this -> P2 ->write(_P2);
    this -> P1 ->write(_P1);
    this -> P0 ->write(_P0);
    
}