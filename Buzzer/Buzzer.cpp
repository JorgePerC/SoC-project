#include "Buzzer.h"
#include <chrono>



// constructor         
Buzzer::Buzzer(PinName pin) : pulse(pin) {
    pulse.write(0.0);     // after creating it have to be off       
}

// switch off
void Buzzer::silence() {
    pulse.write(0.0); 
}

// Buzzer
void Buzzer::makeBeep(float freq, std::chrono::microseconds time) {
    Timeout toff;
    pulse.period(1.0/freq);
    pulse.write(0.3);            // 50% duty cycle - Buzzer on
    toff.attach( callback(this, &Buzzer::silence), time);   // time to off
    //ThisThread::sleep_for(50ms);
}

// Buzzer
void Buzzer::makeBeep(float freq) {
    Timeout toff;
    pulse.period(1.0/freq);
    pulse.write(0.3);            // 30% duty cycle - Buzzer on
    wait_us(200);
    silence();
}