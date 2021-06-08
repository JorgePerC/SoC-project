#include "mbed.h"
#include <Ticker.h>
#include <mbed_chrono.h>

namespace mbed {

    class Buzzer {  
        public:
            Buzzer (PinName p);

            void makeBeep(float freq, std::chrono::microseconds time);

            void silence();

        private:
            PwmOut pulse;

    };

}