//fruit.cpp

#include "fruit.h"
#include <stdint.h>
#include "Random.h"
#include "Images.h"
#include "fruitImage.h"
#include "Sound.h"
#include "SSD1306.h"

fruit_t::fruit_t() {
	
}

bool fruit_t::Collision(int x1, int y1) {
    int x = get_x() + 6 ;
    int y = get_y() - 6;
    if( ((x1-x)*(x1-x) + (y1-y)*(y1-y)) < 41){
      set_status(dead);
      Points_Sound();
			Init(Random()%120, 76, 1, -1, dead, Random()%5);
			return 1;
    }
		return 0;
}

void fruit_t::Init(int x,int y, int vx, int vy, status_t life, int n){
	set_x(x);
	set_y(y);
	set_vx(vx);
	set_vy(vy);
	decrScore = false;
	
	
	//random fruit selected 	
	switch (n) {
  case 0:
    set_image(watermelon);
    break;
  case 1:
    set_image(orange);
    break;
  case 2:
    set_image(pineapple);
    break;
  case 3:
    set_image(banana);
    break;
  case 4:
    set_image(strawberry);
		break;
	}
	set_status(life);
}

void fruit_t::Move(){
	
	/******set y value*****/
			int y = get_y();
			//bottom of screen
			if(y > 76){
				set_status(dead);
				Init((Random()%110)+5, 76, 1, -1, dead, Random()%5);
				decrScore = true;
				return;
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

void fruit_t::Draw() {
	//set_x(6);
	SSD1306_DrawBMP(get_x(), get_y(), get_image(), 0, SSD1306_INVERSE);
}

bool fruit_t::get_error() {
	return decrScore;
}
