#include<TurboAnal.h>
uint16_t TurboAnal::lastMeasurement = 0;
uint8_t TurboAnal::flag = 0;
void (*TurboAnal::interruptFunction)() = NULL;
//562 ns + 843 ns cpu sekunder

//5900 ns med en prescaler //vores er 4.2 gange så hurtig

//110 000 ns cpu sekunder med rigtig prescaler // vores er 78x så hurtig. eller den bruger 1.2% så mange cpu sekunder




//https://bytes.usc.edu/files/ee109/labs/lab5/ADC.pdf


void TurboAnal::Setup(uint8_t pin, void (*ISRFunc)()){
    
    /*[TODO]:
    FIX så den kan starte på timer regs. side 220 ATMEGA328P. kan give gøre så funktionen kun skal bruge 0.7%
    */
    cli(); //gør vi ikke kan interruptes
    PRR &= ~(1 << PRADC); //slår adc'en til i Power reduction registeret 
    ACSR &= ~ (1 << ACD); //tænder Comparatoren (hvis den er i sleepmode (side 203 ATMEGA28P datasheet))
    ADCSRA |= (1 << ADEN); //tænder ADC igen?? (side 218 ATMEGA28P datasheet))
    
    ADMUX &= B00110000; //sætter ADC'en til 0 så der kan vælges en write
    ADMUX |= B01000000; //sætter voltage reference (s 217)
    ADMUX |= pin & B00001111; //sætter pin der lyttes på
    
    ADCSRA |= (1 << ADIE); //Slår interrupts til for ADC'en
    interruptFunction = ISRFunc;
    sei(); //gør vi kan interruptes igen
    //ADCSRA &= B11111000; //Sætter prescaler så clocken bare køre fuld gadaffi (side 219 AMEGA28P datasheet)
}


void TurboAnal::Setup_timer(uint8_t pin, void (*ISRFunc)()){
    Setup(pin, ISRFunc);
    ADCSRB |= B11111000; //sætter til free runnning (side 220, 207)
    ADCSRA |= (1 << ADATE);
    interruptFunction = ISRFunc;
}

void TurboAnal::Measure(){
    ADCSRA |= (1 << ADSC); //Starter konvertering
}

ISR(ADC_vect){
    //gør ingenting so far
    TurboAnal::lastMeasurement = ADC;
    TurboAnal::flag += 1;
    if(TurboAnal::interruptFunction != NULL){
        (*TurboAnal::interruptFunction)(); //den funktion der skal kaldes.
    }
}