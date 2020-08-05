#pragma once
#include "Pacman.h"

class Ghost {
private:
	int maxFrightTime;
	int frightenedTime;
	int mode; //0 = chase / scatter, 1 = frightened, 2 = eaten
	int previousDir; //index of previous direction in the directions array
	int homeTile[2];
	int direction[2];
	int prevTile[2]; //previous tile which the ghost has turned at
	float position[2];
	float speed;
	float r;
	float g;
	float b;

public:
	Ghost(float x, float y, int xHome, int yHome, float speed, int maxFrightTime, float r, float g, float b); //home refers to the ghost's corner
	void update(float xMin, float yMin, float xMax, float yMax);
	bool colliding(Pacman pacman);
	void frighten();
	void eaten();
	//void updateDir(float x, float y);

	void setPosition(float x, float y);
	float getPosition(int index);
	void setDirection(float x, float y);
	int getDirection(int index);
	void setPreviousDir(int index);
	int getPreviousDir();
	void setPreviousTile(int x, int y);
	int getPreviousTile(int index);
	void setMode(int value);
	int getMode();
	int getHomeTile(int index);
	float getR();
	float getG();
	float getB();

	~Ghost() {}
};