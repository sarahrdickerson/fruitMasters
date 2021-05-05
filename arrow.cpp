// arrow.cpp
#include "arrow.h"
#include "SSD1306.h"
#include "fruitImage.h"

arrow_t::arrow_t() {
	
}

void arrow_t::Init(int x,int y, int vx, int vy, status_t life, int n) {
	set_x(x);
	set_y(y);
	set_vx(vx);
	set_vy(vy);
	set_status(life);
	set_image(arrow);
	
}

void arrow_t::Move() {
		if(get_y() < 7){
			set_status(dead);
		} else{
			set_y(get_y() + get_vy());
		}
}

void arrow_t::Draw() {
	SSD1306_DrawBMP(get_x(), get_y(), get_image(), 0, SSD1306_INVERSE);
}

bool arrow_t::Collision(int x1, int y1) {
	return 0;
}
