
#include"config.h"//include the config header file
#include"RTCDRIVER.h"//including the RTCDRIVER header file
#define _XTAL_FREQ 20000000//Define the Oscillator frequency
#include <xc.h>//include the PIC18F877A header file
int main(void) {
    declaration();//calling the declaration function
    LCD_intialize();//Calling the LCD_initalize function
    I2C_intialize(100000);//Setting the Frequency for baurd rate
    newtime();//calling the newtime function
    while(1)
    {
        updated();//Calling the updated function
        datatolcd();//Calling the datatolcd function
        position1();//calling the position1 function
        position2();//calling the position2 function
        __delay_ms(500);
    }
}





