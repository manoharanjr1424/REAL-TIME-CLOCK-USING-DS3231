/*
 * File:   RTCDRIVER.c
 * Author: jayka
 *
 * Created on 17 October, 2023, 6:52 PM
 */


#include <xc.h>
#include"RTCDRIVER.h"//Include the RTCDRIVER headerfile
#define _XTAL_FREQ 20000000 //intialize the oscilator frequency
#define RS RC1//Define the RS variable to the RC1
#define EN RC2//Define the RS variable to the RC2
char hour1,hour2,sec1,sec2,min1,min2,date1,date2,var1;//Declare the variables 
char year1,year2,month1,month2;//Declare the variable 
int hour=11,min=59,sec=55,date=18,month=10,year=23;//declaring the time and date
unsigned char tim[10]={"TIME:"};//Define the Compile time Array for time 
unsigned char dat[10]={"DATE:"};//Define the Compile time Array for date
unsigned char am[2]={"AM"},pm[2]={"PM"};//Define the Compile time Array for AM and PM


void declaration()//Definition for the declaration
{
    TRISD=0x00;//Setting the direction for the PORTD 0000 0000
    TRISC=0x18;//Setting the Direction for the PORTC 0001 1000
    PORTD=0x00;//Setting the PORTD as 0000 0000
    PORTC=0x00;//Setting the PORTC as 0000 0000
}

void LCD_intialize()//Definition for the LCD_initialize function
{
  LCD_command(0x30);//Setting the LCD in 8 Mode
      __delay_ms(100);
      LCD_command(0x30);//Setting the LCD in 8 Mode
      __delay_ms(100);
      LCD_command(0x30);//Setting the LCD in 8 Mode
      __delay_ms(100);
      LCD_command(0x38);//Clearing the RAM of the LCD
      __delay_ms(100);
      LCD_command(0x0C);//Turn On the LCD and cursor Off
      __delay_ms(100);
      LCD_command(0x06);//Display the From right to left 
      __delay_ms(100);
      LCD_command(0x01);//clear display
      __delay_ms(100);
}


void LCD_data(unsigned char a)//definition for LCD_data function
{
    RS=1;//Enable the Register select pin
    PORTD=a;//Assign the data to the PORTD
    EN=1;//Enable the Enable pin
    __delay_ms(10);
    EN=0;//Disable the Enable pin
}

void LCD_command(unsigned char b)
{
    RS=0;//Disable the Register Select pin
    PORTD=b;//Assign the data to the PORTD
    EN=1;//Enable the Enable pin
    __delay_ms(10);
    EN=0;//Disable the Enable pin
}
void I2C_intialize(const unsigned  long freq)
{
     SSPSTAT=0x00;//Intialize the 0000 0000 to the SSPSTAT Register
     SSPCON=0x28;//Intialize the 0010 1000 to the SSPCON Register
     SSPCON2=0x00;//Intialize the 0000 0000 to the SSPCON2 Register
     SSPADD = (_XTAL_FREQ/(4*freq*100))-1;//Setting up the baurd Rate
    
}
void I2C_wait()//Definition for the i2c_wait function
{
    while(SSPCON2 & 0x1F || SSPSTAT & 0x04);/*Checking If any operation
                                             * in the communication
                                             * SSPCON2 and with 0001 1111
                                             * SSPSTAT and with 0000 0100
                                              */
}
void I2C_start()//Definition for the i2c_start function
{
    I2C_wait();//Waiting and checking any operation are going in the bus line
    SSPCON2|=0x01;//Enable the SEN for start bit 
}

void I2C_repeatedstart()//Definition for the i2c_repeatedstart function
{
    I2C_wait();//Waiting and checking any operation are going in the bus line
    SSPCON2|=0x02;/*Enable the RSEN pin for Initiate Repeated 
                   * Start condition on SDA and SCL pins*/
}
void I2C_stop()//Definition for the i2c_stop function
{
    I2C_wait();//Waiting and checking any operation are going in the bus line
    SSPCON2|=0x04;/*Enable the PEN pin for Initiate 
                   * Stop condition on SDA and SCL pins*/
}
void I2C_write(unsigned char temp)/*Declaration the i2c_write function*/
{
    I2C_wait();//Waiting and checking any operation are going in the bus line
    SSPBUF=temp;//Assign the data into the SSPBUF buffer 
}
int I2C_read(int ack)//Declaration i2c_read function
{
    I2C_wait();//Waiting and checking any operation are going in the bus line
    SSPCON2|=0x08;//Setting master into the Rx mode 
    int value=SSPBUF; /*data whatever present in the
                       *  SSPBUF buffer assign into value 
                       * variable*/
    I2C_wait();//Waiting and checking any operation are going in the bus line
    ACKDT=(ack)?0:1;//Checking the ack 
    ACKEN=1;//send the ack 
    return value;//return the data contain on the value variable 
}
int DECtoBCD(int value)//definition for the DECtoBCD funnction
{ 
return ((value/10)<<4)+(value%10);/*divide the value with 10 and 
                                   * seaperate the MSB and left shit 
                                   * that to assign to the MSB
                                   * 
                                   *taking module the data contain on the value 
                                   * variable for get reminder and take LSB*/
}
int BCDtoDEC(int value1)//definition for the BCDtoDEC function
{ 
return ((value1>>4)*10)+(value1&0x0F);/*Right shift the data contain 
                                       * on the value1 variable to 4 times
                                       * and during AND operation with value 
                                       * 0000 1111 with data contain on the 
                                       * value 1 variable */
}

void newtime()//Define the for the newtime function
{
    I2C_start();//calling the start function
    I2C_write(0xD0);//write the Address of the DS3232 
    I2C_write(0);//set 0 for write function
    I2C_write(DECtoBCD(sec));/*write the sec to the DS3232
                              * After convert the decimal value 
                              * into the binary code 
                              * */
    I2C_write(DECtoBCD(min));/*write the minute to the DS3232
                              * After convert the decimal value 
                              * into the binary code 
                              * */
    I2C_write(0x60|(DECtoBCD(hour)));/*write the Hour to the DS3232
                              * After convert the decimal value 
                              * into the binary code 
                              * */
    I2C_write(1);
    I2C_write(DECtoBCD(date));/*write the date to the DS3232
                              * After convert the decimal value 
                              * into the binary code 
                              * */
    I2C_write(DECtoBCD(month));/*write the Month to the DS3232
                              * After convert the decimal value 
                              * into the binary code 
                              * */
    I2C_write(DECtoBCD(year));/*write the Year to the DS3232
                              * After convert the decimal value 
                              * into the binary code 
                              * */
    I2C_stop();//calling the stop function
}
void updated()//Definition for the updated function
{
    I2C_start();//calling the start function
    I2C_write(0xD0);//Write the DS3232 Address 
    I2C_write(0);//set mode as a Write for write the address
    I2C_repeatedstart();//calling the repeat start for Recieve 
    I2C_write(0xD1);//Write the Address with the read bit 
    sec=(BCDtoDEC(I2C_read(1)));/*read the data from the seconds
                                 * Address and convert that BCD 
                                 * to the DEC and store into the 
                                 * sec variable*/
    min=(BCDtoDEC(I2C_read(1)));/*read the data from the Minute
                                 * Address and convert that BCD 
                                 * to the DEC and store into the 
                                 * minute variable*/
    var1=(I2C_read(1));/*Receive the data from the hour address of the DS3232
                        * module and AM and PM */
    hour=(BCDtoDEC(0x1F &var1));/*store the hour value to the hour variable
                                 * */
    (I2C_read(1));
    date=(BCDtoDEC(I2C_read(1)));/*read the data from the Date
                                 * Address and convert that BCD 
                                 * to the DEC and store into the 
                                 * data variable*/
     month=(BCDtoDEC(I2C_read(1)));/*read the data from the Month
                                 * Address and convert that BCD 
                                 * to the DEC and store into the 
                                 * month variable*/
      year=(BCDtoDEC(I2C_read(0)));/*read the data from the Year
                                 * Address and convert that BCD 
                                 * to the DEC and store into the 
                                 * Year variable*/
      I2C_stop();//calling the function
}
void datatolcd()//Definition for the datatolcd function
{
    sec1=(sec/10);//Seperate  the MSB from the original data
    sec2=(sec%10);//Seperate  the LSB from the original data
    min1=(min/10);//Seperate  the MSB from the original data
    min2=(min%10);//Seperate  the LSB from the original data
    hour1=(hour/10);//Seperate  the MSB from the original data
    hour2=(hour%10);//Seperate  the LSB from the original data
    date1=(date/10);//Seperate  the MSB from the original data
    date2=(date%10);//Seperate  the LSB from the original data
    month1=(month/10);//Seperate  the MSB from the original data
    month2=(month%10);//Seperate  the LSB from the original data
    year1=(year/1000);//Seperate  the MSB from the original data
    year2=(year%1000);//Seperate  the LSB from the original data
}
void position1()//Definition for the position1 function
{
      LCD_command(0x80);//Assign the position for the string
    for(int i=0;i<5;i++)
    {
        LCD_data(tim[i]);//print the "TIME" on the display
    }
    LCD_data(0x30+hour1);//print hour on the LCD
    LCD_data(0x30+hour2);//print hour on the LCD
    LCD_data(0x3a);
    LCD_data(0x30+min1);//print minute on the LCD
    LCD_data(0x30+min2);//print minute on the LCD
    LCD_data(0x3a);
    LCD_data(0x30+sec1);//print Seconds on the LCD
    LCD_data(0x30+sec2);//print Seconds on the LCD
             if(var1&0x20)//Checking the AM or PM
        {
                    for(int i=0;i<2;i++)
                    {
                        LCD_data(pm[i]);
                    }
        }
                else
                {
                    for(int i=0;i<2;i++)
                    {
                        LCD_data(am[i]);
                    }
                }
    
}

void position2()//Assign the position for the string
{
      LCD_command(0xC0);//Assign the position for the string
    for(int i=0;i<5;i++)
    {
        LCD_data(dat[i]);
    }
    LCD_data((0x30+date1));//print date on the LCD
    LCD_data((0x30+date2));//print date on the LCD
    LCD_data(0x2f);
    LCD_data((0x30+month1));//print month on the LCD
    LCD_data((0x30+month2));//print month on the LCD
    LCD_data(0x2f);
    LCD_data((0x30+year1));//print Year on the LCD
    LCD_data((0x30+year2));//print Year on the LCD
    
}
