/*
 *	arrow_t publicly inherits from sprite_t to implement special functions
 */
#ifndef ARROW_H
#define ARROW_H

#include <stdint.h>
#include "sprite.h"

class arrow_t : public sprite_t {
	
public:
	arrow_t();
	virtual void Init(int x,int y, int vx, int vy, status_t life, int n);
	virtual void Move(void);
	virtual void Draw(void);
	virtual bool Collision(int x1, int y1);
		
};

#endif
