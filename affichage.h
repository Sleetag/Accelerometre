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
#ifndef __FB_GRAPHIC_H__
#define __FB_GRAPHIC_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <linux/fb.h>



#define FB_DEV_NAME "/dev/fb0"

#define BLACK 0x000000
#define WHITE 0xFFFFFF
#define GREEN 0x00FF00
#define BLUE 0xFF0000
//#define BACKGROUND_COLOR WHITE
#define BACKGROUND_COLOR BLACK

#define COEFF 1.4
#define COEFF_X (0-COEFF)
#define COEFF_Y (0-COEFF)

#define BARRE_SIZE 30
#define BALL_SIZE 2

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

typedef struct {
	short int x, y, color, size;
} ball;


/**
 * The mmap-ed access to the framebuffer device file should be much
 * faster but unfortunately mmap does not work on all drivers.
 */
#define CONFIG_MMAP	0

struct fb_sess{
	int fd;
	unsigned char *memp;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
};



unsigned int fb_get_fb_size(struct fb_sess *fb);
//fb_sess *fb_init(const char *fb_dev_name);
void fb_close(struct fb_sess *fb);
void fb_draw_pixel(struct fb_sess *fb, unsigned int x, unsigned int y, unsigned int color);
unsigned int fb_xres(struct fb_sess *fb);
unsigned int fb_yres(struct fb_sess *fb);
unsigned int fb_bits_per_pixel(struct fb_sess *fb);
void fb_sync(struct fb_sess *fb);
unsigned int invert_y(struct fb_sess *, unsigned int);
int getCharNum(char);
unsigned int drawChar(struct fb_sess *, unsigned int, unsigned int, unsigned int, char, unsigned int, unsigned int);
unsigned int drawString(struct fb_sess *, unsigned int, unsigned int, unsigned int, char s[], unsigned int, unsigned int, unsigned int);

ball makeBall(int, int, int, int);
void drawBall(struct fb_sess *, ball *);
void clearBall(struct fb_sess *, ball *);
void moveBall(struct fb_sess *, ball *, int, int);

#endif
