#include <stdint.h>
#include "sprite.h"

sprite_t::sprite_t() {
	x = 0;
	y = 0;
}

void sprite_t::set_image(const uint8_t* img) {
	image = img;
}

void sprite_t::set_x(int32_t xvar) {
	x = xvar;
}

void sprite_t::set_y(int32_t yvar) {
	y = yvar;
}

void sprite_t::set_vx(int32_t vxvar) {
	vx = vxvar;
}

void sprite_t::set_vy(int32_t vyvar) {
	vy = vyvar;
}

void sprite_t::set_status(status_t s) {
	life = s;
}

int32_t sprite_t::get_x() {
	return x;
}

int32_t sprite_t::get_y() {
	return y;
}

int32_t sprite_t::get_vx() {
	return vx;
}

int32_t sprite_t::get_vy() {
	return vy;
}

bool sprite_t::get_status() {
	return life;
}

const uint8_t* sprite_t::get_image() {
	return image;
}

