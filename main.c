#include <stdio.h>
#include "smb380_drv.h"
#include <unistd.h>
#include "fb_Graphic.h"
#include <time.h>
#include <math.h>

int main(void)
{
	struct fb_sess *fb;
	const char *fb_dev_name;
	char buf[256];
	pSMB380_Data_t pData;
	enum Direction d; 
	unsigned int xres, yres;
	int i = 0, time = 20 ;
	char mess[30] ;

	if(argc == 1) {
		fb_dev_name = "/dev/fb0";
	} else if(argc == 2) {
		fb_dev_name = argv[1];
	} else {
		fprintf(stderr, "Usage: \n\t%s [FRAMEBUFFER_DEVICE]\n\n",
				argc ? argv[0] : "fbtest");
		exit(EXIT_FAILURE);
	}

	printf_message() ;
 
	printf("Trying to open the framebuffer device...\n");

	fb = fb_init(fb_dev_name);
	if(!fb) {
		fprintf(stderr, "Could not initialize %s.\n", fb_dev_name);
		exit(EXIT_FAILURE);
	}

	printf("Getting framebuffer information:\n");
	printf("\tID:            %s\n", fb->finfo.id);
	printf("\tcolordepth:    %d bits/pixel\n", fb_bits_per_pixel(fb));
	printf("\tvideo memsize: %d bytes\n", fb_get_fb_size(fb));
	printf("\tresolution:    %dx%d\n", fb_xres(fb), fb_yres(fb));
	printf("\tvirtual res.:  %dx%d\n", fb->vinfo.xres_virtual,
					   fb->vinfo.yres_virtual);
	printf("\tgrayscale:     %d\n", fb->vinfo.grayscale);
	printf("\tphys size:     %dx%d mm\n", fb->vinfo.width,
						fb->vinfo.height);
	printf("\tpixel clock:   %d picoseconds\n", fb->vinfo.pixclock);
	printf("\tleft margin:   %d\n", fb->vinfo.left_margin);
	printf("\tright margin:  %d\n", fb->vinfo.right_margin);
	printf("\tupper margin:  %d\n", fb->vinfo.upper_margin);
	printf("\tlower margin:  %d\n", fb->vinfo.lower_margin);
	printf("\thsync_len:     %d\n", fb->vinfo.hsync_len);
	printf("\tvsync_len:     %d\n", fb->vinfo.vsync_len);
	printf("\trotate:        %d\n", fb->vinfo.rotate);
	printf("\tsmem_len:      %d\n", fb->finfo.smem_len);
	printf("\tline_length:   %d\n", fb->finfo.line_length);
	printf("\tmmio_start:    0x%08x\n", fb->finfo.mmio_start);
	printf("\tmmio_len:      %d\n", fb->finfo.mmio_len);
	printf("\tmmio_len:      %d\n", fb->finfo.mmio_len);

	//printf("coucou\n");

	//cd

	usleep(100000);
	calibrage();

	SMB380_Init();

	while(1) {
		SMB380_GetData(pData);
		d = detection(pData, 8, 16);
		printf("Direction : %d", (int)d); 
		usleep(1000*1000);
	}

	
	

	draw_background(fb);
	pokiball_initial(fb);
	usleep(1000*1000);
	pikatchu_move (fb,30);
	usleep(1000*1000);
	droite(fb);
	usleep(1000*1000);
	gauche(fb);
	usleep(1000*1000);
	haut(fb);
	usleep(1000*1000);
	bas(fb);

	fb_close(fb);

	printf("Done. Exitting...\n");

	return EXIT_SUCCESS;
}

	
