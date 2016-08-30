#include <msp430.h> 
#include <main.h>
#include <MPU9250.h>
#include <LSM9DS1.h>
#include <BMX055.h>



int main(void) {

	sensor=BMX055;

	InitSPI();

	switch (sensor) {
		case MPU9250:
		{
			Init_MPU9250();
			Setup_Wake_on_Motion_Interrupt();
			break;
		}
		case LSM9DS1:
		{
			Init_LSM9DS1();
			break;
		}
		case BMX055:
		{
			Init_BMX055();
			break;
		}
		default:
			break;
	}



	while(1)
	{

		switch (sensor) {
				case MPU9250:
				{
					whoami= SPI_Read(MPU9250_AGM,MPUREG_WHOAMI);				//should be 0x71
					Init_MPU9250();
					Read_Accelorameter(accelorameter_raw);
					ax=accelorameter_raw[0]*aRes;
					ay=accelorameter_raw[1]*aRes;
					az=accelorameter_raw[2]*aRes;
					Read_Gyroscope(gyroscope_raw);
					gx=gyroscope_raw[0]*gRes;
					gy=gyroscope_raw[1]*gRes;
					gz=gyroscope_raw[2]*gRes;
					Read_Magnetometer(magnetometer_raw);
					mx=magnetometer_raw[0]*mRes;
					my=magnetometer_raw[1]*mRes;
					mz=magnetometer_raw[2]*mRes;
					temperature = ((float)Read_Temp()/333.87+ 21.0);
					break;
				}
				case LSM9DS1:
				{
					whoami=SPI_Read(LSM9DS1_M,0x0F);								//0x3D
					Read_Accelorameter(accelorameter_raw);
					ax=accelorameter_raw[0]*aRes;
					ay=accelorameter_raw[1]*aRes;
					az=accelorameter_raw[2]*aRes;
					Read_Gyroscope(gyroscope_raw);
					gx=gyroscope_raw[0]*gRes;
					gy=gyroscope_raw[1]*gRes;
					gz=gyroscope_raw[2]*gRes;
					Read_Magnetometer(magnetometer_raw);
					mx=magnetometer_raw[0]*0.58;
					my=magnetometer_raw[1]*0.58;
					mz=magnetometer_raw[2]*0.58;
					temperature = ((float)Read_Temp()/16+ 25.0);
					break;
				}
				case BMX055:
				{
					whoami=SPI_Read(BMX055_A,0x00);								//0xFA
					Read_Accelorameter(accelorameter_raw);
					ax=accelorameter_raw[0]*aRes;
					ay=accelorameter_raw[1]*aRes;
					az=accelorameter_raw[2]*aRes;
					/*Read_Gyroscope(gyroscope_raw);
					gx=gyroscope_raw[0]*gRes;
					gy=gyroscope_raw[1]*gRes;
					gz=gyroscope_raw[2]*gRes;
					Read_Magnetometer(magnetometer_raw);
					mx=magnetometer_raw[0]*0.58;
					my=magnetometer_raw[1]*0.58;
					mz=magnetometer_raw[2]*0.58;
					temperature = ((float)Read_Temp()/16+ 25.0);*/
					break;
				}
				default:
					break;
			}

	//-------BMX055------------------------------------------------------------------
		//whoami=SPI_Read(CS_0,0x00);							//0xFA
	//-------BMI160------------------------------------------------------------------
		//whoami=SPI_Read(CS_0,0x00);							//0xD1


	}
}


void InitSPI(){
	//--------------Init SPI ----------------------------------------------------------------------

		// Sensor			MPU9250			LSM9DS1			BMX055			BMI160
		//Port 1.7	CS											M
		//Port 1.6	CS							M				G
		//Port 1.5  CS		AGM					AG				A
		//Port 1.4	CLK
		//Port 1.3  Int
		//Port 1.2	MOSI
		//Port 1.1	MISO

		WDTCTL = WDTPW + WDTHOLD; 								// Stop WDT

		P1OUT |= BIT5 + BIT6 + BIT7;							//Port 1.5,1.6,1.7 as High
		P1DIR |= BIT5 + BIT6 + BIT7;							//Port 1.5,1.6,1.7 as Output
		P1SEL = BIT1 | BIT2 | BIT4;								//Port Bit 1,2,4 as SPI Interface
		P1SEL2 = BIT1 | BIT2 | BIT4;							//Port Bit 1,2,4 as SPI Interfasce

		UCA0CTL1 = UCSWRST;
		UCA0CTL0 |= /*UCCKPH +*/ UCCKPL + UCMSB + UCMST + UCSYNC; 	// 3-pin, 8-bit SPI master
		UCA0CTL1 |= UCSSEL_2; 									// SMCLK
		UCA0BR0 |= 0x02; 										// /2
		UCA0BR1 = 0; 											//
		UCA0MCTL = 0; 											// No modulation
		UCA0CTL1 &= ~UCSWRST; 									// **Initialize USCI state machine**


		//P1DIR |= BIT6;											//LED2 as OUtput
		//P1OUT &= ~BIT6;											//LED2 as off
		P1IE |= BIT3;											//P1.3 Interrupt enabled
		P1IES &= ~BIT3;											//Interrupt direction from low to high
		P1IFG &= ~BIT3;											//P1.3 IFG is cleared
		//__enable_interrupt();

	//----------------Init SPI End----------------------------------------------------------------------

		/*
		 * SPI  INTERFACE
		 *
		 * 		MSB	|	6	|	5	|	4	|	3	|	2	|	1	|	LSB
		 * 	------------------------------------------------------------------
		 * 		R/W	|	A6	|	A5	|	A4	|	A3	|	A2	|	A1	|	A0
		 *
		 *
		 * 		MSB	|	6	|	5	|	4	|	3	|	2	|	1	|	LSB
		 * 	------------------------------------------------------------------
		 * 		D7	|	D6	|	D5	|	D4	|	D3	|	D2	|	D1	|	D0
		 *
		 * 		Read	1
		 * 		Write	0
		 * 		A		Address
		 * 		D		Data
		 * 		*/
}


void Init_MPU9250(){

	SPI_Write(MPU9250_AGM,MPUREG_PWR_MGMT_1,BIT_H_RESET);			//RESET_All

	_delay_cycles(120000);								//100ms delay

	SPI_Write(MPU9250_AGM,MPUREG_PWR_MGMT_1,0x00);					//CYCLE =1 -> Clock Source

	_delay_cycles(120000);								//100ms delay

	SPI_Write(MPU9250_AGM,MPUREG_PWR_MGMT_2,0x00);					//Enable ACC,Gyro

	SPI_Write(MPU9250_AGM,MPUREG_CONFIG,0x03);						//

	SPI_Write(MPU9250_AGM,MPUREG_GYRO_CONFIG,0x18);					//Gyro


	SPI_Write(MPU9250_AGM,MPUREG_ACCEL_CONFIG,0x08);				//Accel -> 4G Range


	SPI_Write(MPU9250_AGM,MPUREG_ACCEL_CONFIG_2,0x09);				//Accel 2


	SPI_Write(MPU9250_AGM,MPUREG_INT_PIN_CFG,0x22);					//int




	SPI_Write(MPU9250_AGM,MPUREG_USER_CTRL,0x20);					//user -> Enable fifo operation mode

	SPI_Write(MPU9250_AGM,MPUREG_I2C_MST_CTRL,0x0d);				//ctrl -> MPU-9250 clock divider for i2C -> /20 -> 400kHz

	SPI_Write(MPU9250_AGM,MPUREG_I2C_SLV0_ADDR,AK8963_I2C_ADDR);	//addr -> Write AK8963 I2C Address into Register for I2C Communication with Slave

	SPI_Write(MPU9250_AGM,MPUREG_I2C_SLV0_REG,AK8963_CNTL2);		//reg -> I2C slave register address from where to begin data transfer

	SPI_Write(MPU9250_AGM,MPUREG_I2C_SLV0_DO,0x01);					//do -> Data to be written if I2C Slave 0 enabled -> Reset magnetometer

	SPI_Write(MPU9250_AGM,MPUREG_I2C_SLV0_CTRL,0x81);				//ctrl ->

	SPI_Write(MPU9250_AGM,MPUREG_I2C_SLV0_REG,AK8963_CNTL1);		//reg

	SPI_Write(MPU9250_AGM,MPUREG_I2C_SLV0_DO,0x12);					//do	-> Magnetometer continous Measurement 16 bit

	SPI_Write(MPU9250_AGM,MPUREG_I2C_SLV0_CTRL,0x81);				//ctrl

	MagID= Read_Magnetometer_Id();

}

void Init_LSM9DS1(){

	SPI_Write(LSM9DS1_AG,CTRL_REG8,0x01);		//Reset A&G Device
	__delay_cycles(100000);

	SPI_Write(LSM9DS1_AG,CTRL_REG1_G,((G_ODR_952<<5)+(G_SCALE_2000DPS<<3)));	//Gyroscope Setup
	SPI_Write(LSM9DS1_AG,CTRL_REG6_XL,((XL_ODR_952<<5)+(A_SCALE_4G<<3)));		//Accelorameter Setup
	SPI_Write(LSM9DS1_M,CTRL_REG3_M,0x80);										//Mag i2C disable + Contionus mode
	SPI_Write(LSM9DS1_M,CTRL_REG1_M,0x7C);										//Magnetometer UHP-Mode+80HZ ODR
	SPI_Write(LSM9DS1_M,CTRL_REG2_M,(M_SCALE_16GS<<5));

}

void Init_BMX055(){

	SPI_Write(BMX055_A,BMX055_ACC_BGW_SOFTRESET,0xB6);
	SPI_Write(BMX055_G,BMX055_GYRO_BGW_SOFTRESET,0xB6);
	SPI_Write(BMX055_M,BMX055_MAG_PWR_CNTL1,0x82);
	__delay_cycles(100000);
	SPI_Write(BMX055_A,BMX055_ACC_PMU_RANGE,AFS_4G);			//Set ACC
	SPI_Write(BMX055_A,BMX055_ACC_PMU_BW,ABW_8Hz);


}

int Read_Temp(){

	int rawData[2];

	switch (sensor) {
		case MPU9250:
		{
			rawData[0]=(int)SPI_Read(MPU9250_AGM,MPUREG_TEMP_OUT_H);
			rawData[1]=(int)SPI_Read(MPU9250_AGM,MPUREG_TEMP_OUT_L);
			break;
		}
		case LSM9DS1:
		{
			rawData[0]=(int)SPI_Read(LSM9DS1_AG,OUT_TEMP_H);
			rawData[1]=(int)SPI_Read(LSM9DS1_AG,OUT_TEMP_L);
			break;
		}

		default:
			break;
	}


	return ((rawData[0]<<8)|rawData[1]);
}

void Read_Accelorameter(int * destination){

	int rawData[6];

	switch (sensor) {
		case MPU9250:
		{	rawData[0]=(int)SPI_Read(MPU9250_AGM,MPUREG_ACCEL_XOUT_H);
			rawData[1]=(int)SPI_Read(MPU9250_AGM,MPUREG_ACCEL_XOUT_L);
			rawData[2]=(int)SPI_Read(MPU9250_AGM,MPUREG_ACCEL_YOUT_H);
			rawData[3]=(int)SPI_Read(MPU9250_AGM,MPUREG_ACCEL_YOUT_L);
			rawData[4]=(int)SPI_Read(MPU9250_AGM,MPUREG_ACCEL_ZOUT_H);
			rawData[5]=(int)SPI_Read(MPU9250_AGM,MPUREG_ACCEL_ZOUT_L);
			break;
		}
		case LSM9DS1:
		{
			rawData[0]=(int)SPI_Read(LSM9DS1_AG,OUT_X_H_XL);
			rawData[1]=(int)SPI_Read(LSM9DS1_AG,OUT_X_L_XL);
			rawData[2]=(int)SPI_Read(LSM9DS1_AG,OUT_Y_H_XL);
			rawData[3]=(int)SPI_Read(LSM9DS1_AG,OUT_Y_L_XL);
			rawData[4]=(int)SPI_Read(LSM9DS1_AG,OUT_Z_H_XL);
			rawData[5]=(int)SPI_Read(LSM9DS1_AG,OUT_Z_L_XL);
			break;
		}
		case BMX055:
		{
			rawData[0]=(int)SPI_Read(BMX055_A,BMX055_ACC_D_X_MSB);
			rawData[1]=(int)SPI_Read(BMX055_A,BMX055_ACC_D_X_LSB);
			rawData[2]=(int)SPI_Read(BMX055_A,BMX055_ACC_D_Y_MSB);
			rawData[3]=(int)SPI_Read(BMX055_A,BMX055_ACC_D_Y_LSB);
			rawData[4]=(int)SPI_Read(BMX055_A,BMX055_ACC_D_Z_MSB);
			rawData[5]=(int)SPI_Read(BMX055_A,BMX055_ACC_D_Z_LSB);
			break;
		}
		default:
			break;
	}

	destination[0]=(rawData[0]<<8)|rawData[1];
	destination[1]=(rawData[2]<<8)|rawData[3];
	destination[2]=(rawData[4]<<8)|rawData[5];
}

void Read_Gyroscope(int * destination){

	int rawData[6];

	switch (sensor) {
		case MPU9250:
		{
			rawData[0]=(int)SPI_Read(MPU9250_AGM,MPUREG_GYRO_XOUT_H);
			rawData[1]=(int)SPI_Read(MPU9250_AGM,MPUREG_GYRO_XOUT_L);
			rawData[2]=(int)SPI_Read(MPU9250_AGM,MPUREG_GYRO_YOUT_H);
			rawData[3]=(int)SPI_Read(MPU9250_AGM,MPUREG_GYRO_YOUT_L);
			rawData[4]=(int)SPI_Read(MPU9250_AGM,MPUREG_GYRO_ZOUT_H);
			rawData[5]=(int)SPI_Read(MPU9250_AGM,MPUREG_GYRO_ZOUT_L);
			break;
		}
		case LSM9DS1:
		{
			rawData[0]=(int)SPI_Read(LSM9DS1_AG,OUT_X_H_G);
			rawData[1]=(int)SPI_Read(LSM9DS1_AG,OUT_X_L_G);
			rawData[2]=(int)SPI_Read(LSM9DS1_AG,OUT_Y_H_G);
			rawData[3]=(int)SPI_Read(LSM9DS1_AG,OUT_Y_L_G);
			rawData[4]=(int)SPI_Read(LSM9DS1_AG,OUT_Z_H_G);
			rawData[5]=(int)SPI_Read(LSM9DS1_AG,OUT_Z_L_G);
			break;
		}
		default:
			break;
	}

	destination[0]=(rawData[0]<<8)|rawData[1];
	destination[1]=(rawData[2]<<8)|rawData[3];
	destination[2]=(rawData[4]<<8)|rawData[5];
}

void Read_Magnetometer(int * destination){


	int rawData[6];

	switch (sensor) {
		case MPU9250:
		{
			SPI_Write(MPU9250_AGM,MPUREG_I2C_SLV0_ADDR,(AK8963_I2C_ADDR|0x80));			//Read from Magnetometer
			SPI_Write(MPU9250_AGM,MPUREG_I2C_SLV0_REG,AK8963_HXL);						//Write Address of to read Mag register
			SPI_Write(MPU9250_AGM,MPUREG_I2C_SLV0_CTRL,0x87);							// 0x8- Read Data from AK8963 0x-7 Read 7 bytes

			rawData[0]=(int)SPI_Read(MPU9250_AGM,MPUREG_EXT_SENS_DATA_01);				//HXH
			rawData[1]=(int)SPI_Read(MPU9250_AGM,MPUREG_EXT_SENS_DATA_00);				//HXL
			rawData[2]=(int)SPI_Read(MPU9250_AGM,MPUREG_EXT_SENS_DATA_03);				//HYH
			rawData[3]=(int)SPI_Read(MPU9250_AGM,MPUREG_EXT_SENS_DATA_02);				//HYL
			rawData[4]=(int)SPI_Read(MPU9250_AGM,MPUREG_EXT_SENS_DATA_05);				//HZH
			rawData[5]=(int)SPI_Read(MPU9250_AGM,MPUREG_EXT_SENS_DATA_04);				//HZL
			break;
		}
		case LSM9DS1:
		{
			rawData[0]=(int)SPI_Read(LSM9DS1_M,OUT_X_H_M);				//HXH
			rawData[1]=(int)SPI_Read(LSM9DS1_M,OUT_X_L_M);				//HXL
			rawData[2]=(int)SPI_Read(LSM9DS1_M,OUT_Y_H_M);				//HYH
			rawData[3]=(int)SPI_Read(LSM9DS1_M,OUT_Y_L_M);				//HYL
			rawData[4]=(int)SPI_Read(LSM9DS1_M,OUT_Z_H_M);				//HZH
			rawData[5]=(int)SPI_Read(LSM9DS1_M,OUT_Z_L_M);				//HZL
			break;
		}
		default:
			break;
	}

	destination[0]=(rawData[0]<<8)|rawData[1];
	destination[1]=(rawData[2]<<8)|rawData[3];
	destination[2]=(rawData[4]<<8)|rawData[5];
}

int Read_Magnetometer_Id(){

	int id;

	_delay_cycles(100);

	SPI_Write(MPU9250_AGM,MPUREG_I2C_SLV0_ADDR,(AK8963_I2C_ADDR|0x80));			//Read from Magnetometer
	SPI_Write(MPU9250_AGM,MPUREG_I2C_SLV0_REG,0x00);				//Write Address of to read Mag register
	SPI_Write(MPU9250_AGM,MPUREG_I2C_SLV0_CTRL,0x81);							// 0x8- Read Data from AK8963 0x-1 Read 1 bytes

	_delay_cycles(100);

	id= SPI_Read(MPU9250_AGM,MPUREG_EXT_SENS_DATA_00);

	return(id);
}

void Setup_Wake_on_Motion_Interrupt(){

	SPI_Write(MPU9250_AGM,MPUREG_PWR_MGMT_1,0x00);			//Powermanagement sleep=0, cycle=0,
	SPI_Write(MPU9250_AGM,MPUREG_PWR_MGMT_2,0x07);			//Disable Gyro
	SPI_Write(MPU9250_AGM,MPUREG_ACCEL_CONFIG_2,0x09);		//Set ACC to 184HZ

	SPI_Write(MPU9250_AGM,MPUREG_MOT_DETECT_CTRL,0xc0);		//Enable Motion Detection Logic
	SPI_Write(MPU9250_AGM,MPUREG_MOT_THR,0x0f);				//Motion Threshold LSB -> 15
	SPI_Write(MPU9250_AGM,MPUREG_LP_ACCEL_ODR,0x04);		//Wake up intervall 3.91HZ
	SPI_Write(MPU9250_AGM,MPUREG_PWR_MGMT_1,0x20);			//Cycle =1;

	SPI_Write(MPU9250_AGM,MPUREG_INT_ENABLE,0x40);			//Enable Motion Interrupt
}


void SPI_Write (char cs_signal,char reg, char data){

	SPI_Transceive(cs_signal,reg,data);
}

char SPI_Read (char cs_signal, char reg){


	return(SPI_Transceive(cs_signal,(reg|0x80),0x55));
}

char SPI_Transceive(char cs_signal,char reg,char data) {


	P1OUT &= (~cs_signal); 							// Pin 1.5 High

	while (!(IFG2 & UCA0TXIFG)); 					// USCI_A0 TX buffer ready?
	UCA0TXBUF = reg; 								// Send variable "reg" over SPI to Slave
	while (!(IFG2 & UCA0RXIFG)); 					// USCI_A0 RX Received?
	received_ch = UCA0RXBUF;						// Store received data

	while (!(IFG2 & UCA0TXIFG)); 					// USCI_A0 TX buffer ready?
	UCA0TXBUF = data; 								// Send variable "data" over SPI to Slave
	while (!(IFG2 & UCA0RXIFG)); 					// USCI_A0 RX Received?
	received_ch = UCA0RXBUF;						// Store received data


	P1OUT |= (cs_signal); 							// Pin 1.5 Low

	_delay_cycles(150);

	return (received_ch);
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){

	P1OUT ^= BIT6;				//Toggle LED
	P1IFG &= ~BIT3;				//Clear Interrupt Flag
	SPI_Read(MPU9250_AGM,MPUREG_INT_STATUS);//Clear INterrupt MPU9250
}
