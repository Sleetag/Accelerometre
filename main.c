#include <stdio.h>
#include "smb380_drv.h"
#include <unistd.h>
#include "fb_Graphic.h"
#include <time.h>
#include <math.h>


void calibrage (void) {
	pSMB380_Data_t pData;
	unsigned int xres, yres;
	int i = 0, time = 20 ;
	char mess[30] ;

	SMB380_Init();
	
	printf("Calibrage de la position ...\n");
	for(i = 0; i < time; ++i) {
		SMB380_GetData(pData);
		printf("... %d /20 \n", i);
		usleep(1000000);
	}
	printf("AccX = %d\n", pData->AccX);	
	printf("AccY = %d\n", pData->AccY);	
	printf("AccZ = %d\n", pData->AccZ);	

}
int main(void)
{
	printf("coucou\n");
	usleep(100000);
	calibrage();

	
	return 0 ;
}

	
