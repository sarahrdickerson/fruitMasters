/*	sprite.h
 *	Runs on TM4C123
 *	Class for all sprites in game
 *	Created 4/28/2021
 *	Student names: Sarah Dickerson & Nidhi Dubagunta
 *
 */
#ifndef SPRITE_H
#define SPRITE_H
#include <stdint.h>

typedef enum{dead, alive} status_t;

class sprite_t {
private:
	int32_t x;	// xpos
	int32_t y;	// ypos
	int32_t vx;	// x velocity
	int32_t vy;	// y velocity
	const uint8_t *image;	// pointer to image of sprite
	bool life;	// status (alive(1) or dead(0))
	
public:
	sprite_t();		// constructor
	//virtual ~sprite_t();
	virtual void Init(int x,int y, int vx, int vy, status_t life, int n) = 0;	// initialize sprite object
	virtual void Move(void) = 0;	// move sprite
	virtual void Draw(void) = 0;	// draw sprite
	virtual bool Collision(int x1, int y1) = 0;	// collision function
	void set_image(const uint8_t* img);	// set image
	void set_x(int32_t xvar);
	void set_y(int32_t yvar);
	void set_vx(int32_t vxvar);
	void set_vy(int32_t vyvar);
	void set_status(status_t s);
	int32_t get_x();
	int32_t get_y();
	int32_t get_vx();
	int32_t get_vy();
	bool get_status();
	const uint8_t* get_image();
		
};

#endif
