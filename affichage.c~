#include "affichage.h"
#include "fb_Graphic.h"

void careau_blanc(struct fb_sess *fb)
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


void draw_background(struct fb_sess *fb)
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

	for(x=0; x<fb_xres(fb); ++x) {
		for(y=(fb_yres(fb)/2)+1; y<fb_yres(fb); ++y) {
			fb_draw_pixel(fb, x, y, vert);
		}
	}
	fb_sync(fb);
}

void draw_pokiball(struct fb_sess *fb)
{
	const unsigned int blanc = 0xffffff;//blanc
	const unsigned int vert = 0x00ff00;
	const unsigned int rouge = 0xdd;
	const unsigned int noir = 0x000000;
	const unsigned int jaune = 0x00ffff;


	unsigned int centre_x = fb_xres(fb) / 2;
	unsigned int centre_y = (fb_yres(fb)/3 )+ 81;

	unsigned  int rayon ;
	unsigned int couleur;

	unsigned int x, y;

	for ( rayon =  0 ; rayon < 21 ; ++rayon)
	{

		for ( x = centre_x - rayon ; x < centre_x+rayon ; ++x)
		{

			couleur = blanc ;
			y = sqrt((rayon*rayon)-((x - centre_x)*(x - centre_x)) ) + centre_y ;


			if ((rayon == 21 || rayon == 20 )||
				(rayon == 7 || rayon == 6 ) || (rayon == 0 || rayon == 1 || rayon == 2 ))
				couleur = noir ;

			carreau(fb, x, y,couleur);

			y = -(sqrt((rayon*rayon )-((x - centre_x)*(x - centre_x)) ) )+ centre_y ;

			if (rayon > 7 && (rayon != 21 && rayon != 20) )
				couleur = rouge ;

			carreau(fb, x, y,couleur);
		}
	}

	// lignes noires

	for ( x = centre_x -rayon ;x < centre_x; ++x)
	{
		if ( x < centre_x - 7 )
			carreau(fb, x,  centre_y ,noir);
	}

	for ( x = centre_x ;x < centre_x + rayon ; ++x)
	{
		if ( x > centre_x + 7 )
			carreau(fb, x,  centre_y ,noir);
	}





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


void draw_pikatchou(struct fb_sess *fb,int x, int y){


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
