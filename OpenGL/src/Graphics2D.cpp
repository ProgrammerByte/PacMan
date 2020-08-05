#include "Graphics2D.h"
#include "maths.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

using namespace std;

//TODO - DOESN'T CONTAIN ALL FEATURES PRESENT IN GITHUB VERSION

float** Graphics2D::convertToDynamic(float coords[][2], int count) { //REQUIRED FOR THE POLYGON METHOD - TODO - MOVE TO THE GRAPHICS2D CLASS
    float** result = new float* [count];
    for (int i = 0; i < count; i++) {
        result[i] = new float[2];
        result[i][0] = coords[i][0];
        result[i][1] = coords[i][1];
    }
    return result;
}

float** Graphics2D::calculateCircleCoords(int value) {
    float** result = new float* [value];
    double increment = (double)(2 * 3.14159265358979) / value;
    double currentAngle = 0;
    for (int i = 0; i < value; i++) {
        result[i] = new float[2];
        result[i][0] = cos(currentAngle, 100000);
        result[i][1] = sin(currentAngle, 100000);
        currentAngle += increment;
    }
    return result;
}

void Graphics2D::resizeText(float size) {
    float tempCellVertices[14][4] = { {0, 2, 1, 2}, //corresponds to diagram of 14-segment display
                                     {0, 2, 0, 1},
                                     {1, 2, 1, 1},
                                     {0, 1, 0.5, 1},
                                     {0.5, 1, 1, 1},
                                     {0, 0, 0, 1},
                                     {1, 0, 1, 1},
                                     {0, 0, 1, 0},
                                     {0, 2, 0.5, 1},
                                     {0.5, 1, 0.5, 2},
                                     {0.5, 1, 1, 2},
                                     {0, 0, 0.5, 1},
                                     {0.5, 1, 0.5, 0},
                                     {0.5, 1, 1, 0} };
    for (int i = 0; i < 14; i++) {
        for (int x = 0; x < 4; x++) {
            cellVertices[i][x] = tempCellVertices[i][x] * size; //scales down the base size of the text
        }
    }
}


static unsigned int compileShader(unsigned int type, string name, const string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = &source[0]; //pointer to first character of glsl code
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == false) { //if an error occured during compilation
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        const int lengthTemp = length;
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        cout << "The compilation of the " << name << " shader has failed, error message as follows:" << endl;
        cout << message << endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}
static unsigned int createShaders(const string& vertexShader, const string& fragmentShader) {
    unsigned int program = glCreateProgram();
    //glUniform4f(glGetUniformLocation(program, "inColour"), 1, 0, 0, 1);
    unsigned int vs = compileShader(GL_VERTEX_SHADER, "vertex", vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, "fragment", fragmentShader);

    if (vs != 0 && fs != 0) { //if there has been no compilation errors

        glAttachShader(program, vs);
        glAttachShader(program, fs);

        glLinkProgram(program);
        glValidateProgram(program);

        glDetachShader(program, vs);
        glDetachShader(program, fs);

        return program;
    }
    else {
        return 0; //error code
    }
}

int currentKey = -1; //defines currentKey
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) { //WHERE INPUT IS HANDLED
    if (glfwGetKey(window, key) == GLFW_PRESS) {
        currentKey = key;
    }
}

//GLFWwindow* window;
//unsigned int shader;

//public:
Graphics2D::Graphics2D(unsigned int resX, unsigned int resY, const char* title, bool fullscreen) { //initialises window as well as OpenGL TODO - DONE FOR NOW, HOWEVER ADD MORE METHODS!!!
    glfwInit();
    if (fullscreen == true) {
        this->window = glfwCreateWindow(resX, resY, title, glfwGetPrimaryMonitor(), NULL);
    }
    else {
        this->window = glfwCreateWindow(resX, resY, title, NULL, NULL);
    }
    this->invAspectRatio = (float)resY / resX;
    this->aspectRatio = (float)resX / resY;
    this->resX = resX;
    this->resY = resY;
    if (!window) {
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    glewInit();

    unsigned int buffer;
    glGenBuffers(1, &buffer); //generate a single triangle buffer 
    glBindBuffer(GL_ARRAY_BUFFER, buffer); //binds buffer to OpenGL

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    glLineWidth(1);
    renderType = 0;

    string vertexShader = "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position; \n"
        "void main() { \n"
        "   gl_Position = position; \n"
        "}\n";

    string fragmentShader = "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 outColour; \n"
        "\n"
        "uniform vec4 inColour; \n"
        "\n"
        "void main() { \n"
        "   outColour = inColour; \n"
        "}\n";

    shader = createShaders(vertexShader, fragmentShader);
    if (shader != 0) { //if successful
        glUseProgram(shader);
        glUniform4f(glGetUniformLocation(shader, "inColour"), 1, 1, 1, 1); //white is default colour
    }

    for (int i = 0; i < 4; i++) {
        fillColour[i] = 1;
        lineColour[i] = 1;
        textColour[i] = 1;
    }

    circleVerticesCount = 25;
    circleCoords = calculateCircleCoords(circleVerticesCount);



    glfwSetKeyCallback(window, keyCallback); //ALLOWS FOR A METHOD TO BE EXECUTED IF ANY KEY HAS BEEN PRESSED

    //All following code initialises the text attributes.
    resizeText(0.25);

    bool tempCharacterSet[36][14] = { {1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0}, //0
                                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0}, //1
                                        {1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0}, //2
                                        {1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0}, //3
                                        {0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0}, //4
                                        {1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0}, //5
                                        {1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}, //6
                                        {1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, //7
                                        {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}, //8
                                        {1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0}, //9
                                        {1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0}, //A
                                        {1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0}, //B
                                        {1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0}, //C
                                        {1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0}, //D
                                        {1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0}, //E
                                        {1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, //F
                                        {1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}, //G
                                        {0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0}, //H
                                        {1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0}, //I
                                        {0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0}, //J
                                        {0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1}, //K
                                        {0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0}, //L
                                        {0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0}, //M
                                        {0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1}, //N
                                        {1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0}, //O
                                        {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}, //P
                                        {1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1}, //Q
                                        {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1}, //R
                                        {1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0}, //S
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0}, //T
                                        {0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0}, //U
                                        {0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0}, //V
                                        {0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1}, //W
                                        {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1}, //X
                                        {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0}, //Y
                                        {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0} };//Z

    for (int i = 0; i < 36; i++) {
        for (int x = 0; x < 14; x++) {
            characterSet[i][x] = tempCharacterSet[i][x];
        }
    }
};

bool Graphics2D::shouldClose() {
    return glfwWindowShouldClose(window);
}

void Graphics2D::closeWindow() {
    glfwSetWindowShouldClose(window, GL_TRUE);
}

void Graphics2D::setColour(float r, float g, float b, float o) {
    glUniform4f(glGetUniformLocation(shader, "inColour"), r, g, b, o);
}

void Graphics2D::clear() {

    glfwSwapBuffers(window);

    currentKey = -1;
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT); //TODO - THIS MIGHT HAVE TO BE CALLED SEPARATELY
}

void Graphics2D::line(float x1, float y1, float x2, float y2) { //uses what the current colour is, if type = 0 then the fill colour will be used
    this->setColour(lineColour[0], lineColour[1], lineColour[2], lineColour[3]); //this is needed as this can be called if the render type is not 1

    float positions[] = { x1 * invAspectRatio, y1, x2 * invAspectRatio, y2 };
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float), positions, GL_STATIC_DRAW);
    glDrawArrays(GL_LINES, 0, 2);

    this->setColour(fillColour[0], fillColour[1], fillColour[2], fillColour[3]);
}

void Graphics2D::lineTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {
    line(x1, y1, x2, y2);
    line(x2, y2, x3, y3);
    line(x3, y3, x1, y1);
}

void Graphics2D::filledTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {
    float positions[] = { x1, y1, x2, y2, x3, y3 };
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Graphics2D::triangle(float x1, float y1, float x2, float y2, float x3, float y3) { //TODO - ADD OPTION FOR LINE OR FILLED
    //float positions[] = { x1, y1, x2, y2, x3, y3 };
    //glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    x1 *= invAspectRatio;
    x2 *= invAspectRatio;
    x3 *= invAspectRatio;
    if (renderType == 0) {
        filledTriangle(x1, y1, x2, y2, x3, y3);
    }
    else if (renderType == 1) {
        lineTriangle(x1, y1, x2, y2, x3, y3);
    }
    else if (renderType == 2) {
        setColour(fillColour[0], fillColour[1], fillColour[2], fillColour[3]);
        filledTriangle(x1, y1, x2, y2, x3, y3);
        setColour(lineColour[0], lineColour[1], lineColour[2], lineColour[3]);
        lineTriangle(x1, y1, x2, y2, x3, y3);
    }
}

void Graphics2D::filledRect(float x, float y, float xMax, float yMax) {
    filledTriangle(x, y, x, yMax, xMax, yMax);
    filledTriangle(x, y, xMax, y, xMax, yMax);
}

void Graphics2D::lineRect(float x, float y, float xMax, float yMax) {
    line(x, y, x, yMax);
    line(x, yMax, xMax, yMax);
    line(xMax, yMax, xMax, y);
    line(xMax, y, x, y);
}

void Graphics2D::rect(float x, float y, float width, float height) {
    float xMax = x + width;
    float yMax = y + height;
    if (renderType == 0) {
        filledRect(x * invAspectRatio, y, xMax * invAspectRatio, yMax); //TODO - ADD THE *INVASPECTRATIO TO GITHUB
    }
    else if (renderType == 1) {
        lineRect(x * invAspectRatio, y, xMax * invAspectRatio, yMax); //AS WELL AS THIS
    }
    else if (renderType == 2) {
        setColour(fillColour[0], fillColour[1], fillColour[2], fillColour[3]);
        filledRect(x * invAspectRatio, y, xMax * invAspectRatio, yMax); //AND HERE AS WELL
        setColour(lineColour[0], lineColour[1], lineColour[2], lineColour[3]);
        lineRect(x * invAspectRatio, y, xMax * invAspectRatio, yMax); //AND HERE
    }
}

void Graphics2D::filledPolygon(float** coords, int vertices) {
    for (int i = 1; i < vertices - 1; i++) {
        filledTriangle(coords[0][0] * invAspectRatio, coords[0][1], coords[i][0] * invAspectRatio, coords[i][1], coords[i + 1][0] * invAspectRatio, coords[i + 1][1]);
    }
}

void Graphics2D::linePolygon(float** coords, int vertices) { //TODO - WORKED WHEN float coords[][2] was used instead
    for (int i = 0; i < vertices - 1; i++) {
        line(coords[i][0], coords[i][1], coords[i + 1][0], coords[i + 1][1]);
    }
    line(coords[vertices - 1][0], coords[vertices - 1][1], coords[0][0], coords[0][1]);
}


void Graphics2D::polygon(float** coords, int vertices) {
    if (renderType == 0) {
        filledPolygon(coords, vertices);
    }
    else if (renderType == 1) {
        linePolygon(coords, vertices);
    }
    else if (renderType == 2) {
        setColour(fillColour[0], fillColour[1], fillColour[2], fillColour[3]);
        filledPolygon(coords, vertices);
        setColour(lineColour[0], lineColour[1], lineColour[2], lineColour[3]);
        linePolygon(coords, vertices);
    }
}

//the following has been modified for pacman, normal circle if gap = 0
void Graphics2D::circle(float x, float y, float radius, int gapPos, int gap) {
    float** currentCoords;
    int vertexCount, gapDiff;
    if (gap == 0) {
        currentCoords = new float* [circleVerticesCount]; //calculates the coordinates of the given circle
        vertexCount = circleVerticesCount;
        gapDiff = 0;
    }
    else {
        vertexCount = circleVerticesCount - 2 * (gap - 1);
        currentCoords = new float* [vertexCount];
        currentCoords[0] = new float[2];
        currentCoords[0][0] = x;
        currentCoords[0][1] = y;
        gapDiff = 1 - gap;
    }

    for (int i = gap; i < vertexCount - gapDiff; i++) {
        //cout << i + gapDiff << endl;
        currentCoords[i + gapDiff] = new float[2];
        currentCoords[i + gapDiff][0] = (circleCoords[(gapPos + i) % circleVerticesCount][0] * radius) + x;
        currentCoords[i + gapDiff][1] = (circleCoords[(gapPos + i) % circleVerticesCount][1] * radius) + y;
    }
    polygon(currentCoords, vertexCount);
}








unsigned int Graphics2D::getRenderType() {
    return renderType;
}

void Graphics2D::setRenderType(unsigned int value) { //also changes colour if needed
    renderType = value;
    if (value == 0) {
        setColour(fillColour[0], fillColour[1], fillColour[2], fillColour[3]);
    }
    else if (value == 1) {
        setColour(lineColour[0], lineColour[1], lineColour[2], lineColour[3]);
    }
}

void Graphics2D::setFillColour(float r, float g, float b, float o) {
    fillColour[0] = r;
    fillColour[1] = g;
    fillColour[2] = b;
    fillColour[3] = o;
    if (renderType == 0 || renderType == 2) {
        this->setColour(r, g, b, o);
    }
}

void Graphics2D::setLineColour(float r, float g, float b, float o) {
    lineColour[0] = r;
    lineColour[1] = g;
    lineColour[2] = b;
    lineColour[3] = o;
    if (renderType == 1) { //TODO - ADD THESE LINES TO GITHUB
        this->setColour(r, g, b, o);
    }
}

void Graphics2D::setTextColour(float r, float g, float b, float o) {
    textColour[0] = r;
    textColour[1] = g;
    textColour[2] = b;
    textColour[3] = 0;
}

void Graphics2D::setClearColour(float r, float g, float b, float o) {
    glClearColor(r, g, b, o);
}

void Graphics2D::setLineWidth(float width) {
    glLineWidth(width);
}

int Graphics2D::getCircleVerticesCount() {
    return circleVerticesCount;
}

float Graphics2D::getAspectRatio() {
    return aspectRatio;
}

void Graphics2D::setCircleVerticesCount(int value) {
    if (value >= 3) {
        circleVerticesCount = value;
        circleCoords = calculateCircleCoords(value);
    }
}





//the following is for input
bool Graphics2D::keyPress(int key) { //checks if a specific key has been pressed
    return glfwGetKey(window, key);
}
int Graphics2D::getCurrentKey() { //waits continuously for keyboard input
    return currentKey;
}
bool Graphics2D::mouseButtonPress(int button) {
    return glfwGetMouseButton(window, button);
}
void Graphics2D::getMousePos(double* xPos, double* yPos) {
    glfwGetCursorPos(window, xPos, yPos);
    *xPos = (double)((*xPos * 2) - resX) / resY;
    *yPos = 1 - (double)(*yPos * 2) / resY;
}




//the following is for rendering text
void Graphics2D::renderChar(float xPos, float yPos, int index) {
    if (index >= 0 && index < 36) { //if valid
        float line[4];
        for (int i = 0; i < 14; i++) { //iterates through all segments
            if (characterSet[index][i] == true) {
                line[0] = (cellVertices[i][0] + xPos) * invAspectRatio;
                line[1] = cellVertices[i][1] + yPos;
                line[2] = (cellVertices[i][2] + xPos) * invAspectRatio;
                line[3] = cellVertices[i][3] + yPos;
                glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float), line, GL_STATIC_DRAW);
                glDrawArrays(GL_LINES, 0, 2);
            }
        }
    }
}

void Graphics2D::renderString(float xPos, float yPos, char* contents, int length) {
    setColour(textColour[0], textColour[1], textColour[2], textColour[3]);

    float xOffset = 0; //allows multiple characters to be placed next to one another
    float yOffset = 0; //for new lines
    for (int i = 0; i < length; i++) {
        if (isdigit(contents[i])) {
            renderChar(xPos + xOffset, yPos + yOffset, (int)contents[i] - '0');
        }
        else if (contents[i] == '\n') {
            yOffset -= cellVertices[0][3] * 1.25;
            xOffset = -cellVertices[0][2] * 1.25;
        }
        else {
            renderChar(xPos + xOffset, yPos + yOffset, (int)contents[i] - 55);
        }
        xOffset += cellVertices[0][2] * 1.25; //width of cell (plus a small space between letters)
    }

    if (renderType == 0 || renderType == 2) {
        setColour(fillColour[0], fillColour[1], fillColour[2], fillColour[3]);
    }
    else {
        setColour(lineColour[0], lineColour[1], lineColour[2], lineColour[3]);
    }
}








Graphics2D::~Graphics2D() { cout << "Engine destroyed!" << endl; };