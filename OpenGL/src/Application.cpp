//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
#include "maths.h"
#include "Graphics2D.h"
#include "Pacman.h"
#include "Ghost.h"

#include <iostream>
#include <vector>

using namespace std;
//TODO - REPLACE ALL 7'S WITH A CONSTANT!!!
float tileLength;
int xTileInd, yTileInd; //TileInd refers to index within tiles array
int directions[4][2] = { {0, 1}, {-1, 0}, {0, -1}, {1, 0} }; //all possible directions the ghost can travel in, and is in order of priority (up, left, down, right)

/*const int tilesCountX = 8, tilesCountY = 7;
int tiles[tilesCountY][tilesCountX] = {{1, 1, 1, 1, 1, 1, 1, 1},
									   {1, 3, 2, 2, 2, 3, 0, 1},
									   {1, 2, 1, 1, 1, 2, 0, 1},
									   {1, 2, 1, 0, 0, 2, 0, 1},
									   {1, 2, 1, 1, 1, 2, 0, 1},
									   {1, 3, 2, 2, 2, 3, 0, 1},
									   {1, 1, 1, 1, 1, 1, 1, 1}}; //3 = big pellet, 2 = pac-pellet, 1 = wall, 0 = empty*/

const int tilesCountX = 28, tilesCountY = 31;
int originalTiles[tilesCountY][tilesCountX] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
											   {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
											   {1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1},
											   {1, 3, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 3, 1},
											   {1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1},
											   {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
											   {1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 2, 1},
											   {1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 2, 1},
											   {1, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 1},
											   {1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1},
											   {1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1},
											   {1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1},
											   {1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1},
											   {1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1},
											   {0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0},
											   {1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1},
											   {1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1},
											   {1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1},
											   {1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1},
											   {1, 1, 1, 1, 1, 1, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 1, 1, 1, 1, 1},
											   {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
											   {1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1},
											   {1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1},
											   {1, 3, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 3, 1},
											   {1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1},
											   {1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1},
											   {1, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 1},
											   {1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1},
											   {1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1},
											   {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
											   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

int tiles[tilesCountY][tilesCountX];
int pellets, currentPellets;
int lives = 2;
int maxFrightTime = 1500;
int level = 0;
					
int xTileHome, yTileHome; //home of ghost

Graphics2D engine = Graphics2D(1920 / 2, 1080 / 2, "Pac-man", false);
Pacman pacman = Pacman(0, 0, 0, 0);
//Ghost ghost = Ghost(0, 0, 0); //TODO - TEMPORARY
Ghost blinky = Ghost(0, 0, 0, 0, 0, 0, 0, 0, 0);
Ghost inky = Ghost(0, 0, 0, 0, 0, 0, 0, 0, 0);
Ghost pinky = Ghost(0, 0, 0, 0, 0, 0, 0, 0, 0);
Ghost clyde = Ghost(0, 0, 0, 0, 0, 0, 0, 0, 0);

const float tilesAR = (float)tilesCountX / tilesCountY;
float startPosX, startPosY; //top left corner of grid

float threshold; //used for input
float halfTileLength;

float time = 0; //used to determine when scatter mode should occur


void resetBoard() {
	currentPellets = 0;
	pellets = 0;
	for (int i = 0; i < tilesCountY; i++) {
		for (int x = 0; x < tilesCountX; x++) {
			tiles[i][x] = originalTiles[i][x];
			if (tiles[i][x] == 2 || tiles[i][x] == 3) {
				pellets += 1;
			}
		}
	}
}

void resetPositions() {
	//THE FOLLOWING LOOKS MESSY DUE TO THE X AND Y POSITIONS OF EACH BEING CALCULATED, THIS WILL BE SIMPLER IF ALL GHOSTS START AT THE GHOST HOUSE
	pacman = Pacman(startPosX + 14 * tileLength, startPosY - (23 * tileLength + halfTileLength), (float)tileLength / 25, (float)tileLength / 3);
	blinky = Ghost(startPosX + (tileLength * tilesCountX) - 3 * halfTileLength, startPosY - 3 * halfTileLength, tilesCountX - 1, 0, (float)tileLength / 25, maxFrightTime - (200 * level), 1, 0, 0);
	inky = Ghost(startPosX + (tileLength * tilesCountX) - 3 * halfTileLength, startPosY + (3 * halfTileLength) - (tileLength * tilesCountY), tilesCountX - 1, tilesCountY - 1, (float)tileLength / 25, maxFrightTime - (200 * level), 0, 1, 1);
	pinky = Ghost(startPosX + 3 * halfTileLength, startPosY - 3 * halfTileLength, 0, 0, (float)tileLength / 25, maxFrightTime - (200 * level), 1, 0.71875, 1);
	clyde = Ghost(startPosX + 3 * halfTileLength, startPosY + (3 * halfTileLength) - (tileLength * tilesCountY), 0, tilesCountY - 1, (float)tileLength / 25, maxFrightTime - (200 * level), 1, 0.71875, 0.3203125);
}

void initialiseBoard() {
	if (tilesAR > engine.getAspectRatio()) { //TODO - MOVE THIS TO A METHOD
		tileLength = (float)(2 * engine.getAspectRatio()) / tilesCountX;
		startPosY = ((float)tilesCountY / 2) * tileLength;
		startPosX = -engine.getAspectRatio();
	}
	else {
		tileLength = (float)2 / tilesCountY;
		startPosY = 1;
		startPosX = -tilesAR;
	}
	halfTileLength = (float)tileLength / 2;
	threshold = (float)tileLength / 8;
	xTileHome = (tilesCountX / 2);
	yTileHome = (tilesCountY / 2) - 4;
	resetPositions();
	resetBoard();
}

float distFromEdge(float a, float b) { //similar to a % b but for floats, distance from nearest edge
	if (a < 0) {
		a = -a;
	}
	while (a > b) {
		a -= b;
	}
	if (a < b - a) {
		return a;
	}
	return b - a;
}

void calcTilePosition(int& xTileInd, int& yTileInd, float xPos, float yPos) {
	xTileInd = (xPos - startPosX) / tileLength;
	yTileInd = (startPosY - yPos) / tileLength;
	//cout << xTileInd << endl;
	//cout << yTileInd << endl;
	//cout << "" << endl;
}

void adjustGhostPosition(Ghost &ghost, int xTile, int yTile, int dirIndex) {
	if (dirIndex == 0 || dirIndex == 2) { //if moving vertically fix horizontal position
		ghost.setPosition(startPosX + (xTile + 0.5) * tileLength, ghost.getPosition(1));
	}
	else { //if moving horizontally fix vertical position
		ghost.setPosition(ghost.getPosition(0), startPosY - (yTile + 0.5) * tileLength);
	}
}

void updateGhostDir(Ghost &ghost, int x, int y, bool isRandom) { //assuming that the ghost is ready to turn (when at the centre of a tile), x and y refer to tile position
	int xTile, yTile;
	calcTilePosition(xTile, yTile, ghost.getPosition(0), ghost.getPosition(1));

	if (xTile != ghost.getPreviousTile(0) || yTile != ghost.getPreviousTile(1)) { //ensures that the same node isn't checked multiple times
		int oppositeGhostDir = (ghost.getPreviousDir() + 2) % 4;
		if (isRandom == false) { //pick direction that is closest to target
			int minDistSquare = 2147483647;
			int currentDist, currentIndex = -1;

			int currentChangeX, currentChangeY;
			int changeX = x - xTile;
			int changeY = y - yTile;

			for (int i = 0; i < 4; i++) { //for each direction
				if (i != oppositeGhostDir && tiles[(tilesCountY + yTile - directions[i][1]) % tilesCountY][(tilesCountX + xTile + directions[i][0]) % tilesCountX] != 1) {
					currentChangeX = changeX - directions[i][0];
					currentChangeY = changeY + directions[i][1]; //TODO - COULD BE ADD AS IN OPPOSITE DIRECTION

					currentDist = currentChangeX * currentChangeX + currentChangeY * currentChangeY;
					if (currentDist < minDistSquare) {
						minDistSquare = currentDist;
						currentIndex = i;
					}
				}
			}

			if (currentIndex != -1) {
				/*if (currentIndex == 0 || currentIndex == 2) { //if moving vertically fix horizontal position
					ghost.setPosition(startPosX + (xTile + 0.5) * tileLength, ghost.getPosition(1));
				}
				else { //if moving horizontally fix vertical position
					ghost.setPosition(ghost.getPosition(0), startPosY - (yTile + 0.5) * tileLength);
				}*/
				adjustGhostPosition(ghost, xTile, yTile, currentIndex);

				ghost.setDirection(directions[currentIndex][0], directions[currentIndex][1]);
				ghost.setPreviousDir(currentIndex);
			}
			else { //this should only be executed if the ghost encounters a dead end - this will only be useful for user made mazes
				ghost.setDirection(directions[oppositeGhostDir][0], directions[oppositeGhostDir][1]);
				ghost.setPreviousDir(oppositeGhostDir);
			}
		}

		else { //pick random valid direction
			int totalValid = 0;
			vector<int> valid;
			for (int i = 0; i < 4; i++) {
				if (i != oppositeGhostDir && tiles[(yTile - directions[i][1]) % tilesCountY][(xTile + directions[i][0]) % tilesCountX] != 1) {
					valid.push_back(i);
					totalValid += 1;
				}
			}
			if (totalValid != 0) {
				int chosenIndex = valid[rand() % totalValid];
				adjustGhostPosition(ghost, xTile, yTile, chosenIndex);
				ghost.setDirection(directions[chosenIndex][0], directions[chosenIndex][1]);
				ghost.setPreviousDir(chosenIndex);
			}
			else {
				ghost.setDirection(directions[oppositeGhostDir][0], directions[oppositeGhostDir][1]);
				ghost.setPreviousDir(oppositeGhostDir);
			}
		}
		ghost.setPreviousTile(xTile, yTile);
	}

}

void checkGhost(Ghost &ghost, int x, int y) { //if ghost can change direction then it will try to, x and y refer to target tile
	float dist = distFromEdge(ghost.getPosition(0) - startPosX, tileLength) - halfTileLength;
	if (dist <= threshold && dist >= -threshold) {
		float dist = distFromEdge(startPosY - ghost.getPosition(1), tileLength) - halfTileLength;
		if (dist <= threshold && dist >= -threshold) {
			//int xTile, yTile;
			//calcTilePosition(xTile, yTile, pacman.getPosition(0), pacman.getPosition(1));
			if (ghost.getMode() != 1) {
				updateGhostDir(ghost, x, y, false);
			}
			else {
				updateGhostDir(ghost, x, y, true);
			}
		}
	}
}

void eatenGhost(Ghost &ghost) {
	int xTile, yTile;
	calcTilePosition(xTile, yTile, ghost.getPosition(0), ghost.getPosition(1));
	if (xTile == xTileHome && yTile == yTileHome) {
		ghost.setMode(0);
	}
	else {
		checkGhost(ghost, xTileHome, yTileHome);
	}
}

void scatter(Ghost& ghost) {
	if (ghost.getMode() != 2) {
		checkGhost(ghost, ghost.getHomeTile(0), ghost.getHomeTile(1));
	}
	else {
		eatenGhost(ghost);
	}
}

void updateGhosts() { //updates all ghosts, CURRENTLY ONLY CHASE MODE IS SUPPORTED
	int xTile, yTile;

	if (time >= 20) {
		scatter(blinky);
		scatter(inky);
		scatter(pinky);
		scatter(clyde);

		if (time >= 27 - level) {
			time = 0;
		}
	}

	else {
		if (blinky.getMode() != 2) {
			checkGhost(blinky, xTileInd, yTileInd); //targets pacman
		}
		else {
			eatenGhost(blinky);
		}

		if (inky.getMode() != 2) {
			xTile = xTileInd + (pacman.getDirection(0) * 2);
			yTile = yTileInd - (pacman.getDirection(1) * 2); //targets two tiles in front of pacman
			xTile = (2 * xTile) - blinky.getPreviousTile(0);
			yTile = (2 * yTile) - blinky.getPreviousTile(1); //TODO - THIS COULD BE ADD
			checkGhost(inky, xTile, yTile); //targets pacman depending on the position of blinky
		}
		else {
			eatenGhost(inky);
		}

		if (pinky.getMode() != 2) {
			xTile = xTileInd + (pacman.getDirection(0) * 4);
			yTile = yTileInd - (pacman.getDirection(1) * 4);
			checkGhost(pinky, xTile, yTile); //targets 4 tiles infront of pacman
		}
		else {
			eatenGhost(pinky);
		}

		if (clyde.getMode() != 2) {
			calcTilePosition(xTile, yTile, clyde.getPosition(0), clyde.getPosition(1));
			int changeX = xTile - xTileInd;
			int changeY = yTile - yTileInd;
			if (changeX * changeX + changeY * changeY <= 64) { //go back to corner
				checkGhost(clyde, startPosX + 3 * halfTileLength, startPosY + (3 * halfTileLength) - (tileLength * tilesCountY));
			}
			else { //go towards pacman
				checkGhost(clyde, xTileInd, yTileInd);
			}
		}
		else {
			eatenGhost(clyde);
		}
	}


	blinky.update(startPosX, -startPosY, -startPosX, startPosY);
	inky.update(startPosX, -startPosY, -startPosX, startPosY);
	pinky.update(startPosX, -startPosY, -startPosX, startPosY);
	clyde.update(startPosX, -startPosY, -startPosX, startPosY);

	time += 0.01;
}

bool ghostCollision(Ghost& ghost) {
	switch (ghost.getMode()) {
		case 0:
			return ghost.colliding(pacman);

		case 1:
			if (ghost.colliding(pacman)) {
				ghost.eaten();
			}
			return false;
	}
	return false;
}

bool checkForCollisions() {
	return ghostCollision(blinky) || ghostCollision(inky) || ghostCollision(pinky) || ghostCollision(clyde);
}

void renderGhost(Ghost &ghost) {
	switch (ghost.getMode()) {
		case 0:
			engine.setFillColour(ghost.getR(), ghost.getG(), ghost.getB(), 1);
			break;

		case 1:
			engine.setFillColour(0, 0, 1, 1);
			break;

		case 2:
			engine.setFillColour(0.2 * ghost.getR(), 0.2 * ghost.getG(), 0.2 * ghost.getB(), 1);
			break;
	}
	engine.circle(ghost.getPosition(0), ghost.getPosition(1), pacman.getRadius(), 0, 0);
}

void renderGhosts() {
	/*if (blinky.getMode() == 0) {
		engine.setFillColour(1, 0, 0, 1);
	}
	else {
		engine.setFillColour(0, 0, 1, 1);
	}
	engine.circle(blinky.getPosition(0), blinky.getPosition(1), pacman.getRadius(), 0, 0);
	engine.setFillColour(0, 1, 1, 1);
	engine.circle(inky.getPosition(0), inky.getPosition(1), pacman.getRadius(), 0, 0);
	engine.setFillColour(1, 0.71875, 1, 1);
	engine.circle(pinky.getPosition(0), pinky.getPosition(1), pacman.getRadius(), 0, 0);
	engine.setFillColour(1, 0.71875, 0.3203125, 1);
	engine.circle(clyde.getPosition(0), clyde.getPosition(1), pacman.getRadius(), 0, 0);*/
	renderGhost(blinky);
	renderGhost(inky);
	renderGhost(pinky);
	renderGhost(clyde);
}

void input() {
	if (engine.keyPress('W')) {
		float dist = distFromEdge(pacman.getPosition(0) - startPosX, tileLength) - halfTileLength;
		if (dist <= threshold && dist >= -threshold && tiles[yTileInd - 1][xTileInd] != 1) {
			pacman.setPosition(startPosX + (xTileInd + 0.5) * tileLength, pacman.getPosition(1));
			pacman.setDirection(0, 1);
			pacman.setStart(12); //circleCoords / 4
		}
	}

	else if (engine.keyPress('A')) {
		float dist = distFromEdge(startPosY - pacman.getPosition(1), tileLength) - halfTileLength;
		if (dist <= threshold && dist >= -threshold && tiles[yTileInd][xTileInd - 1] != 1) {
			pacman.setPosition(pacman.getPosition(0), startPosY - (yTileInd + 0.5) * tileLength);
			pacman.setDirection(-1, 0);
			pacman.setStart(24); //circleCoords / 2
		}
	}

	else if (engine.keyPress('S')) {
		float dist = distFromEdge(pacman.getPosition(0) - startPosX, tileLength) - halfTileLength;
		if (dist <= threshold && dist >= -threshold && tiles[yTileInd + 1][xTileInd] != 1) {
			pacman.setPosition(startPosX + (xTileInd + 0.5) * tileLength, pacman.getPosition(1));
			pacman.setDirection(0, -1);
			pacman.setStart(36); //3 * circleCoords / 4
		}
	}

	else if (engine.keyPress('D')) {
		float dist = distFromEdge(startPosY - pacman.getPosition(1), tileLength) - halfTileLength;
		if (dist <= threshold && dist >= -threshold && tiles[yTileInd][xTileInd + 1] != 1) {
			pacman.setPosition(pacman.getPosition(0), startPosY - (yTileInd + 0.5) * tileLength);
			pacman.setDirection(1, 0);
			pacman.setStart(0); //c0
		}
	}

	if (engine.keyPress(GLFW_KEY_ESCAPE)) {
		engine.closeWindow();
	}
}

/*int calcTilePosition(float position) {
	float numerator = position + (0.5 * tileLength);
	if (numerator < 0) {
		numerator -= tileLength;
	}
	return numerator / tileLength;
}*/

void renderTiles() {
	float currentXPos;
	float currentYPos = startPosY - tileLength;
	float pelletRad = (float)tileLength / 12; //radius of small pellet as it's used multiple times
	for (int i = 0; i < tilesCountY; i++) {
		currentXPos = startPosX;
		for (int x = 0; x < tilesCountX; x++) {
			switch (tiles[i][x]) {
				case 1:
					engine.setFillColour(0, 0, 1, 1);
					engine.rect(currentXPos, currentYPos, tileLength, tileLength);
					break;

				case 2:
					engine.setFillColour(1, 1, 1, 1);
					engine.rect((currentXPos + halfTileLength) - pelletRad, (currentYPos + halfTileLength) - pelletRad, 2 * pelletRad, 2 * pelletRad);
					break;

				case 3:
					engine.setFillColour(1, 1, 1, 1);
					engine.circle(currentXPos + halfTileLength, currentYPos + halfTileLength, (float)tileLength / 4, 0, 0);
					break;
			}
			currentXPos += tileLength;
		}
		currentYPos -= tileLength;
	}
}

bool canMove() {
	//int xTile = calcTilePosition(pacman.getPosition(0) + pacman.getDirection(0) * (float)(halfTileLength));
	//int yTile = calcTilePosition(pacman.getPosition(1) + pacman.getDirection(1) * (float)(halfTileLength));
	int xTile, yTile;
	calcTilePosition(xTile, yTile, pacman.getPosition(0) + pacman.getDirection(0) * halfTileLength, pacman.getPosition(1) + pacman.getDirection(1) * halfTileLength);

	if (tiles[yTile % tilesCountY][xTile % tilesCountX] == 1) {
		return false;
	}
	return true;
}

//for now the coordinates system goes from (-aspectRatio, -1) to (aspectRatio, 1)
int main() {
	initialiseBoard();
	engine.setFillColour(1, 1, 0, 1);
	engine.setCircleVerticesCount(48);

	while (engine.shouldClose() == false) {
		renderTiles();

		calcTilePosition(xTileInd, yTileInd, pacman.getPosition(0), pacman.getPosition(1));

		if (tiles[yTileInd][xTileInd] == 2) {
			currentPellets += 1;
			tiles[yTileInd][xTileInd] = 0;
		}
		else if (tiles[yTileInd][xTileInd] == 3) {
			currentPellets += 1;
			tiles[yTileInd][xTileInd] = 0;
			blinky.frighten();
			inky.frighten();
			pinky.frighten();
			clyde.frighten();
		}

		input();
		if (canMove() == true) {
			pacman.update(startPosX, -startPosY, -startPosX, startPosY);
		}

		//checkGhost(ghost, xTileInd, yTileInd);
		updateGhosts();
		renderGhosts();
		//ghost.update(startPosX, -startPosY, -startPosX, startPosY);
		//engine.setFillColour(1, 0, 0, 1);
		//engine.circle(ghost.getPosition(0), ghost.getPosition(1), pacman.getRadius(), 0, 0);

		//engine.setFillColour(1, 0, 0, 1);
		//engine.rect(startPosX + xTileInd * tileLength, startPosY - (yTileInd + 1) * tileLength, tileLength, tileLength); //shows position of Pacman

		engine.setFillColour(1, 1, 0, 1);
		engine.circle(pacman.getPosition(0), pacman.getPosition(1), pacman.getRadius(), pacman.getStart(), pacman.getGap());


		engine.setFillColour(0, 0, 0, 1); //The following prevents everything on the outside of the board from being seen
		engine.rect(startPosX - 1, -startPosY, 1, 2);
		engine.rect(startPosX, startPosY, 2 * engine.getAspectRatio(), 2);
		engine.rect(-startPosX, -startPosY, 1, 2);
		engine.rect(startPosX, -startPosY - 2, 2 * engine.getAspectRatio(), 2);

		engine.setFillColour(1, 1, 0, 1);
		for (int i = 0; i < lives; i++) {
			engine.circle(-engine.getAspectRatio() + (3 * i + 1) * pacman.getRadius(), 1 - pacman.getRadius(), pacman.getRadius(), 0, 7);
		}

		engine.clear();

		if (currentPellets == pellets) {
			level += 1;
			resetPositions();
			lives += 1;
			resetBoard();
		}

		if (checkForCollisions() == true) {
			if (lives == 0) {
				engine.closeWindow();
			}
			lives -= 1;
			resetPositions();
		}
	}
}