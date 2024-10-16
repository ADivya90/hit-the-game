#include <GL/glut.h>
#include <cmath>
#include <ctime>
#include <string>
#include <sstream>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


// Window size
const int windowWidth = 800;
const int windowHeight = 600;

// Arrow position and speed
float arrowX = -350.0f;
float arrowY = 0.0f;
float arrowSpeed = 5.0f;
bool arrowShot = false;

// Target position and radius
float targetX = 350.0f;
float targetY = 0.0f;
const float targetRadius = 50.0f;

// Score
int score = 0;

// Function to draw a circle
void drawCircle(float x, float y, float radius) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 360; i++) {
        float angle = i * M_PI / 180.0f;
        glVertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
    }
    glEnd();
}

// Function to display text
void drawText(float x, float y, const std::string& text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void bitmap_output(int x, int y, const char* string, void* font) {
    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++) {
        glutBitmapCharacter(font, string[i]);
    }
}

// Function to initialize OpenGL settings
void initialize() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-windowWidth / 2, windowWidth / 2, -windowHeight / 2, windowHeight / 2);
}

// Function to handle key presses
void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
    case ' ':
        arrowShot = true;
        break;
    case 'r':
        arrowShot = false;
        arrowX = -350.0f;
        arrowY = 0.0f;
        score = 0;
        break;
    }
}




// Function to handle special key presses
void handleSpecialKeypress(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        arrowY += 10.0f;
        break;
    case GLUT_KEY_DOWN:
        arrowY -= 10.0f;
        break;
    }
}

// Function to update the game state
void update(int value) {
    if (arrowShot) {
        arrowX += arrowSpeed;
        if (arrowX > windowWidth / 2) {
            arrowShot = false;
            arrowX = -350.0f;
        }

        // Check for collision with the target
        float dx = arrowX - targetX;
        float dy = arrowY - targetY;
        if (sqrt(dx * dx + dy * dy) < targetRadius) {
            score++;
            arrowShot = false;
            arrowX = -350.0f;

            // Randomly reposition the target
            targetX = rand() % (windowWidth - 2 * (int)targetRadius) - (windowWidth / 2 - targetRadius);
            targetY = rand() % (windowHeight - 2 * (int)targetRadius) - (windowHeight / 2 - targetRadius);
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// Function to display the game
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the target
    glColor3f(1.0f, 0.0f, 0.0f);
    drawCircle(targetX, targetY, targetRadius);

    // Draw the arrow
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(arrowX - 10.0f, arrowY - 2.0f);
    glVertex2f(arrowX + 10.0f, arrowY - 2.0f);
    glVertex2f(arrowX + 10.0f, arrowY + 2.0f);
    glVertex2f(arrowX - 10.0f, arrowY + 2.0f);
    glEnd();

    // Display the score
    std::stringstream ss;
    ss << "Score: " << score;
    glColor3f(0.0f, 0.0f, 0.0f);
    drawText(-windowWidth / 2 + 10, windowHeight / 2 - 30, ss.str());
    std::string instructions = "press 'up arrow key' to move upwards,press 'down arrow key' to move down, press ' ' to hitt!";
    drawText(-windowWidth / 2 + 10, windowHeight / 22 - 60, instructions);
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Archery Game");

    initialize();
    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeypress);
    glutSpecialFunc(handleSpecialKeypress);
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}