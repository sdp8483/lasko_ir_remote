#include "clock.h"

void clock_init(void) {
    #if (F_CPU == 20000000UL)
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0x00); /* no division on clock */
    #elif (F_CPU == 10000000UL)
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, (CLKCTRL_PDIV_2X_gc | CLKCTRL_PEN_bm));
    #elif (F_CPU == 5000000UL)
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, (CLKCTRL_PDIV_4X_gc | CLKCTRL_PEN_bm));
    #elif (F_CPU == 2500000UL)
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, (CLKCTRL_PDIV_8X_gc | CLKCTRL_PEN_bm));
    #elif (F_CPU == 1250000UL)
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, (CLKCTRL_PDIV_16X_gc | CLKCTRL_PEN_bm));
    #elif (F_CPU == 625000UL)
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, (CLKCTRL_PDIV_32X_gc | CLKCTRL_PEN_bm));
    #elif (F_CPU == 312500UL)
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, (CLKCTRL_PDIV_64X_gc | CLKCTRL_PEN_bm));
    #elif (F_CPU == 3333333UL)
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, (CLKCTRL_PDIV_6X_gc | CLKCTRL_PEN_bm));
    #elif (F_CPU == 2000000UL)
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, (CLKCTRL_PDIV_10X_gc | CLKCTRL_PEN_bm));
    #elif (F_CPU == 1666666UL)
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, (CLKCTRL_PDIV_12X_gc | CLKCTRL_PEN_bm));
    #elif (F_CPU == 833333UL)
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, (CLKCTRL_PDIV_24X_gc | CLKCTRL_PEN_bm));
    #elif (F_CPU == 416666UL)
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, (CLKCTRL_PDIV_2X_gc | CLKCTRL_PEN_bm));
    #else
        #ifndef F_CPU
            #error "F_CPU not defined"
        #else
            #error "F_CPU defined as an unsupported value for untuned internal oscillator"
        #endif
    #endif
}