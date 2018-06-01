/*
 * main.c
 *
 *  Created on: 27-May-2018
 *      Author: Nachiket
 */

#include "stdio.h"
#include "xparameters.h"
#include "PmodACL2.h"
#include "xtime_l.h"


int main(){
	PmodACL2 acl;
	ACL2_begin(&acl, XPAR_PMODACL2_0_AXI_LITE_GPIO_BASEADDR, XPAR_PMODACL2_0_AXI_LITE_SPI_BASEADDR);
	ACL2_configure(&acl);

	XTime prev, curr;
	float prevTime = 0, currTime = 0;
	float prevAcl = 0, currAcl = 0, vel = 0;

	XTime_GetTime(&prev);
	prevTime = ((float) prev)/((float) COUNTS_PER_SECOND);
	float initial = 0;

	for(int i = 0; i < 1000; i++){
		for(int j = 0; j<1000000; j++);
		initial = initial + ACL2_DataToG(&acl, ACL2_getData(&acl, ACL2_XDATA_L_REG));
	}
	initial = initial/1000;

	while(1){
		XTime_GetTime(&curr);
		currTime = ((float) curr)/((float) COUNTS_PER_SECOND);
		currAcl = ACL2_DataToG(&acl, ACL2_getData(&acl, ACL2_XDATA_L_REG)) - initial;
		vel = vel + 0.5*(currTime - prevTime)*(currAcl + prevAcl);
		prevTime = currTime;
		prevAcl = currAcl;
		printf("Vel: %f Acl: %f\n", vel, currAcl);
		for(int i = 0; i<1000000; i++);
	}

	return 0;
}
