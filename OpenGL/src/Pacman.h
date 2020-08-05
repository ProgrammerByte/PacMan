#pragma once

class Pacman {
private:
	float position[2];
	int direction[2];
	double gap;
	int gapDir; //whether the mouth is opening or closing
	int start; //where the circle should start (i.e. where the gap is going to be)
	float speed;
	float radius;

public:
	Pacman(float x, float y, float speed, float radius);
	void update(float xMin, float yMin, float xMax, float yMax);

	void setDirection(int x, int y);
	int getDirection(int index);
	int getGap();
	void setPosition(float x, float y);
	float getPosition(int index);
	float getRadius();
	void setStart(int value);
	int getStart();

	~Pacman() {};
};