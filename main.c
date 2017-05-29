#include <stdio.h>
#include "smb380_drv.h"
#include <unistd.h>
#include "fb_Graphic.h"
#include <time.h>
#include <math.h>


enum Direction {DROITE = 0, GAUCHE = 1, HAUT = 2, NULLE = -1};

Int16S ACC_X, ACC_Y, ACC_Z; 
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
	
	ACC_X = pData->AccX;
	ACC_Y = pData->AccY;
	ACC_Z = pData->AccZ;	

}

enum Direction detection(pSMB380_Data_t pData, Int16S seuil_accept_x, Int16S seuil_accept_y) {
	enum Direction d; 

	if(pData->AccX < ACC_X-seuil_accept_x)
		d = GAUCHE;

	else if(pData->AccX > ACC_X+seuil_accept_x)
		d = DROITE;

	else if(pData->AccY > ACC_Y+seuil_accept_y)
		d = HAUT;

	else
		d = NULLE;  
	
	return d; 		

	
}
int main(void){
	//printf("coucou\n");


	pSMB380_Data_t pData;
	enum Direction d; 
	unsigned int xres, yres;
	int i = 0, time = 20 ;
	char mess[30] ;

	usleep(100000);
	calibrage();

	SMB380_Init();

	while(1) {
		SMB380_GetData(pData);
		d = detection(pData, 8, 16);
		printf("Direction : %d", (int)d); 
		usleep(1000*1000);
	}

	
	return 0 ;
}
	
