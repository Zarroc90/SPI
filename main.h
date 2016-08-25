/*
 * main.h
 *
 *  Created on: 14.06.2016
 *      Author: Marvin
 */

#ifndef MAIN_H_
#define MAIN_H_

int test_0,test_1,test_2,test_3;

int accelorameter_raw[3];
int gyroscope_raw[3];
int magnetometer_raw[3];
int whoami;
int MagID;
float aRes = 4.0/32768.0;
float gRes = 2000.0/32768.0;
float mRes = 10.0 * 4219.0/32760.0;
float ax,ay,az,gx,gy,gz,mx,my,mz;

float temperature;

void Init_MPU9250();
int Read_Temp();
void Read_Accelorameter(int * destination);
void Read_Gyroscope(int * destination);
void Read_Magnetometer(int * destination);
int Read_Magnetometer_Id();
void Setup_Wake_on_Motion_Interrupt();
char SPI_Transceive(char reg,char data);
void SPI_Write (char reg, char data);
char SPI_Read (char reg);

#endif /* MAIN_H_ */
