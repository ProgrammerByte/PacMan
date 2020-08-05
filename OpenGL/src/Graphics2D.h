#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#pragma once

//CHANGE - ADDED KEY PRESSES SO THAT A CURRENT KEY PRESS CAN BE STORED

class Graphics2D {
private:
	//static unsigned int compileShader(unsigned int type, string name, const string& source);
	//static unsigned int createShaders(const string& vertexShader, const string& fragmentShader);

	float** calculateCircleCoords(int value);
	void setColour(float r, float g, float b, float o);
	void lineTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
	void filledTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
	void lineRect(float x, float y, float xMax, float yMax);
	void filledRect(float x, float y, float xMax, float yMax);
	void linePolygon(float** coords, int vertices);
	void filledPolygon(float** coords, int vertices);
	void renderChar(float xPos, float yPos, int index);
	GLFWwindow* window;
	unsigned int shader;
	unsigned int renderType; //0 = filled, 1 = line, 2 = filled with outline (i.e. both 1 and 0)
	unsigned int resX;
	unsigned int resY;
	float lineColour[4];
	float fillColour[4];
	float textColour[4];
	float** circleCoords; //coordinates of a unit circle
	float aspectRatio;
	float invAspectRatio;
	int circleVerticesCount;

	float cellVertices[14][4]; //coordinates of the 14 segment display
	bool characterSet[36][14]; //determines which segments of the 14 segment display should be switched on for each character

public:
	Graphics2D(unsigned int resX, unsigned int resY, const char* title, bool fullscreen);
	float** convertToDynamic(float coords[][2], int count);
	bool shouldClose();
	void closeWindow();
	void clear();
	void triangle(float x1, float y1, float x2, float y2, float x3, float y3);
	void rect(float x, float y, float width, float height);
	void line(float x1, float y1, float x2, float y2);
	void polygon(float** coords, int vertices);
	void circle(float x, float y, float radius, int gapPos, int gap);
	unsigned int getRenderType();
	void setRenderType(unsigned int value);
	void setLineColour(float r, float g, float b, float o);
	void setFillColour(float r, float g, float b, float o);
	void setTextColour(float r, float g, float b, float o);
	void setClearColour(float r, float g, float b, float o);
	void setLineWidth(float width);
	float getAspectRatio();
	int getCurrentKey();
	int getCircleVerticesCount();
	void setCircleVerticesCount(int value);
	bool keyPress(int key);
	bool mouseButtonPress(int button);
	void getMousePos(double* xPos, double* yPos);
	void resizeText(float size);
	void renderString(float xPos, float yPos, char* contents, int length);
	~Graphics2D();
};