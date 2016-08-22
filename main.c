#include <msp430.h> 
#include <main.h>
#include <MPU9250.h>

volatile char received_ch = 0;

int main(void) {

//--------------Init SPI ----------------------------------------------------------------------
	//Port 1.5  CS
	//Port 1.4	CLK
	//Port 1.2	MOSI
	//Port 1.1	MISO

	WDTCTL = WDTPW + WDTHOLD; 								// Stop WDT

	P1OUT |= BIT5;											//Port 1.5 High
	P1DIR |= BIT5;											//Port 1.5 as Output
	P1SEL = BIT1 | BIT2 | BIT4;								//Port Bit 1,2,4 as SPI Interface
	P1SEL2 = BIT1 | BIT2 | BIT4;							//Port Bit 1,2,4 as SPI Interfasce

	UCA0CTL1 = UCSWRST;
	UCA0CTL0 |= UCCKPH + UCCKPL + UCMSB + UCMST + UCSYNC; 	// 3-pin, 8-bit SPI master
	UCA0CTL1 |= UCSSEL_2; 									// SMCLK
	UCA0BR0 |= 0x0a; 										// /10
	UCA0BR1 = 0; 											//
	UCA0MCTL = 0; 											// No modulation
	UCA0CTL1 &= ~UCSWRST; 									// **Initialize USCI state machine**

//----------------Init SPI End----------------------------------------------------------------------


/*/----------------Init Wake on Motion MPU9250-------------------------------------------------------
	SPI_Transceive(0x6a);						//Write User Ctrl
	SPI_Transceive(0x10);						//Disable I2C
	SPI_Transceive(0x6b);						//Write PWR-MGMT_1
	SPI_Transceive(0x00);						//CYCLE =0, SLEEP = 0 and STANDBY = 0, 20MHz internal Oszilator
	SPI_Transceive(0x6c);						//Write PWR-MGMT_2
	SPI_Transceive(0x07);						//DIS_XA, DIS_YA, DIS_ZA = 0 and DIS_XG, DIS_YG, DIS_ZG = 1
	SPI_Transceive(0x1D);						//Write ACCEL_Config_2
	SPI_Transceive(0x09);						//ACCEL_FCHOICE_B = 0 and A_DLPFCFG[2:0]=1
	SPI_Transceive(0x38);						//Write INT_Enable
	SPI_Transceive(0x40);						//enable motion interrupt only.
	SPI_Transceive(0x69);						//Write MOT_Detect_CTRL
	SPI_Transceive(0xC0);						//set ACCEL_INTEL_EN = 1 and ACCEL_INTEL_MODE = 1Set
	SPI_Transceive(0x1F);						//Write WOM_THR (motion Threshold)
	SPI_Transceive(0x7F);						//set the WOM_Threshold[7:0] to 1~255 LSBs (0~1020mg) "127"
	SPI_Transceive(0x1E);						//Write LP_ACCEL_ODR
	SPI_Transceive(0x00);						//Lposc_clksel[3:0] = 0.24Hz ~ 500Hz
	SPI_Transceive(0x6b);						//Write PWR_MGMT_1
	SPI_Transceive(0x20);						//CYCLE =1

*///-----------------End Wake on Motion MPU9250----------------------------------------------------------

	SPI_Write(MPUREG_PWR_MGMT_1,BIT_H_RESET);			//RESET_All

	_delay_cycles(120000);								//100ms delay

	SPI_Write(MPUREG_PWR_MGMT_1,0x01);					//CYCLE =1 -> Clock Source

	_delay_cycles(120000);								//100ms delay

	SPI_Write(MPUREG_PWR_MGMT_2,0x00);					//Enable ACC,Gyro

	SPI_Write(MPUREG_CONFIG,0x03);						//

	SPI_Write(MPUREG_GYRO_CONFIG,0x18);					//Gyro


	SPI_Write(MPUREG_ACCEL_CONFIG,0x08);				//Accel


	SPI_Write(MPUREG_ACCEL_CONFIG_2,0x09);				//Accel 2


	SPI_Write(MPUREG_INT_PIN_CFG,0x22);					//int




	SPI_Write(MPUREG_USER_CTRL,0x20);					//user

	SPI_Write(MPUREG_I2C_MST_CTRL,0x0d);				//ctrl

	SPI_Write(MPUREG_I2C_SLV0_ADDR,AK8963_I2C_ADDR);	//addr

	SPI_Write(MPUREG_I2C_SLV0_REG,AK8963_CNTL2);		//reg

	SPI_Write(MPUREG_I2C_SLV0_DO,0x01);					//do

	SPI_Write(MPUREG_I2C_SLV0_CTRL,0x81);				//ctrl

	SPI_Write(MPUREG_I2C_SLV0_REG,AK8963_CNTL1);		//reg

	SPI_Write(MPUREG_I2C_SLV0_DO,0x12);					//do

	SPI_Write(MPUREG_I2C_SLV0_CTRL,0x81);				//ctrl

	while(1)
	{
		SPI_Read(0x6B);
		SPI_Read(MPUREG_WHOAMI);

	}
}

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



void SPI_Write (char reg, char data){

	SPI_Transceive(reg,data);
}

char SPI_Read (char reg){


	return(SPI_Transceive((reg|0x80),0x00));
}

char SPI_Transceive(char reg,char data) {

	P1OUT &= (~BIT5); 								// Pin 1.5 High

	while (!(IFG2 & UCA0TXIFG)); 					// USCI_A0 TX buffer ready?
	UCA0TXBUF = reg; 								// Send variable "data" over SPI to Slave
	while (!(IFG2 & UCA0RXIFG)); 					// USCI_A0 RX Received?
	received_ch = UCA0RXBUF;						// Store received data

	while (!(IFG2 & UCA0TXIFG)); 					// USCI_A0 TX buffer ready?
	UCA0TXBUF = data; 								// Send variable "data" over SPI to Slave
	while (!(IFG2 & UCA0RXIFG)); 					// USCI_A0 RX Received?
	received_ch = UCA0RXBUF;						// Store received data


	P1OUT |= (BIT5); 								// Pin 1.5 Low

	_delay_cycles(250);

	return (received_ch);



}


