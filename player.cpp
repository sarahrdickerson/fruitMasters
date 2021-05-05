#include "player.h"
#include <stdio.h>
#include "SlidePot.h"
#include "SSD1306.h"
#include "fruitImage.h"

player_t::player_t() {
	
}

void player_t::Init(int x,int y, int vx, int vy, status_t life, int n) {
	set_x(x);
	set_y(y);
	set_vx(vx);
	set_vy(vy);
	set_status(life);
	set_image(bow);
	numLives = 3;
	totalPoints = 0;
}

void player_t::Move() {
	int ADCdata = ADC_In();
	int xpos = (ADCdata * 127) / 4095;
	set_x(xpos);
	
}

void player_t::Draw() {
	SSD1306_DrawBMP(get_x(), get_y(), get_image(), 0, SSD1306_INVERSE);
}

bool player_t::Collision(int x1, int y1) {
	return 0;
}

void player_t::setNumLives(uint32_t n) {
	numLives = n;
}

uint32_t player_t::getNumLives() {
	return numLives;
}

void player_t::setPoints(uint32_t n) {
	totalPoints = n;
}

uint32_t player_t::getPoints() {
	return totalPoints;
}
