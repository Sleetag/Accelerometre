#ifndef __AFFICHAGE_H__
#define __AFFICHAGE_H__
#include "fb_Graphic.h"

void careau_blanc(struct fb_sess *fb);
void draw_background(struct fb_sess *fb);
void draw_pokiball(struct fb_sess *fb,unsigned  int centre_x, unsigned int centre_y);
void carreau_vert(struct fb_sess *fb);
void pokiball_initial(struct fb_sess *fb);
void droite(struct fb_sess *fb);
void gauche(struct fb_sess *fb);
void haut(struct fb_sess *fb);
void bas(struct fb_sess *fb);
void petit_carreau(struct fb_sess *fb, int x, int y,unsigned int couleur);
void carreau(struct fb_sess *fb, int x, int y,unsigned int couleur);
void draw_pikatchou(struct fb_sess *fb,int x, int y);

#endif //__AFFICHAGE_H__
