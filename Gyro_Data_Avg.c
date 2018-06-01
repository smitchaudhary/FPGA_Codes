/*
 * main.c
 *
 *  Created on: 29-May-2018
 *      Author: Nachiket
 */

#include "PmodGYRO.h"
#include "xparameters.h"
#include "stdio.h"
#include "xtime_l.h"
#include "myGYRO.h"

int main(){
	PmodGYRO gyro;
	GYRO_begin(&gyro, XPAR_PMODGYRO_0_AXI_LITE_SPI_BASEADDR, XPAR_PMODGYRO_0_AXI_LITE_GPIO_BASEADDR);

	float avgX = 0, avgY = 0, avgZ = 0;
	int readings = 300;
	for(int i = 0; i < readings; i++){
		printf("Initializing.....\n\r");
		printf("\x1B[2J");
		printf("\x1B[H");
		for(int j = 0; j < 1000000; j++);
		avgX = avgX + (double)myGYRO_twosComplement(GYRO_getX(&gyro)); //The functions are defined in myGyro library
		avgY = avgY + (double)myGYRO_twosComplement(GYRO_getY(&gyro));
		avgZ = avgZ + (double)myGYRO_twosComplement(GYRO_getZ(&gyro));
	}
	avgX = avgX/readings;
	avgY = avgY/readings;
	avgZ = avgZ/readings;

	XTime prev, curr;
	double wX = 0, wY = 0, wZ = 0, prevTime = 0, currTime = 0, deltaT = 0;

	mat3 rotMat;
	myGYRO_setIdentity(&rotMat);

	XTime_GetTime(&prev);
	prevTime = ((double) prev)/((double) COUNTS_PER_SECOND);

	while(1){
		XTime_GetTime(&curr);
		currTime = ((double) curr)/((double) COUNTS_PER_SECOND);

		wX = (myGYRO_twosComplement(GYRO_getX(&gyro)) - avgX)*0.00875; //Multiplying by 0.00875 because of the sensitivity that's set at 250dps.
		wY = (myGYRO_twosComplement(GYRO_getY(&gyro)) - avgY)*0.00875; //For different sensitivities, will have to multiply by different numbers.
		wZ = (myGYRO_twosComplement(GYRO_getZ(&gyro)) - avgZ)*0.00875;

		mat3 tempX, tempY, tempZ, temp1, temp2, tempM;
		deltaT = currTime - prevTime;
		myGYRO_RotX(myGYRO_degToRad(wX*deltaT), &tempX); //Function defined in library
		myGYRO_RotY(myGYRO_degToRad(wY*deltaT), &tempY);
		myGYRO_RotZ(myGYRO_degToRad(wZ*deltaT), &tempZ);
		myGYRO_Multiply(tempX, tempY, &temp1);
		myGYRO_Multiply(temp1, tempZ, &temp2);				// Multiply matrices
		myGYRO_Multiply(temp2, rotMat, &tempM);
		rotMat = tempM;																//Changes Rotation Matrix

		prevTime = currTime;

		printf("Rotation Matrix:\n\r");
		myGYRO_printMat(rotMat);
		printf("Angular Velocity = (%lf,\t%lf,\t%lf)\n\r", wX, wY, wZ);
		printf("\x1B[2J");
		printf("\x1B[H");
	}

	return 0;
}
