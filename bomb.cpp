#include "bomb.h"
#include "Sound.h"
#include "fruitImage.h"
#include "SSD1306.h"
#include "Random.h"

bomb_t::bomb_t() {
	
}

bool bomb_t::Collision(int x1, int y1) {
	int x = get_x() + 6 ;
  int y = get_y() - 6;
  if( ((x1-x)*(x1-x) + (y1-y)*(y1-y)) < 41){
    set_status(dead);
		Explosion_Sound();
		return 1;
  }
	return 0;
    
}

void bomb_t::Init(int x,int y, int vx, int vy, status_t life, int n) {
	set_x(x);
	set_y(y);
	set_vx(vx);
	set_vy(vy);
	set_status(life);
	set_image(bomb);
}

void bomb_t::Move(void) {
	/******set y value*****/
			int y = get_y();
			//bottom of screen
			if(y > 80){
				//set_status(dead);
				Init((Random() % 110) + 5, 80, 1, -1, dead, 0);
			}	
			else{
				//top of screen
				if(y < 15){
					set_vy(1);
				}
				//going up
				set_y(y + get_vy());								
			}
		/******set x value*****/
			int x = get_x();
			if(x > 115){
				set_vx(-1);
			} 
			if(x < 5){
				set_vx(1);
			}
//		if( fruits[i].get_y() < 10){
//			fruits[i].set_vx(0);
//		}
			set_x(x + get_vx());	
}

void bomb_t::Draw(void) {
	SSD1306_DrawBMP(get_x(), get_y(), get_image(), 0, SSD1306_INVERSE);
}
