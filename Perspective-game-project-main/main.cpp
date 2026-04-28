#include <GL/glut.h>
#include <cmath>
#include <GL/freeglut.h>
#include <iostream>

// Function declaration
void setPerspectiveProjection();

// Global variables for square position
float squarePosX = -4.0f;
float squarePosY = 0.5f;
float fallSpeed = 0.0f;

// Camera control variables
float cameraAngleY = 0.0f;  // Rotation around Y-axis
float cameraPosZ = 10.0f;    // Distance from scene

// Middle platform position
float middlePlatformX = 0.0f;
bool platformLatched = false;

// Game state
bool gameStarted = false;

// Function to set perspective projection
void setPerspectiveProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// Function to draw a cube
void drawCube() {
    glBegin(GL_QUADS);

   // Front Face
    glColor3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.3f, -0.3f, 0.3f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.3f, -0.3f, 0.3f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.3f, 0.3f, 0.3f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.3f, 0.3f, 0.3f);

    // Back Face
    glColor3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.3f, -0.3f, -0.3f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.3f, 0.3f, -0.3f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.3f, 0.3f, -0.3f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.3f, -0.3f, -0.3f);

    // Left Face
    glColor3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.3f, -0.3f, -0.3f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.3f, -0.3f, 0.3f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.3f, 0.3f, 0.3f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.3f, 0.3f, -0.3f);

    // Right Face
    glColor3f(1.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.3f, -0.3f, -0.3f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.3f, 0.3f, -0.3f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.3f, 0.3f, 0.3f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.3f, -0.3f, 0.3f);

    glEnd();
}



void drawCube1() {
    glBegin(GL_QUADS);

   // Front Face
     glColor4f(0.8f, 0.8f, 0.8f, 0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.8f, -0.8f, 0.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.3f, 0.5f, -0.10f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.3f, 0.5f, 0.10f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.8f, -0.8f, 0.8f);



    glEnd();
}



// Function to draw a platform
void drawPlatform(float xOffset, float width) {
     glColor3f(0.6f, 0.3f, 0.1f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-width / 1.5 + xOffset, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(width / 1.5 + xOffset, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(width / 1.5 + xOffset, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-width / 1.5 + xOffset, 0.0f, 1.0f);
    glEnd();
}

// Function to apply gravity
void applyGravity() {
    fallSpeed -= 0.001f;
    squarePosY += fallSpeed;

    // Collision with left and right platforms
    if ((squarePosX >= -6.0f && squarePosX <= -1.0f && squarePosY <= 0.25f) ||
        (squarePosX >= 1.0f && squarePosX <= 6.0f && squarePosY <= 0.25f) ||
        (squarePosX >= middlePlatformX - 1.0f && squarePosX <= middlePlatformX + 1.0f && squarePosY <= 0.25f)) {
        fallSpeed = 0.0f;
        squarePosY = 0.25f;
    }

    // Reset if falls off the screen
    if (squarePosY < -5.0f) {
        squarePosX = -4.0f;
        squarePosY = 0.5f;
        fallSpeed = 0.0f;
    }
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!gameStarted) {
        // Display menu
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, 800, 0, 600);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(350, 400);
        std::string menuTitle = "Main Menu";
        for (char c : menuTitle) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }

        glRasterPos2f(300, 300);
        std::string startOption = "Press ' 1 ' to start playing";

        for (char c : startOption) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }

        glRasterPos2f(300, 250);
        std::string exitOption = "Press ' 2 ' to Exit the game";
        for (char c : exitOption) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    } else {
        // Set the perspective projection for the game
        setPerspectiveProjection();

        // Save the current camera view
        glPushMatrix();

        // Set the camera view
        float cameraX = sin(cameraAngleY) * cameraPosZ;
        float cameraZ = cos(cameraAngleY) * cameraPosZ;
        gluLookAt(cameraX, 2.0, cameraZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

        // Update middle platform position based on zoom
        if (cameraPosZ <= 6.0f) {
            middlePlatformX = 0.0f;
            platformLatched = true;
        } else {
            middlePlatformX = 0.5f; // Initial gap
            platformLatched = false;
        }

        // Draw the moving platforms
        drawPlatform(-3.5f, 3.5f);  // Left platform (shorter)
        drawPlatform(3.5f, 3.5f);   // Right platform (shorter)

        // Apply gravity and update player's cube
        applyGravity();

        // Draw the moving cube (player)
        glPushMatrix();
        glColor3f(0.0f, 0.0f, 1.0f);
        glTranslatef(squarePosX, squarePosY, 0.0f);
        drawCube();
        glPopMatrix();

        // Restore camera transformation
        glPopMatrix();

        // Draw the middle platform (not affected by camera)
        glPushMatrix();
        glLoadIdentity();  // Reset to world space (not camera space)
        drawPlatform(middlePlatformX, 2.0f);  // Middle platform remains 2.0f wide
        glPopMatrix();

        // Draw the fixed stationary square that will not move with the camera
        glPushMatrix();
        glLoadIdentity();  // Reset to world coordinates, unaffected by camera
        glTranslatef(0.0f, 0.0f, -5.0f);  // This places the square 5 units in front of the camera
        glColor3f(1.0f, 1.0f, 0.0f);  // Set color to yellow
        drawCube1();  // A square with size 0.5 units
        glPopMatrix();
    }

    glutSwapBuffers();
}

// Update function
void update(int value) {
    if (gameStarted) {
        glutPostRedisplay();
    }
    glutTimerFunc(16, update, 0);
}

// Key press handling
void specialKeyPressed(int key, int x, int y) {
    if (gameStarted) {
        if (key == GLUT_KEY_LEFT) squarePosX -= 0.1f;
        if (key == GLUT_KEY_RIGHT) squarePosX += 0.1f;
        if (key == GLUT_KEY_UP) cameraPosZ -= 0.5f;
        if (key == GLUT_KEY_DOWN) cameraPosZ += 0.5f;
    }
}

// Camera rotation with keys A and D
void keyPressed(unsigned char key, int x, int y) {
    if (gameStarted) {
        if (key == 'a') cameraAngleY -= 0.05f;
        if (key == 'd') cameraAngleY += 0.05f;
    }
    if (key == '1') {
        gameStarted = true;
        setPerspectiveProjection();  // Reset to perspective projection
    }
    if (key == '2') {
        glutLeaveMainLoop();
    }
    if (key == 27) glutLeaveMainLoop();
}

// Initialize OpenGL
void initialize() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND); // Enable blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set blending function
    glClearColor(0.2f, 0.5f, 0.7f, 1.0f);
    setPerspectiveProjection();
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Camera Perspective Control");

    initialize();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyPressed);
    glutSpecialFunc(specialKeyPressed);
    glutTimerFunc(16, update, 0);
    glutMainLoop();

    return 0;
}
