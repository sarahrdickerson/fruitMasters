/*
 *	fruit_t publicly inherits from sprite_t to implement special functions
 */
#ifndef FRUIT_H
#define FRUIT_H

#include <stdint.h>
#include "sprite.h"

class fruit_t : public sprite_t {
	bool decrScore;
	
public:
	fruit_t();
	virtual void Init(int x,int y, int vx, int vy, status_t life, int n);
	virtual void Move(void);
	virtual void Draw(void);
	virtual bool Collision(int x1, int y1);
	bool get_error();
		
};

#endif
