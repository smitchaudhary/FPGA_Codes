/*
 * main.c
 *
 *  Created on: May 23, 2018
 *      Author: smitc
 */

//

#include "PmodACL2.h"
#include "stdio.h"
#include "xparameters.h"

int main(){
	PmodACL2 acl; //declaring variable acl of PmodACL2
	ACL2_begin(&acl, XPAR_PMODACL2_0_AXI_LITE_GPIO_BASEADDR, XPAR_PMODACL2_0_AXI_LITE_SPI_BASEADDR); //initialising the variable
	ACL2_configure(&acl);

	u8 regName = ACL2_XDATA_L_REG; //declaring and initialising an unsigned 8 bit integer
	int initial = ACL2_getData(&acl, regName); //Getting the initial data and storing it in

	while(1){
		printf("%f\n", ACL2_DataToG(&acl, ACL2_getData(&acl, regName)-initial)); //subtracting the initial data
		for(int i = 0; i<1000000; i++);
	}
	return 0;
}
