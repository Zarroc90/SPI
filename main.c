#include <msp430.h> 
#include <main.h>


volatile char received_ch = 0;

int main(void) {

//--------------Init SPI ----------------------------------------------------------------------
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
	SPI_transceive(0x6a);						//Write User Ctrl
	SPI_transceive(0x10);						//Disable I2C
	SPI_transceive(0x6b);						//Write PWR-MGMT_1
	SPI_transceive(0x00);						//CYCLE =0, SLEEP = 0 and STANDBY = 0, 20MHz internal Oszilator
	SPI_transceive(0x6c);						//Write PWR-MGMT_2
	SPI_transceive(0x07);						//DIS_XA, DIS_YA, DIS_ZA = 0 and DIS_XG, DIS_YG, DIS_ZG = 1
	SPI_transceive(0x1D);						//Write ACCEL_Config_2
	SPI_transceive(0x09);						//ACCEL_FCHOICE_B = 0 and A_DLPFCFG[2:0]=1
	SPI_transceive(0x38);						//Write INT_Enable
	SPI_transceive(0x40);						//enable motion interrupt only.
	SPI_transceive(0x69);						//Write MOT_Detect_CTRL
	SPI_transceive(0xC0);						//set ACCEL_INTEL_EN = 1 and ACCEL_INTEL_MODE = 1Set
	SPI_transceive(0x1F);						//Write WOM_THR (motion Threshold)
	SPI_transceive(0x7F);						//set the WOM_Threshold[7:0] to 1~255 LSBs (0~1020mg) "127"
	SPI_transceive(0x1E);						//Write LP_ACCEL_ODR
	SPI_transceive(0x00);						//Lposc_clksel[3:0] = 0.24Hz ~ 500Hz
	SPI_transceive(0x6b);						//Write PWR_MGMT_1
	SPI_transceive(0x20);						//CYCLE =1

*///-----------------End Wake on Motion MPU9250----------------------------------------------------------

	SPI_transceive(0x6b);						//Write PWR_MGMT_1
	SPI_transceive(0x80);						//RESET_All

	_delay_cycles(120000);						//100ms delay

	SPI_transceive(0x6b);						//Write PWR_MGMT_1
	SPI_transceive(0x01);						//CYCLE =1 -> Clock Source

	_delay_cycles(120000);						//100ms delay

	SPI_transceive(0x6c);						//Write PWR-MGMT_2
	SPI_transceive(0x00);						//Enable ACC,Gyro

	SPI_transceive(0x1a);						//Config
	SPI_transceive(0x10);						//

	SPI_transceive(0x1b);						//Gyro
	SPI_transceive(0x18);						//

	SPI_transceive(0x1c);						//Accel
	SPI_transceive(0x08);						//

	SPI_transceive(0x1d);						//Accel 2
	SPI_transceive(0x09);						//

	SPI_transceive(0x37);						//int
	SPI_transceive(0x30);						//



	SPI_transceive(0x6a);						//user
	SPI_transceive(0x20);						//

	SPI_transceive(0x24);						//ctrl
	SPI_transceive(0x0d);						//

	SPI_transceive(0x25);						//addr
	SPI_transceive(0xDC);						//Magnetometer address

	SPI_transceive(0x26);						//reg
	SPI_transceive(0x0b);						//

	SPI_transceive(0x63);						//do
	SPI_transceive(0x01);						//

	SPI_transceive(0x27);						//ctrl
	SPI_transceive(0x81);						//

	SPI_transceive(0x26);						//reg
	SPI_transceive(0x0a);						//

	SPI_transceive(0x63);						//do
	SPI_transceive(0x12);						//

	SPI_transceive(0x27);						//ctrl
	SPI_transceive(0x81);						//

	while(1)
	{
		SPI_transceive(0x9a);
		SPI_transceive(0x00);
		SPI_transceive(0x1a);
		SPI_transceive(0x00);
		/*SPI_transceive(0xbb);					//read high byte of x_Axis Acceloration
		SPI_transceive(0x00);
		SPI_transceive(0xbc);					//read low byte of x_Axis Accelorameter
		SPI_transceive(0x00);*/

	}
}


char SPI_transceive(char data) {

	P1OUT &= (~BIT5); 								// Pin 1.5 High

	while (!(IFG2 & UCA0TXIFG)); 					// USCI_A0 TX buffer ready?
	UCA0TXBUF = data; 								// Send variable "data" over SPI to Slave
	while (!(IFG2 & UCA0RXIFG)); 					// USCI_A0 RX Received?
	received_ch = UCA0RXBUF;						// Store received data

	P1OUT |= (BIT5); 								// Pin 1.5 Low

	_delay_cycles(1000);

}


