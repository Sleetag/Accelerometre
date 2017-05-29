/****************************************************************************
 *
 * Project: Framebuffer test application.
 *
 * Copyright by Olimex Ltd. All rights reserved.
 *
 * File: fbtest.c
 * Description: Very simple user-space Linux framebuffer test application.
 * Developer: Dimitar Dimitrov ( dinuxbg,gmail.com )
 *
 * Last change: $Date: 2008-09-29 14:28:41 +0300 (Mon, 29 Sep 2008) $
 * Revision: $Revision: 421 $
 * Id: $Id: fbtest.c 421 2008-09-29 11:28:41Z dimitar $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 ****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <time.h>

#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <linux/fb.h>

#include "hal.h"

#include "smb380_drv.h"

//  à  tester sans les fb_sync(fb) après les carreau blanc et carreau vert

// pokiball diminue de taille si je vais vers le haut ( s'approche de pikatchou) et si je vais vers le bas elle augmente de taille
// POKIBALL_x , POKIBALL_y et POKIBALL_r sont initialisés en pokiball_initial(fb)
// PIKATCHOU_x et PIKATCHOU_y sont initialisés en pikatchou_initial(fb)


static unsigned int POKIBALL_x;
static unsigned int POKIBALL_y;
static unsigned int POKIBALL_r;
static unsigned int PIKATCHOU_x;
static unsigned int PIKATCHOU_y;

/*
 la premiere chose à faire c'est draw_background(fb) qui permet d'afficher le backgroun de l'ecran  
*/
/* On peut changer le backgroun en changant les fonctions draw_background_bas(fb) et draw_background_bas(fb)
*/

/* après avoir initialisé pikatchou ( avec pikatchu_initial(fb) ), pikatchou peut aller vers la gauche et vers la doite avec certain pas positif
avec les fonctions gauche_pikatchou(fb,pas) et droite_pikatchou(fb,pas)
*/

/* après avoir initialisé pokiball ( avec pokiball_initial(fb) ), le pokiball peut aller vers la gauche, vers la doite , en haut et en bas
avec les fonctions gauche(fb), droite(fb), haut(fb) et bas(fb), ici on se deplace d'un pixel pour chaque fonction
*/


/** 
 * The mmap-ed access to the framebuffer device file should be much
 * faster but unfortunately mmap does not work on all drivers. 
 */
#define CONFIG_MMAP	0

struct fb_sess {
	int fd;
	unsigned char *memp;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
};


void fb_sync(struct fb_sess *fb);

static unsigned int fb_get_fb_size(struct fb_sess *fb)
{
	return fb->vinfo.xres * fb->vinfo.yres * (fb->vinfo.bits_per_pixel/8);
}


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

void fb_close(struct fb_sess *fb)
{
	if(fb) {
#if CONFIG_MMAP
		munmap(fb->memp, fb_get_fb_size(fb));
#else
		free(fb->memp);
#endif
		close(fb->fd);
		free(fb);
	}
}



/**
 * @note The 16 bit pixel color is formed from the following bits:
 * 	- bits 12-15: blue;
 * 	- bits 8-11: green;
 * 	- bits 4-7: red;
 * 	- bits 0-3: ???;
 */
void fb_draw_pixel(struct fb_sess *fb, unsigned int x, unsigned int y, unsigned int color)
{
	unsigned int offset;
	
	/* framebuffer is 16 bits per pixel */
	offset = (x + fb->vinfo.xoffset) * (fb->vinfo.bits_per_pixel/8) 
			+ (y + fb->vinfo.yoffset) * fb->finfo.line_length;
	switch(fb->vinfo.bits_per_pixel) {
		case 32:
		case 24:
			*(fb->memp + offset+2) = (color >> 16) & 0xff;
		case 16:
			*(fb->memp + offset+1) = (color >> 8) & 0xff;
		case 8:
			*(fb->memp + offset) = (color >> 0) & 0xff;
			break;
		default:
			abort();
	}
}

unsigned int fb_xres(struct fb_sess *fb)
{
	return fb->vinfo.xres;
}

unsigned int fb_yres(struct fb_sess *fb)
{
	return fb->vinfo.yres;
}

unsigned int fb_bits_per_pixel(struct fb_sess *fb)
{
	return fb->vinfo.bits_per_pixel;
}


void fb_sync(struct fb_sess *fb)
{
#if CONFIG_MMAP
	msync(fb->memp, fb_get_fb_size(fb), MS_SYNC);
#else
	lseek(fb->fd, 0, SEEK_SET);
	write(fb->fd, fb->memp, fb_get_fb_size(fb));
#endif
}

void draw_background_haut(struct fb_sess *fb)
{

	const unsigned int blanc = 0xffffff;
	const unsigned int vert = 0x00ff00;
	const unsigned int orange = 0xff;
	const unsigned int noir = 0x000000;
	const unsigned int jaune = 0x00ffff;

	unsigned int x, y;

	for(x=0; x<fb_xres(fb); ++x) {
		for(y=0; y<=fb_yres(fb)/2; ++y) {
			fb_draw_pixel(fb, x, y, blanc);
		}
	}

}
void draw_background_bas(struct fb_sess *fb)
{
	const unsigned int blanc = 0xffffff;//blanc
	const unsigned int vert = 0x00ff00;
	const unsigned int rouge = 0xdd;
	const unsigned int noir = 0x000000;
	const unsigned int jaune = 0x00ffff;

	unsigned int x, y;

	for(x=0; x<fb_xres(fb); ++x) {
		for(y=(fb_yres(fb)/2)+1; y<fb_yres(fb); ++y) {
			fb_draw_pixel(fb, x, y, vert);
		}
	}
}

void draw_background(struct fb_sess *fb)
{
	const unsigned int blanc = 0xffffff;
	const unsigned int vert = 0x00ff00;
	const unsigned int orange = 0xff;
	const unsigned int noir = 0x000000;
	const unsigned int jaune = 0x00ffff;

	draw_background_haut(fb);
	draw_background_bas(fb);
	fb_sync(fb);
}

void draw_pokiball(struct fb_sess *fb,unsigned  int centre_x, unsigned int centre_y,unsigned  int rayon)
{
	const unsigned int blanc = 0xffffff;//blanc
	const unsigned int vert = 0x00ff00;
	const unsigned int rouge = 0xdd;
	const unsigned int noir = 0x000000;
	const unsigned int jaune = 0x00ffff;


	unsigned int couleur;

	unsigned int x, y;

		for ( x = centre_x - rayon ; x <= centre_x+rayon ; ++x)
		{
			for ( y = centre_y - rayon ; y <= centre_y+rayon ; ++y)
			{

				couleur = blanc ;

				if ( (x-centre_x)*(x-centre_x) + (y-centre_y)*(y-centre_y) <= rayon*rayon )
				{

		
					if (y<centre_y)
						couleur = rouge ;
	
					if (((x-centre_x)*(x-centre_x) + (y-centre_y)*(y-centre_y)<=rayon*rayon 
					&& (x-centre_x)*(x-centre_x) + (y-centre_y)*(y-centre_y)>=(rayon-2)*(rayon-2))
					|| ((x-centre_x)*(x-centre_x) + (y-centre_y)*(y-centre_y)<=(rayon/3)*(rayon/3)
					&&(x-centre_x)*(x-centre_x) + (y-centre_y)*(y-centre_y)>=((rayon/3)-2)*((rayon/3)-2)))
					{
						couleur = noir ;
					}

					carreau(fb, x, y,couleur);
				}
			}
		}

	// lignes noires

	for ( x = centre_x -rayon ;x <= centre_x; ++x)
	{
		if ( x < centre_x - (rayon/3) )
			carreau(fb, x,  centre_y ,noir);
	}

	for ( x = centre_x ;x <= centre_x + rayon ; ++x)
	{
		if ( x > centre_x + (rayon/3) )
			carreau(fb, x,  centre_y ,noir);
	}
}


void pokiball_initial(struct fb_sess *fb)
{
	POKIBALL_x = fb_xres(fb) / 2;
	POKIBALL_y = (fb_yres(fb)/3 )+ 81;
	POKIBALL_r=21;
	draw_background_bas(fb);
	fb_sync(fb);
	draw_pokiball(fb,POKIBALL_x,POKIBALL_y,POKIBALL_r);
	fb_sync(fb);
}

void droite(struct fb_sess *fb)
{
	draw_background_bas(fb);
	fb_sync(fb);
	if (POKIBALL_x+1< fb_xres(fb)-POKIBALL_r)
	{
		POKIBALL_x = POKIBALL_x+1 ;
		draw_pokiball(fb,POKIBALL_x,POKIBALL_y,POKIBALL_r);
	}
	else draw_pokiball(fb,POKIBALL_x,POKIBALL_y,POKIBALL_r);
	fb_sync(fb);
}

void gauche(struct fb_sess *fb)
{
	draw_background_bas(fb);
	fb_sync(fb);
	if (POKIBALL_x - 1 > POKIBALL_r)
	{
		POKIBALL_x = POKIBALL_x-1;
		draw_pokiball(fb,POKIBALL_x,POKIBALL_y,POKIBALL_r);
	}
	else draw_pokiball(fb,POKIBALL_x,POKIBALL_y,POKIBALL_r);
	fb_sync(fb);
}

void haut(struct fb_sess *fb)
{
	draw_background_bas(fb);
	fb_sync(fb);
	if (POKIBALL_y -1 > POKIBALL_r ) 
	{
		POKIBALL_y = POKIBALL_y - 1 ;
		POKIBALL_r = POKIBALL_r -1;
		draw_pokiball(fb,POKIBALL_x,POKIBALL_y,POKIBALL_r);
	}
	else draw_pokiball(fb,POKIBALL_x,POKIBALL_y,POKIBALL_r);
	fb_sync(fb);
}

void bas(struct fb_sess *fb)
{
	draw_background_bas(fb);
	fb_sync(fb);
	if (POKIBALL_y +1 < fb_yres(fb)-POKIBALL_r )
	{
		POKIBALL_y = POKIBALL_y +1 ;
		POKIBALL_r = POKIBALL_r +1;
		draw_pokiball(fb,POKIBALL_x,POKIBALL_y,POKIBALL_r);
	}
	else draw_pokiball(fb,POKIBALL_x,POKIBALL_y,POKIBALL_r);
	fb_sync(fb);
}

void petit_carreau(struct fb_sess *fb, int x, int y,unsigned int couleur)
{
		fb_draw_pixel(fb, x, y, couleur);
		fb_draw_pixel(fb, x, y+1,  couleur);
		fb_draw_pixel(fb, x+1, y,  couleur);
		fb_draw_pixel(fb, x+1, y+1,  couleur);
}



void carreau(struct fb_sess *fb, int x, int y,unsigned int couleur)
{
		fb_draw_pixel(fb, x, y, couleur);
		fb_draw_pixel(fb, x, y+1,  couleur);
		fb_draw_pixel(fb, x, y-1,  couleur);
		fb_draw_pixel(fb, x+1, y,  couleur);
		fb_draw_pixel(fb, x+1, y+1,  couleur);
		fb_draw_pixel(fb, x+1, y-1,  couleur);
		fb_draw_pixel(fb, x-1, y,  couleur);
		fb_draw_pixel(fb, x-1, y+1,  couleur);
		fb_draw_pixel(fb, x-1, y-1,  couleur);

}


void draw_pikatchou(struct fb_sess *fb,int x, int y)

{


	const unsigned int blanc = 0xffffff;
	const unsigned int vert = 0x00ff00;
	const unsigned int orange = 0x00aaff;
	const unsigned int rouge = 0xdd;
	const unsigned int noir = 0x000000;
	const unsigned int jaune = 0x00ffff;


	/*visage*/

	/* gauche */

	// noir

	carreau(fb, x, y,noir);
	carreau(fb, (x)+3, (y),noir);
	carreau(fb, (x)+3, (y)-3,noir);
	carreau(fb, (x)+3, (y)-6,noir);
	carreau(fb, (x)+3, (y)-9,noir);
	carreau(fb, (x)+6, (y)-9,noir);


	carreau(fb, (x), (y)-12,noir);
	carreau(fb, (x), (y)-12,noir);
	carreau(fb, (x)-3, (y)-12,noir);
	carreau(fb, (x)-6, (y)-15,noir);
	carreau(fb, (x)-3, (y)-15,noir);
	carreau(fb, (x)-6, (y)-18,noir);
	carreau(fb, (x)-9, (y)-18,noir);
	carreau(fb, (x)-6, (y)-21,noir);
	carreau(fb, (x)-9, (y)-21,noir);
	carreau(fb, (x)-3, (y)-21,noir);
	carreau(fb, (x), (y)-21,noir);
	carreau(fb, (x)+1, (y)-21,noir);
	carreau(fb, (x)+6, (y)-21,noir);
	carreau(fb, (x)+9, (y)-21,noir);
	carreau(fb, (x)+15, (y)-21,noir);
	carreau(fb, (x)+12, (y)-24,noir);
	carreau(fb, (x)+9, (y)-27,noir);
	carreau(fb, (x)+6, (y)-30,noir);
	carreau(fb, (x)+3, (y)-33,noir);
	carreau(fb, (x), (y)-36,noir);
	carreau(fb, (x)-3, (y)-33,noir);
	carreau(fb, (x)-3, (y)-30,noir);
	carreau(fb, (x)-3, (y)-27,noir);
	carreau(fb, (x)-3, (y)-24,noir);

	carreau(fb, (x)+3, (y)-21,noir);
	carreau(fb, (x)-3, (y)+3,noir);

	carreau(fb, (x)+6, (y)-18,noir);
	carreau(fb, (x)+9, (y)-18,noir);
	carreau(fb, (x)+12, (y)-18,noir);
	carreau(fb, (x)+15, (y)-18,noir);


	carreau(fb, (x)-3, (y)+1,noir);
	carreau(fb, (x)-3, (y)+6,noir);
	carreau(fb, (x), (y)+6,noir);
	carreau(fb, (x), (y)+9,noir);
	carreau(fb, (x)+3, (y)+9,noir);
	carreau(fb, (x)+18, (y)-18,noir);




	// jaune 

	carreau(fb, (x), (y)-33,jaune);
	carreau(fb, (x), (y)-30,jaune);
	carreau(fb, (x)+3, (y)-30,jaune);

	carreau(fb, (x), (y)-27,jaune);
	carreau(fb, (x)+3, (y)-27,jaune);
	carreau(fb, (x)+6, (y)-27,jaune);
	

	carreau(fb, (x), (y)-24,jaune);
	carreau(fb, (x)+3, (y)-24,jaune);
	carreau(fb, (x)+6, (y)-24,jaune);
	carreau(fb, (x)+9, (y)-24,jaune);

	carreau(fb, (x)+12, (y)-21,jaune);


	carreau(fb, (x), (y)-18,jaune);
	

	carreau(fb, (x)+3, (y)-15,jaune);
	carreau(fb, (x)+6, (y)-15,jaune);
	carreau(fb, (x)+9, (y)-15,jaune);
	
	carreau(fb, (x)+15, (y)-15,jaune);

	carreau(fb, (x)+6, (y)-12,jaune);
	carreau(fb, (x)+9, (y)-12,jaune);
	carreau(fb, (x)+12, (y)-12,jaune);
	carreau(fb, (x)+15, (y)-12,jaune);


	
	carreau(fb, (x)+12, (y)-9,jaune);
	carreau(fb, (x)+15, (y)-9,jaune);


	carreau(fb, (x)+9, (y)-6,jaune);
	carreau(fb, (x)+12, (y)-6,jaune);
	carreau(fb, (x)+15, (y)-6,jaune);


	carreau(fb, (x)+9, (y),jaune);
	carreau(fb, (x)+9, (y)-3,jaune);

	carreau(fb, (x)+15, (y),jaune);
	carreau(fb, (x)+15, (y)-3,jaune);

	carreau(fb, (x)+6, (y),jaune);
	carreau(fb, (x)+6, (y)-3,jaune);

	carreau(fb, (x)+6, (y)+3,jaune);
	carreau(fb, (x)+9, (y)+3,jaune);
	carreau(fb, (x)+12, (y)+3,jaune);
	carreau(fb, (x)+15, (y)+3,jaune);

	carreau(fb, (x)+9, (y)+6,jaune);
	carreau(fb, (x)+12, (y)+6,jaune);
	carreau(fb, (x)+15, (y)+6,jaune);

	carreau(fb, (x)+9, (y)+9,jaune);
	carreau(fb, (x)+12, (y)+9,jaune);
	carreau(fb, (x)+15, (y)+9,jaune);

	carreau(fb, (x)+6, (y)+12,jaune);
	carreau(fb, (x)+12, (y)+12,jaune);
	carreau(fb, (x)+15, (y)+12,jaune);

	
	

	//orange

	carreau(fb, (x)-3, (y)-18,orange);
	carreau(fb, (x)+3, (y)-18,orange);

	carreau(fb, (x), (y)-15,orange);
	carreau(fb, (x)+12, (y)-15,orange);

	carreau(fb, (x)+3, (y)-12,orange);

	carreau(fb, (x)+9, (y)-9,orange);

	carreau(fb, (x)+6, (y)-6,orange);

	carreau(fb, (x)+6, (y)+9,orange);
	carreau(fb, (x)+3, (y)+12,orange);
	carreau(fb, (x)+9, (y)+12,orange);


	// rouge 

	carreau(fb, (x), (y)+3,rouge);
	carreau(fb, (x)+3, (y)+3,rouge);
	
	carreau(fb, (x)+3, (y)+6,rouge);
	carreau(fb, (x)+6, (y)+6,rouge);

	/* droite */

	//noir

	carreau(fb, (x)+21, (y)-18,noir);
	carreau(fb, (x)+24, (y)-18,noir);
	carreau(fb, (x)+27, (y)-18,noir);
	carreau(fb, (x)+30, (y)-18,noir);
	
	carreau(fb, (x)+30, (y)-21,noir);
	carreau(fb, (x)+33, (y)-21,noir);
	carreau(fb, (x)+33, (y)-24,noir);
	carreau(fb, (x)+36, (y)-27,noir);
	carreau(fb, (x)+39, (y)-27,noir);
	carreau(fb, (x)+42, (y)-27,noir);
	carreau(fb, (x)+36, (y)-30,noir);
	carreau(fb, (x)+39, (y)-30,noir);
	carreau(fb, (x)+42, (y)-30,noir);
	carreau(fb, (x)+39, (y)-33,noir);
	carreau(fb, (x)+42, (y)-33,noir);
	

	carreau(fb, (x)+39, (y)-24,noir);
	carreau(fb, (x)+42, (y)-24,noir);
	carreau(fb, (x)+42, (y)-21,noir);
	carreau(fb, (x)+42, (y)-18,noir);
	carreau(fb, (x)+39, (y)-18,noir);
	carreau(fb, (x)+39, (y)-15,noir);
	carreau(fb, (x)+39, (y)-12,noir);
	carreau(fb, (x)+39, (y)-9,noir);
	carreau(fb, (x)+36, (y)-9,noir);

	carreau(fb, (x)+39, (y)-6,noir);
	carreau(fb, (x)+39, (y)-3,noir);

	carreau(fb, (x)+39, (y),noir);
	carreau(fb, (x)+42, (y),noir);


	carreau(fb, (x)+42, (y)+1,noir);
	
	carreau(fb, (x)+42, (y)+6,noir);
	carreau(fb, (x)+39, (y)+6,noir);
	carreau(fb, (x)+39, (y)+9,noir);
	carreau(fb, (x)+36, (y)+9,noir);


	carreau(fb, (x)+42, (y)+3,noir);


	// jaune 

	carreau(fb, (x)+36, (y)-24,jaune);
	carreau(fb, (x)+36, (y)-21,jaune);
	

	carreau(fb, (x)+36, (y)-18,jaune);
	carreau(fb, (x)+33, (y)-18,jaune);

	
	carreau(fb, (x)+18, (y)-15,jaune);
	carreau(fb, (x)+21, (y)-15,jaune);
	carreau(fb, (x)+24, (y)-15,jaune);
	carreau(fb, (x)+27, (y)-15,jaune);
	carreau(fb, (x)+33, (y)-15,jaune);
	carreau(fb, (x)+36, (y)-15,jaune);


	carreau(fb, (x)+18, (y)-12,jaune);
	carreau(fb, (x)+21, (y)-12,jaune);
	carreau(fb, (x)+24, (y)-12,jaune);
	carreau(fb, (x)+27, (y)-12,jaune);
	carreau(fb, (x)+30, (y)-12,jaune);
	carreau(fb, (x)+33, (y)-12,jaune);
	
	carreau(fb, (x)+18, (y)-9,jaune);
	carreau(fb, (x)+21, (y)-9,jaune);
	carreau(fb, (x)+24, (y)-9,jaune);
	carreau(fb, (x)+27, (y)-9,jaune);
	carreau(fb, (x)+30, (y)-9,jaune);

	
	carreau(fb, (x)+18, (y)-6,jaune);
	carreau(fb, (x)+21, (y)-6,jaune);
	carreau(fb, (x)+24, (y)-6,jaune);
	carreau(fb, (x)+27, (y)-6,jaune);
	carreau(fb, (x)+30, (y)-6,jaune);
	carreau(fb, (x)+33, (y)-6,jaune);

	carreau(fb, (x)+18, (y)-3,jaune);
	carreau(fb, (x)+21, (y)-3,jaune);
	carreau(fb, (x)+24, (y)-3,jaune);
	carreau(fb, (x)+33, (y)-3,jaune);
	carreau(fb, (x)+36, (y)-3,jaune);

	carreau(fb, (x)+18, (y),jaune);
	carreau(fb, (x)+21, (y),jaune);
	carreau(fb, (x)+24, (y),jaune);
	carreau(fb, (x)+33, (y),jaune);
	carreau(fb, (x)+36, (y),jaune);

	carreau(fb, (x)+18, (y)+3,jaune);
	carreau(fb, (x)+24, (y)+3,jaune);
	carreau(fb, (x)+27, (y)+3,jaune);
	carreau(fb, (x)+30, (y)+3,jaune);
	carreau(fb, (x)+33, (y)+3,jaune);

	carreau(fb, (x)+18, (y)+6,jaune);
	carreau(fb, (x)+21, (y)+6,jaune);
	carreau(fb, (x)+24, (y)+6,jaune);
	carreau(fb, (x)+27, (y)+6,jaune);
	carreau(fb, (x)+30, (y)+6,jaune);

	
	carreau(fb, (x)+27, (y)+9,jaune);
	carreau(fb, (x)+30, (y)+9,jaune);
	carreau(fb, (x)+33, (y)+9,jaune);

	carreau(fb, (x)+18, (y)+12,jaune);
	carreau(fb, (x)+21, (y)+12,jaune);
	carreau(fb, (x)+24, (y)+12,jaune);
	carreau(fb, (x)+27, (y)+12,jaune);
	carreau(fb, (x)+33, (y)+12,jaune);
	
	carreau(fb, (x)+27, (y)+15,jaune);
	carreau(fb, (x)+30, (y)+15,jaune);
	carreau(fb, (x)+33, (y)+15,jaune);
	carreau(fb, (x)+36, (y)+15,jaune);

	// orange 

	carreau(fb, (x)+39, (y)-21,orange);

	carreau(fb, (x)+30, (y)-15,orange);

	carreau(fb, (x)+36, (y)-12,orange);

	carreau(fb, (x)+33, (y)-9,orange);

	carreau(fb, (x)+36, (y)-6,orange);

	carreau(fb, (x)+18, (y)+9,orange);
	carreau(fb, (x)+24, (y)+9,orange);
	carreau(fb, (x)+36, (y)+9,orange);

	carreau(fb, (x)+30, (y)+12,orange);
	carreau(fb, (x)+36, (y)+12,orange);

	carreau(fb, (x)+18, (y)+15,orange);
	carreau(fb, (x)+21, (y)+15,orange);
	carreau(fb, (x)+24, (y)+15,orange);


	// rouge

	carreau(fb, (x)+36, (y)+3,rouge);
	carreau(fb, (x)+39, (y)+3,rouge);
	
	carreau(fb, (x)+33, (y)+6,rouge);
	carreau(fb, (x)+36, (y)+6,rouge);


	/*nez*/

	carreau(fb, (x)+21, (y)+3,noir);

	/*bouche */

	carreau(fb, (x)+21, (y)+9,noir);
	

	
	/*yeux*/
	
	/* gauche */ 

	carreau(fb, (x)+9, (y)-3,noir);
	carreau(fb, (x)+9, (y),noir);
	carreau(fb, (x)+12, (y),noir);


	/* droite */

	carreau(fb, (x)+27, (y),noir);
	carreau(fb, (x)+30, (y),noir);
	carreau(fb, (x)+30, (y)-3,noir);
	

	/*corp*/

	/* gauche 18 */


	carreau(fb, (x), (y)+12,noir);
	
	carreau(fb, (x), (y)+15,noir);
	carreau(fb, (x), (y)+21,noir);
	carreau(fb, (x), (y)+18,noir);

	carreau(fb, (x), (y)+30,noir);//rouge test

	carreau(fb, (x)+3, (y)+33,noir);

	carreau(fb, (x), (y)+27,noir);
	carreau(fb, (x), (y)+24,noir);
	carreau(fb, (x)+6, (y)+30,noir);
	carreau(fb, (x)+9, (y)+27,noir);

	carreau(fb, (x), (y)+33,noir);

	carreau(fb, (x)+21, (y)+33,noir);
	carreau(fb, (x)+18, (y)+33,noir);

	carreau(fb, (x)-3, (y)+36,noir);
	
	carreau(fb, (x)+9, (y)+36,noir);
	carreau(fb, (x)+12, (y)+36,noir);
	carreau(fb, (x)+15, (y)+36,noir);
	carreau(fb, (x)+18, (y)+36,noir);

	carreau(fb, (x), (y)+39,noir);
	carreau(fb, (x)+3, (y)+39,noir);
	carreau(fb, (x)+6, (y)+39,noir);
	
	carreau(fb, (x)+6, (y)+36,noir);


	// jaune 


	carreau(fb, (x)+3, (y)+15,jaune);
	carreau(fb, (x)+6, (y)+15,jaune);
	carreau(fb, (x)+9, (y)+15,jaune);
	carreau(fb, (x)+12, (y)+15,jaune);
	carreau(fb, (x)+15, (y)+15,jaune);


	carreau(fb, (x)+3, (y)+18,jaune);
	carreau(fb, (x)+6, (y)+18,jaune);
	carreau(fb, (x)+9, (y)+18,jaune);
	carreau(fb, (x)+12, (y)+18,jaune);
	carreau(fb, (x)+15, (y)+18,jaune);

	carreau(fb, (x)+3, (y)+21,jaune);
	carreau(fb, (x)+6, (y)+21,jaune);
	carreau(fb, (x)+9, (y)+21,jaune);
	carreau(fb, (x)+12, (y)+21,jaune);
	carreau(fb, (x)+15, (y)+21,jaune);

	
	carreau(fb, (x)+6, (y)+24,jaune);
	carreau(fb, (x)+12, (y)+24,jaune);
	carreau(fb, (x)+15, (y)+24,jaune);


	carreau(fb, (x)+3, (y)+27,jaune);
	carreau(fb, (x)+6, (y)+27,jaune);
	carreau(fb, (x)+12, (y)+27,jaune);
	carreau(fb, (x)+15, (y)+27,jaune);


	carreau(fb, (x)+3, (y)+30,jaune);
	carreau(fb, (x)+9, (y)+30,jaune);
	carreau(fb, (x)+12, (y)+30,jaune);
	carreau(fb, (x)+15, (y)+30,jaune);
	
	carreau(fb, (x)+9, (y)+33,jaune);
	carreau(fb, (x)+12, (y)+33,jaune);

	carreau(fb, (x), (y)+36,jaune);
	carreau(fb, (x)+3, (y)+36,jaune);


	// orange 


	carreau(fb, (x)+3, (y)+24,orange);
	carreau(fb, (x)+9, (y)+24,orange);

	carreau(fb, (x)+6, (y)+33,orange);
	carreau(fb, (x)+15, (y)+33,orange);

	/* droite */ 

	carreau(fb, (x)+39, (y)+12,noir);
	carreau(fb, (x)+42, (y)+15,noir);
	carreau(fb, (x)+39, (y)+15,noir);
	

	carreau(fb, (x)+39, (y)+18,noir);
	carreau(fb, (x)+42, (y)+18,noir);
	carreau(fb, (x)+42, (y)+21,noir);
	carreau(fb, (x)+42, (y)+24,noir);
	carreau(fb, (x)+42, (y)+27,noir);
	carreau(fb, (x)+42, (y)+30,noir);
	carreau(fb, (x)+39, (y)+30,noir);
	carreau(fb, (x)+36, (y)+27,noir);
	carreau(fb, (x)+33, (y)+24,noir);
	

	carreau(fb, (x)+39, (y)+33,noir);
	carreau(fb, (x)+36, (y)+33,noir);

	carreau(fb, (x)+24, (y)+33,noir);

	carreau(fb, (x)+42, (y)+36,noir);

	carreau(fb, (x)+21, (y)+36,noir);
	carreau(fb, (x)+24, (y)+36,noir);
	carreau(fb, (x)+27, (y)+36,noir);
	carreau(fb, (x)+30, (y)+36,noir);

	carreau(fb, (x)+27, (y)+36,noir);

	carreau(fb, (x)+39, (y)+39,noir);
	carreau(fb, (x)+36, (y)+39,noir);
	carreau(fb, (x)+33, (y)+39,noir);

	//jaune 

	carreau(fb, (x)+18, (y)+18,jaune);
	carreau(fb, (x)+24, (y)+18,jaune);
	carreau(fb, (x)+27, (y)+18,jaune);
	carreau(fb, (x)+30, (y)+18,jaune);
	carreau(fb, (x)+33, (y)+18,jaune);
	carreau(fb, (x)+36, (y)+18,jaune);

	carreau(fb, (x)+18, (y)+21,jaune);
	carreau(fb, (x)+21, (y)+21,jaune);
	carreau(fb, (x)+24, (y)+21,jaune);
	carreau(fb, (x)+27, (y)+21,jaune);
	carreau(fb, (x)+30, (y)+21,jaune);
	carreau(fb, (x)+36, (y)+21,jaune);
	

	carreau(fb, (x)+18, (y)+24,jaune);
	carreau(fb, (x)+21, (y)+24,jaune);
	carreau(fb, (x)+24, (y)+24,jaune);
	carreau(fb, (x)+27, (y)+24,jaune);
	carreau(fb, (x)+30, (y)+24,jaune);
	carreau(fb, (x)+36, (y)+24,jaune);
	carreau(fb, (x)+39, (y)+24,jaune);

	carreau(fb, (x)+18, (y)+27,jaune);
	carreau(fb, (x)+21, (y)+27,jaune);
	carreau(fb, (x)+24, (y)+27,jaune);
	carreau(fb, (x)+27, (y)+27,jaune);
	carreau(fb, (x)+30, (y)+27,jaune);
	carreau(fb, (x)+33, (y)+27,jaune);
	carreau(fb, (x)+39, (y)+27,jaune);

	carreau(fb, (x)+18, (y)+30,jaune);
	carreau(fb, (x)+21, (y)+30,jaune);
	carreau(fb, (x)+24, (y)+30,jaune);
	carreau(fb, (x)+27, (y)+30,jaune);
	carreau(fb, (x)+30, (y)+30,jaune);
	carreau(fb, (x)+33, (y)+30,jaune);
	
	carreau(fb, (x)+30, (y)+33,jaune);
	
	carreau(fb, (x)+33, (y)+36,jaune);
	carreau(fb, (x)+36, (y)+36,jaune);
	carreau(fb, (x)+39, (y)+36,jaune);

	//orange


	carreau(fb, (x)+21, (y)+18,orange);

	carreau(fb, (x)+33, (y)+21,orange);
	carreau(fb, (x)+39, (y)+21,orange);

	carreau(fb, (x)+36, (y)+30,orange);

	carreau(fb, (x)+27, (y)+33,orange);
	carreau(fb, (x)+33, (y)+33,orange);
	
}


void pikatchu_initial (struct fb_sess *fb)

{
	PIKATCHOU_x =  fb_xres(fb)/2;
	PIKATCHOU_y = fb_yres(fb)/3;
	draw_background_haut(fb);
	fb_sync(fb);
	draw_pikatchou(fb,PIKATCHOU_x,PIKATCHOU_y);
	fb_sync(fb);
}

void droite_pikatchou(struct fb_sess *fb, int  pas)
{
	draw_background_haut(fb);
	fb_sync(fb);
	if (PIKATCHOU_x+1< fb_xres(fb)-44)
	{
		PIKATCHOU_x = PIKATCHOU_x+pas ;
		draw_pikatchou(fb,PIKATCHOU_x,PIKATCHOU_y);
	}
	else draw_pikatchou(fb,PIKATCHOU_x,PIKATCHOU_y);
	fb_sync(fb);
}

void gauche_pikatchou(struct fb_sess *fb, int pas)
{
	draw_background_haut(fb);
	fb_sync(fb);
	if (PIKATCHOU_x - 1 > 9)
	{
		PIKATCHOU_x = PIKATCHOU_x-pas;
		draw_pikatchou(fb,PIKATCHOU_x,PIKATCHOU_y);
	}
	else draw_pikatchou(fb,PIKATCHOU_x,PIKATCHOU_y);
	fb_sync(fb);
}

/*--------------------------------------------------------------*/
enum Direction {DROITE = 0, GAUCHE = 1, HAUT = 2, NULLE = -1};

Int16S ACC_X, ACC_Y, ACC_Z; 


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

void calibrage () {
	pSMB380_Data_t pData;
	int i = 0, time = 20 ;

	SMB380_Init();
	
	//printf("Calibrage de la position ...\n");
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

#define ARRAY_NUMELEM(A)	((sizeof(A)/sizeof((A)[0])))
int main(int argc, char *argv[])
{
	struct fb_sess *fb;
	const char *fb_dev_name;
	char buf[256];
	int i = 0, time = 20 ;
	pSMB380_Data_t pData;
	enum Direction d; 


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


	SMB380_Init();
	
	
	//printf("Calibrage de la position ...\n");
	/*for(i = 0; i < time; ++i) {
		SMB380_GetData(pData);

		printf("... %d /20 \n", i);
		usleep(1000000);
	}*/

	
	
	/*i=0;
	time = 300;

	
	SMB380_GetData(pData);

	
	printf("AccX = %d\n", pData->AccX);	
	printf("AccY = %d\n", pData->AccY);	
	printf("AccZ = %d\n", pData->AccZ);
	
	ACC_X = pData->AccX;
	ACC_Y = pData->AccY;
	ACC_Z = pData->AccZ;	

	i = 0;*/
	while(1) {
		SMB380_GetData(pData);
		d = detection(pData, 8, 16);
		printf("Direction : %d", (int)d); 
		usleep(1000*1000);
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

	return EXIT_SUCCESS;
}





