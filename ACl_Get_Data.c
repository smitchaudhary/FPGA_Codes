/*
 * main.c
 *
 *  Created on: May 23, 2018
 *      Author: smitc
 */



#include "PmodACL2.h"
#include "stdio.h"
#include "xparameters.h"

int main(){
	PmodACL2 acl;
	ACL2_begin(&acl, XPAR_PMODACL2_0_AXI_LITE_GPIO_BASEADDR, XPAR_PMODACL2_0_AXI_LITE_SPI_BASEADDR);
	ACL2_configure(&acl);

	u8 regName = ACL2_XDATA_L_REG;
	int initial = ACL2_getData(&acl, regName);

	while(1){
		printf("%f\n", ACL2_DataToG(&acl, ACL2_getData(&acl, regName)-initial));
		for(int i = 0; i<1000000; i++);
	}
	return 0;
}
