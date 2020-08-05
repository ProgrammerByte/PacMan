#include "Ghost.h"
#include "Pacman.h"

Ghost::Ghost(float x, float y, int xHome, int yHome, float speed, int maxFrightTime, float r, float g, float b) {
	this->maxFrightTime = maxFrightTime;
	this->frightenedTime = 0;
	this->mode = 0;
	this->position[0] = x;
	this->position[1] = y;
	this->speed = speed;
	this->direction[0] = 0;
	this->direction[1] = 1;
	this->previousDir = 0;
	this->prevTile[0] = -1;
	this->prevTile[1] = -1;
	this->homeTile[0] = xHome;
	this->homeTile[1] = yHome;
	this->r = r;
	this->g = g;
	this->b = b;
}

void Ghost::update(float xMin, float yMin, float xMax, float yMax) {
	float currentSpeed = speed;
	if (mode == 1) {
		currentSpeed *= 0.8;
		frightenedTime += 1;
		if (frightenedTime >= maxFrightTime) {
			//frightenedTime = 0;
			mode = 0;
		}
	}

	else if (mode == 2) { //done to ensure that the ghost doesn't take too long returning home
		currentSpeed *= 1.5;
		frightenedTime += 1;
		if (frightenedTime >= 1500) {
			//frightenedTime = 0;
			mode = 0;
		}
	}



	position[0] += direction[0] * currentSpeed; //TODO - screenwrap isn't too smooth for ghosts currently as the radius has been removed
	if (position[0] > xMax) {
		position[0] = xMin;
	}
	else if (position[0] < xMin) {
		position[0] = xMax;
	}

	position[1] += direction[1] * currentSpeed;
	if (position[1] > yMax) {
		position[1] = yMin;
	}
	else if (position[1] < yMin) {
		position[1] = yMax;
	}
}

bool Ghost::colliding(Pacman pacman) {
	float changeX = pacman.getPosition(0) - position[0];
	float changeY = pacman.getPosition(1) - position[1];
	float threshold = 2 * pacman.getRadius(); //max distance between ghost and pacman in order to be colliding
	if (changeX * changeX + changeY * changeY <= threshold * threshold) {
		return true;
	}
	return false;
}

void Ghost::frighten() {
	if (mode == 0) {
		mode = 1;
	}
	frightenedTime = 0;
}

void Ghost::eaten() {
	mode = 2;
	frightenedTime = 0;
}

/*void Ghost::updateDir(float x, float y) {
	float changeX = x - position[0];
	float changeY = y - position[1];
	
	int directions[4][2] = { {0, 1}, {-1, 0}, {0, -1}, {1, 0} }; //all possible directions the ghost can travel in, and is in order of priority (up, left, down, right)
	float currentChangeX, currentChangeY;

}*/




void Ghost::setPosition(float x, float y) {
	position[0] = x;
	position[1] = y;
}

float Ghost::getPosition(int index) {
	return position[index];
}

void Ghost::setDirection(float x, float y) {
	direction[0] = x;
	direction[1] = y;
}

int Ghost::getDirection(int index) {
	return direction[index];
}

void Ghost::setPreviousDir(int index) {
	previousDir = index;
}

int Ghost::getPreviousDir() {
	return previousDir;
}

void Ghost::setPreviousTile(int x, int y) {
	prevTile[0] = x;
	prevTile[1] = y;
}

int Ghost::getPreviousTile(int index) {
	return prevTile[index];
}

void Ghost::setMode(int value) {
	mode = value;
}

int Ghost::getMode() {
	return mode;
}

int Ghost::getHomeTile(int index) {
	return homeTile[index];
}

float Ghost::getR() {
	return r;
}

float Ghost::getG() {
	return g;
}

float Ghost::getB() {
	return b;
}