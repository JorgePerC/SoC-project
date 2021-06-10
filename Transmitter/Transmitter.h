#include "mbed.h"


class Transmitter {

    public:
        Transmitter();
        //Transmitter (PinName * _MSB, PinName En);
        Transmitter (PinName P4, PinName P3, PinName P2, PinName P1, PinName P0, PinName _En);
        
        //void send(char data);
        void send (int _P4, int _P3, int _P2, int _P1, int _P0);
        void disable_data ();
        void enable_data ();
    private:
        
        DigitalOut* P4;
        DigitalOut* P3;
        DigitalOut* P2;
        DigitalOut* P1;
        DigitalOut* P0;
        //DigitalOut * word;
        DigitalOut* En ;
        //int word_length;
};
