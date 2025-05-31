#include <GL/gl.h>
#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>

bool menuActive = true;

float boatX = 0.0f, boatY = -0.8f;

const int numObstacles = 3;
float obstacleX[numObstacles], obstacleY[numObstacles];
float obstacleXSpeed[numObstacles];
float speed = 0.02f;
bool gameOver = false;

int score = 0;
void initObstacles() {
    for (int i = 0; i < numObstacles; i++) {
        obstacleX[i] = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
        obstacleY[i] = ((float)rand() / RAND_MAX) * 2.0f;
        obstacleXSpeed[i] = (((float)rand() / RAND_MAX) * 0.04f) - 0.02f;
}
}

void resetGame() {
    boatX = 0.0f;
    boatY = -0.8f;
    gameOver = false;
    score = 0;
    speed = 0.02f;
    initObstacles();
}

void drawBoat() {
    glColor3f(0.8f, 0.2f, 0.2f);

    glBegin(GL_POLYGON);
    glVertex2f(boatX, boatY + 0.15f);
    glVertex2f(boatX - 0.07f, boatY + 0.05f);
    glVertex2f(boatX - 0.07f, boatY - 0.05f);
    glVertex2f(boatX, boatY - 0.15f);
    glVertex2f(boatX + 0.07f, boatY - 0.05f);
    glVertex2f(boatX + 0.07f, boatY + 0.05f);
    glEnd();


    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_LINES);
    glVertex2f(boatX - 0.07f, boatY + 0.05f);
    glVertex2f(boatX + 0.07f, boatY + 0.05f);

    glVertex2f(boatX - 0.07f, boatY - 0.05f);
    glVertex2f(boatX + 0.07f, boatY - 0.05f);
    glEnd();
}


void drawObstacles() {

    for (int i = 0; i < numObstacles; i++) {

        glColor3f(0.5f, 0.0f, 0.5f);
        glBegin(GL_POLYGON);
        glVertex2f(obstacleX[i], obstacleY[i] + 0.07f);
        glVertex2f(obstacleX[i] - 0.03f, obstacleY[i] + 0.02f);
        glVertex2f(obstacleX[i] - 0.03f, obstacleY[i] - 0.02f);
        glVertex2f(obstacleX[i], obstacleY[i] - 0.07f);
        glVertex2f(obstacleX[i] + 0.03f, obstacleY[i] - 0.02f);
        glVertex2f(obstacleX[i] + 0.03f, obstacleY[i] + 0.02f);
        glEnd();


        glColor3f(0.8f, 0.8f, 0.8f);
        glBegin(GL_LINES);

        glVertex2f(obstacleX[i] - 0.03f, obstacleY[i] + 0.02f);
        glVertex2f(obstacleX[i] + 0.03f, obstacleY[i] + 0.02f);


        glVertex2f(obstacleX[i] - 0.03f, obstacleY[i] - 0.02f);
        glVertex2f(obstacleX[i] + 0.03f, obstacleY[i] - 0.02f);
        glEnd();
    }
}


void drawScore() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-0.9f, 0.9f);
    std::string scoreText = "Score: " + std::to_string(score);
    for (char c : scoreText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}


void handleKeypress(unsigned char key, int x, int y) {
    if (menuActive && key == 13) {

        menuActive = false;
    } else if (gameOver && key == 'r') {

        resetGame();
    }
}

void handleSpecialKeypress(int key, int x, int y) {
    if (!menuActive && !gameOver) {
        if (key == GLUT_KEY_LEFT && boatX > -0.9f) boatX -= 0.1f;
        if (key == GLUT_KEY_RIGHT && boatX < 0.9f) boatX += 0.1f;
        if (key == GLUT_KEY_UP && boatY < 0.9f) boatY += 0.1f;
        if (key == GLUT_KEY_DOWN && boatY > -0.9f) boatY -= 0.1f;
    }
}


void update(int value) {
    if (!menuActive && !gameOver) {
        for (int i = 0; i < numObstacles; i++) {

            obstacleY[i] -= speed;
            obstacleX[i] += obstacleXSpeed[i];

            if (obstacleX[i] < -1.0f || obstacleX[i] > 1.0f) {
                obstacleXSpeed[i] = -obstacleXSpeed[i];
            }


            if (obstacleY[i] < -1.0f) {
                obstacleY[i] = 1.0f;
                obstacleX[i] = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
                obstacleXSpeed[i] = (((float)rand() / RAND_MAX) * 0.04f) - 0.02f;
                score++;


                if (score % 5 == 0 && speed < 0.1f) speed += 0.005f;
            }


            if (fabs(boatX - obstacleX[i]) < 0.1 && fabs(boatY - obstacleY[i]) < 0.1) {
                gameOver = true;
            }
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (menuActive) {

        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(-0.45f, 0.0f);
        const char* message = "Press Enter key to Start the Game";
        for (int i = 0; message[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[i]);
        }
    } else if (gameOver) {

        glColor3f(1.0f, 0.0f, 0.0f);
        glRasterPos2f(-0.2f, 0.0f);
        const char* message = "Game Over!";
        for (int i = 0; message[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[i]);
        }

                glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(-0.25f, -0.1f);
        std::string finalScore = "Final Score: " + std::to_string(score);
        for (char c : finalScore) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(-0.25f, -0.2f);
        const char* restartMessage = "Press 'R' to Restart";
        for (int i = 0; restartMessage[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, restartMessage[i]);
        }
    } else {

        drawBoat();
        drawObstacles();
        drawScore();
    }

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("2D Boat Game");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

    initObstacles();

    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeypress);
    glutSpecialFunc(handleSpecialKeypress);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}
