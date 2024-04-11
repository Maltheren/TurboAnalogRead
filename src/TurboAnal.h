#include<Arduino.h>
/*
* @author Malthe
* @brief EN hurtig analogRead for 1 pin pr program, til hurtig sampling.
* 
*/
namespace TurboAnal{
    extern uint16_t lastMeasurement;
    extern uint8_t flag;
    extern void (*interruptFunction)();
    /*
    @brief registene op så der kan foretages en måling
    @param pin Den pin der måles analogt, automatisk
    */
    void Setup(uint8_t pin, void (*ISRfunc)());



    /*
    @brief starter automatiske analoge målinger i det der hedder "free running" hvor han selv starter en ny måling efter den sidste. 
    der skal køres en Measure() for at sparke den igang
    @param pin Den pin der måles analogt, automatisk
    */
    void Setup_timer(uint8_t pin, void (*ISRfunc)());

    /*
    foretager en måling
    */
    void Measure();

};