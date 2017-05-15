#include "fb_Graphic.h"

void pikatchu_move (struct fb_sess *fb,int pas)
{
	int x;
	int y = fb_yres(fb)/3;
	int direct = 1 ;
	while(1)
	{
		if ( direct  )
		{
			for(x=9;x<fb_xres(fb)-45;x=x+pas)
				{
					draw_pikatchou(fb,x,y);
					fb_sync(fb);
					usleep(1000*1000);
					careau_blanc(fb);
					fb_sync(fb);
				}
			direct = 0;
		}
		else
		{
			for(x=fb_xres(fb)-44;x>9;x=x-pas)
				{
					draw_pikatchou(fb,x,y);
					fb_sync(fb);
					usleep(1000*1000);
					careau_blanc(fb);
					fb_sync(fb);
				}
			direct = 1 ;
		}
	}

}


void pikatchu_not_move (struct fb_sess *fb)

{
	int x =  fb_xres(fb)/2;
	int y = fb_yres(fb)/3;
	draw_pikatchou(fb,x,y);
	fb_sync(fb);

}
void startGame(){
    draw_background(fb);
	draw_pokiball(fb);
	usleep(1000*1000);
	pikatchu_move (fb,30);
	usleep(1000*1000);
}
