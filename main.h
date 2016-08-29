/*
 * main.h
 *
 *  Created on: 14.06.2016
 *      Author: Marvin
 */

#ifndef MAIN_H_
#define MAIN_H_

int test_0,test_1,test_2,test_3;
volatile char received_ch = 0;

enum sensor_type{
	MPU9250,
	LSM9DS1,
	BMX055,
	BMI160,
};



#define CS_0 	BIT5
#define CS_1	BIT6
#define CS_2	BIT7

int sensor;
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

void InitSPI();
void Init_MPU9250();
void Init_LSM9DS1();
int Read_Temp();
void Read_Accelorameter(int * destination);
void Read_Gyroscope(int * destination);
void Read_Magnetometer(int * destination);
int Read_Magnetometer_Id();
void Setup_Wake_on_Motion_Interrupt();
char SPI_Transceive(char cs_signal,char reg,char data);
void SPI_Write (char cs_signal,char reg, char data);
char SPI_Read (char cs_signal,char reg);

#endif /* MAIN_H_ */
