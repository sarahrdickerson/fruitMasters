#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include "sprite.h"

class player_t : public sprite_t{
	uint32_t numLives;
	uint32_t totalPoints;
public:
	player_t();
	virtual void Init(int x,int y, int vx, int vy, status_t life, int n);
	virtual void Move(void);
	virtual void Draw(void);
	virtual bool Collision(int x1, int y1);
	void setNumLives(uint32_t n);
	uint32_t getNumLives();
	void setPoints(uint32_t n);
	uint32_t getPoints(void);
};

#endif
