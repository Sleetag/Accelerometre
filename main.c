#include "affichage.h"
#include "smb380_drv.h"

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <linux/fb.h>
#include "hal.h"

/*typedef struct  {
	int fd;
	unsigned char *memp;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
}fb_sess; 
*/

struct fb_sess {
	int fd;
	unsigned char *memp;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
};

struct fb_sess *fb_init(const char *fb_dev_name)
{
	struct fb_sess *fb;
	int st, size;

	fb = malloc(sizeof(*fb));
	if(!fb) abort();

	fb->fd = open(fb_dev_name, O_RDWR);
	if(fb->fd < 0) {
		free(fb);
		fprintf(stderr, "Cannot open framebuffer device file.\n");
		exit(EXIT_FAILURE);
	}

	st = ioctl(fb->fd, FBIOGET_FSCREENINFO, &fb->finfo);
	if(st) {
		perror("ioctl failed");
		close(fb->fd);
		free(fb);
		fprintf(stderr, "Cannot get fixed screen info.\n");
		exit(EXIT_FAILURE);
	}

	st = ioctl(fb->fd, FBIOGET_VSCREENINFO, &fb->vinfo);
	if(st) {
		perror("ioctl failed");
		close(fb->fd);
		free(fb);
		fprintf(stderr, "Cannot get variable screen info.\n");
		exit(EXIT_FAILURE);
	}
	
	size = fb_get_fb_size(fb);
#if CONFIG_MMAP
	fb->memp = mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, fb->fd, 0);
	if(fb->memp == MAP_FAILED) {
		perror("mmap failed");
		close(fb->fd);
		free(fb);
		fprintf(stderr, "Cannot mmap the framebuffer device.\n");
		exit(EXIT_FAILURE);
	}
#else
	fb->memp = malloc(size);
	if(!fb->memp) {
		abort();
	}
#endif

	/* clear display */
	memset(fb->memp, 0, size);
	fb_sync(fb);

	return fb;
}
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
int main(int argc, char * argv[]){
	//printf("coucou\n");
	pSMB380_Data_t pData;
	enum Direction d;
	unsigned int xres, yres;
	int i = 0, time = 20 ;
	char mess[30] ;
	struct fb_sess *fb;
	char *fb_dev_name;
	char buf[256];
	int max_iter;
 
	if(argc == 1) {
		fb_dev_name = "/dev/fb0";
	} else if(argc == 2) {
		fb_dev_name = argv[1];
	} else {
		fprintf(stderr, "Usage: \n\t%s [FRAMEBUFFER_DEVICE]\n\n",
				argc ? argv[0] : "fbtest");
		exit(EXIT_FAILURE);
	}

 
	printf("Trying to open the framebuffer device...\n");

	fb = fb_init(fb_dev_name);
	if(!fb) {
		fprintf(stderr, "Could not initialize %s.\n", fb_dev_name);
		exit(EXIT_FAILURE);
	}

	calibrage();

	max_iter = 3000;
	i=0;
	while(i<max_iter) {
		SMB380_GetData(pData);
		d = detection(pData, 8, 16);
		printf("Direction : %d \n\r", (int)d);
		//usleep(1000*1000);
		++i;
	}

	draw_background(fb);
	pokiball_initial(fb);
	usleep(1000*1000);
	droite(fb);
	usleep(1000*1000);
	droite(fb);
	usleep(1000*1000);
	droite(fb);
	usleep(1000*1000);
	droite(fb);
	usleep(1000*1000);
	droite(fb);
	usleep(1000*1000);
	droite(fb);
	usleep(1000*1000);
	droite(fb);
	usleep(1000*1000);
	droite(fb);
	usleep(1000*1000);
	droite(fb);
	usleep(1000*1000);
	droite(fb);
	usleep(1000*1000);
	droite(fb);
	//pikatchu_move (fb,30);
	/*usleep(1000*1000);
	gauche(fb);
	usleep(1000*1000);
	haut(fb);
	usleep(1000*1000);
	bas(fb);*/

	fb_close(fb);

	printf("Done. Exitting...\n");

	return 0 ;
}
