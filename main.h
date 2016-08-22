/*
 * main.h
 *
 *  Created on: 14.06.2016
 *      Author: Marvin
 */

#ifndef MAIN_H_
#define MAIN_H_

char SPI_Transceive(char reg,char data);
void SPI_Write (char reg, char data);
char SPI_Read (char reg);

#endif /* MAIN_H_ */
