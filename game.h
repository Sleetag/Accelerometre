#ifndef __GAME_H__
#define __GAME_H__
#include "fb_Graphic.h"
#include "smb380_drv.h"

  void pikatchu_move (struct fb_sess *fb,int pas);
  void pikatchu_not_move (struct fb_sess *fb);
  void startGame();

#endif //__GAME_H__
