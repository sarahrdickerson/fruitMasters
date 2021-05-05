#ifndef BOMB_H
#define BOMB_H

#include <stdint.h>
#include "sprite.h"

class bomb_t : public sprite_t {
	
public:
	bomb_t();
	virtual void Init(int x,int y, int vx, int vy, status_t life, int n);
	virtual void Move(void);
	virtual void Draw(void);
	virtual bool Collision(int x1, int y1);
};

#endif
