#include "Pacman.h"

Pacman::Pacman(float x, float y, float speed, float radius) {
	if (radius != 0) {
		this->direction[0] = 1;
		this->direction[1] = 0;
		this->position[0] = x;
		this->position[1] = y;
		this->speed = speed;
		this->radius = radius;
		this->gap = 0.5;
		this->start = 0;
		this->gapDir = 1;
	}
}

void Pacman::update(float xMin, float yMin, float xMax, float yMax) {
	position[0] += direction[0] * speed;
	if (position[0] > xMax + radius) {
		position[0] = xMin - radius;
	}
	else if (position[0] < xMin - radius) {
		position[0] = xMax + radius;
	}

	position[1] += direction[1] * speed;
	if (position[1] > yMax + radius) {
		position[1] = yMin - radius;
	}
	else if (position[1] < yMin - radius) {
		position[1] = yMax + radius;
	}

	if (gap >= 8) {
		gapDir = -1;
	}
	else if (gap <= 1) {
		//cout << "ooi" << endl;
		gapDir = 1;
	}

	gap += 0.5 * gapDir;

	//if (gap < 0) {
	//	cout << "FUUKE" << endl;
	//}
}

void Pacman::setDirection(int x, int y) {
	direction[0] = x;
	direction[1] = y;
}

int Pacman::getDirection(int index) {
	return direction[index];
}

void Pacman::setStart(int value) {
	start = value;
}

int Pacman::getStart() {
	return start;
}

void Pacman::setPosition(float x, float y) {
	position[0] = x;
	position[1] = y;
}

float Pacman::getPosition(int index) {
	return position[index];
}

float Pacman::getRadius() {
	return radius;
}

int Pacman::getGap() {
	return (int)gap;
}