#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <cstdlib> 
#include <ctime>  
#include <cmath> 
#include <glut.h>


// topbar variables 
float health = 5;
int score = 0;
int gameTime = 120;
int lastTime = 0;
int factor = 100;
int lastingMoon = 0;
int step = 10;
int lastingRocket = 0;
bool end = false;
float playerAngle = 0;
float rotationAngle = 0.0;
float scaleFactor = 1.0;
float scaleIncrement = 0.01;
bool scalingUp = true;

// player position 
float playerX = 350;
float playerY = 300;

// Obstacles Positions
float obstacleX[5];
float obstacleY[5];

// Collectables Positions 
float collectableX[5];
float collectableY[5];


// Moon powerUp Positions
float moonX[2];
float moonY[2];


// Rocket powerup positions
float rocketX[2];
float rocketY[2];


// Random points in background positions 
float backX[15];
float backY[15];

void Display();





void print(int x, int y, char* string)
{
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}

void timer(int value) {
	// Calculate the elapsed time
	int currentTime = time(NULL);
	int elapsedTime = currentTime - lastTime;

	// Update the gameTime
	if (elapsedTime >= 1 && !end) {
		gameTime -= elapsedTime;
		lastTime = currentTime;
		lastingMoon -= 1;
		lastingRocket -= 1;
		if (lastingMoon == 0) {
			factor = 100;
		}
		if (lastingRocket == 0) {
			step = 10;
		}
		if (gameTime <= 0) {
			end = true;
		}
	}

	glutTimerFunc(1000, timer, 0);
	glutPostRedisplay();
}
void updateRotation(int value) {
	// Increase the rotation angle 
	rotationAngle += 1.0;

	// Ensure the rotation angle stays within a full circle
	if (rotationAngle >= 360.0) {
		rotationAngle -= 360.0;
	}

	glutPostRedisplay();
	glutTimerFunc(60, updateRotation, 0);
}

void updateScale(int x) {
	if (scalingUp) {
		scaleFactor += scaleIncrement;
		if (scaleFactor >= 1.2) { // Adjust the desired scale factor as needed
			scalingUp = false;
		}
	}
	else {
		scaleFactor -= scaleIncrement;
		if (scaleFactor <= 1.0) {
			scalingUp = true;
		}
	}

	glutPostRedisplay();
	glutTimerFunc(40, updateScale, 0); // Update the scale every 100 milliseconds
}



int checkRCollisions() {
	for (int i = 0;i < 5;i++) {
		std::cout << obstacleX[i] << std::endl;
		std::cout << playerX << std::endl;
		if (playerX <= obstacleX[i] && abs(playerX - obstacleX[i]) < 45 && abs(playerY - obstacleY[i]) < 45) {
			return 1;
		}
	}
	return 0;

}

int checkLCollisions() {
	for (int i = 0;i < 5;i++) {
		std::cout << obstacleX[i] << std::endl;
		std::cout << playerX << std::endl;
		if (playerX >= obstacleX[i] && abs(playerX - obstacleX[i]) < 45 && abs(playerY - obstacleY[i]) < 45) {
			return 1;
		}
	}
	return 0;

}

int checkUCollisions() {
	for (int i = 0;i < 5;i++) {
		if (playerY <= obstacleY[i] && abs(playerX - obstacleX[i]) < 45 && abs(playerY - obstacleY[i]) < 45) {
			return 1;
		}
	}
	return 0;

}


int checkBCollisions() {
	for (int i = 0;i < 5;i++) {
		if (playerY >= obstacleY[i] && abs(playerX - obstacleX[i]) < 45 && abs(playerY - obstacleY[i]) < 45) {
			return 1;
		}
	}
	return 0;

}

int checkCollection() {
	for (int i = 0;i < 5;i++) {
		if (abs(playerX - collectableX[i]) < 30 && abs(playerY - collectableY[i]) < 30) {
			return i;
		}
	}
	return -1;
}


int checkMoon() {
	for (int i = 0;i < 2;i++) {
		if (abs(playerX - moonX[i]) < 30 && abs(playerY - moonY[i]) < 30) {
			return i;
		}
	}
	return -1;
}

int checkRocket() {
	for (int i = 0;i < 2;i++) {
		if (abs(playerX - rocketX[i]) < 30 && abs(playerY - rocketY[i]) < 30) {
			return i;
		}
	}
	return -1;
}

bool checkGoal() {
	for (int i = 0;i < 2;i++) {
		if (abs(playerX - 90) < 30 && abs(playerY - 90) < 30) {
			return true;
		}
	}
	return false;
}





void spe(int k, int x, int y)
{
	if (k == GLUT_KEY_RIGHT) {
		if (playerX >= 630 || checkRCollisions() == 1) {
			health--;
		}
		else {
			playerAngle = 180.0;
			playerX += step;
			int index = checkCollection();
			if (index != -1) {
				collectableX[index] = 10000;
				collectableY[index] = 10000;
				score += factor;
			}
			int moonIndex = checkMoon();
			if (moonIndex != -1) {
				moonX[moonIndex] = 10000;
				moonY[moonIndex] = 10000;
				factor = 250;
				lastingMoon = 5;
			}
			int rocketIndex = checkRocket();
			if (rocketIndex != -1) {
				rocketX[rocketIndex] = 10000;
				rocketY[rocketIndex] = 10000;
				step = 25;
				lastingRocket = 5;
			}
			end = checkGoal();

		}
	}
	if (k == GLUT_KEY_LEFT) {
		if (playerX <= 70 || checkLCollisions() == 1) {
			health--;
		}
		else {
			playerAngle = 0.0;
			playerX -= step;
			int index = checkCollection();
			if (index != -1) {
				collectableX[index] = 10000;
				collectableY[index] = 10000;
				score += factor;
			}
			int moonIndex = checkMoon();
			if (moonIndex != -1) {
				moonX[moonIndex] = 10000;
				moonY[moonIndex] = 10000;
				factor = 250;
				lastingMoon = 5;
			}
			int rocketIndex = checkRocket();
			if (rocketIndex != -1) {
				rocketX[rocketIndex] = 10000;
				rocketY[rocketIndex] = 10000;
				step = 25;
				lastingRocket = 5;
			}
			end = checkGoal();
		}
	}
	if (k == GLUT_KEY_UP) {
		if (playerY >= 530 || checkUCollisions() == 1) {
			health--;
		}
		else {
			playerAngle = -90.0;
			playerY += step;
			int index = checkCollection();
			if (index != -1) {
				collectableX[index] = 10000;
				collectableY[index] = 10000;
				score += factor;
			}
			int moonIndex = checkMoon();
			if (moonIndex != -1) {
				moonX[moonIndex] = 10000;
				moonY[moonIndex] = 10000;
				factor = 250;
				lastingMoon = 5;
			}
			int rocketIndex = checkRocket();
			if (rocketIndex != -1) {
				rocketX[rocketIndex] = 10000;
				rocketY[rocketIndex] = 10000;
				step = 25;
				lastingRocket = 5;
			}
			end = checkGoal();
		}
	}
	if (k == GLUT_KEY_DOWN) {
		if (playerY <= 70 || checkBCollisions() == 1) {
			health--;
		}
		else {
			playerAngle = 90.0;
			playerY -= step;
			int index = checkCollection();
			if (index != -1) {
				collectableX[index] = 10000;
				collectableY[index] = 10000;
				score += factor;
			}
			int moonIndex = checkMoon();
			if (moonIndex != -1) {
				moonX[moonIndex] = 10000;
				moonY[moonIndex] = 10000;
				factor = 250;
				lastingMoon = 5;
			}
			int rocketIndex = checkRocket();
			if (rocketIndex != -1) {
				rocketX[rocketIndex] = 10000;
				rocketY[rocketIndex] = 10000;
				step = 25;
				lastingRocket = 5;
			}
			end = checkGoal();
		}
	}
	glutPostRedisplay();
}

void generateObstaclePositions() {
	float minX = 80;  // Minimum X coordinate
	float maxX = 620;   // Maximum X coordinate
	float minY = 80;  // Minimum Y coordinate
	float maxY = 520;   // Maximum Y coordinate

	for (int i = 0; i < 5; i++) {
		int randomX = (float)rand() / RAND_MAX * (maxX - minX) + minX;
		int randomY = (float)rand() / RAND_MAX * (maxY - minY) + minY;
		while ((abs(randomX - playerX) <= 45 && abs(randomY - playerY) <= 45) || (abs(randomX - 75) <= 45 && abs(randomY - 75) <= 45)) {
			randomX = (float)rand() / RAND_MAX * (maxX - minX) + minX;
			randomY = (float)rand() / RAND_MAX * (maxY - minY) + minY;
		}
		// check that i am not on an obstacle 
		for (int i = 0;i < 5;i++) {
			while (abs(randomX - obstacleX[i]) <= 45 && abs(randomY - obstacleY[i]) <= 45) {
				randomX = (float)rand() / RAND_MAX * (maxX - minX) + minX;
				randomY = (float)rand() / RAND_MAX * (maxY - minY) + minY;
			}
		}
		obstacleX[i] = randomX;
		obstacleY[i] = randomY;
	}
	std::cout << "obstacleX" << std::endl;
	for (int i = 0; i < 5; i++) {
		std::cout << obstacleX[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "obstacleY" << std::endl;
	for (int i = 0; i < 5; i++) {
		std::cout << obstacleY[i] << " ";
	}


}


void generateCollectablePositions() {
	// Define the ranges for X and Y coordinates
	float minX = 80;  // Minimum X coordinate
	float maxX = 620;   // Maximum X coordinate
	float minY = 80;  // Minimum Y coordinate
	float maxY = 520;   // Maximum Y coordinate

	for (int i = 0; i < 5; i++) {
		int randomX = (float)rand() / RAND_MAX * (maxX - minX) + minX;
		int randomY = (float)rand() / RAND_MAX * (maxY - minY) + minY;
		while ((abs(randomX - playerX) <= 45 && abs(randomY - playerY) <= 45) || (abs(randomX - 75) <= 45 && abs(randomY - 75) <= 45)) {
			randomX = (float)rand() / RAND_MAX * (maxX - minX) + minX;
			randomY = (float)rand() / RAND_MAX * (maxY - minY) + minY;
		}
		// check that i am not on an obstacle 
		for (int i = 0;i < 5;i++) {
			while (abs(randomX - obstacleX[i]) <= 45 && abs(randomY - obstacleY[i]) <= 45) {
				randomX = (float)rand() / RAND_MAX * (maxX - minX) + minX;
				randomY = (float)rand() / RAND_MAX * (maxY - minY) + minY;
			}
		}
		// check that i am not on a collectable
		for (int i = 0;i < 5;i++) {
			while (abs(randomX - collectableX[i]) <= 45 && abs(randomY - collectableY[i]) <= 45) {
				randomX = (float)rand() / RAND_MAX * (maxX - minX) + minX;
				randomY = (float)rand() / RAND_MAX * (maxY - minY) + minY;
			}
		}
		collectableX[i] = randomX;
		collectableY[i] = randomY;
	}
	std::cout << "collectableX" << std::endl;
	for (int i = 0; i < 5; i++) {
		std::cout << collectableX[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "collectableY" << std::endl;
	for (int i = 0; i < 5; i++) {
		std::cout << collectableY[i] << " ";
	}
}

void generateBackPositions() {
	// Define the ranges for X and Y coordinates
	float minX = 80;  // Minimum X coordinate
	float maxX = 620;   // Maximum X coordinate
	float minY = 80;  // Minimum Y coordinate
	float maxY = 520;   // Maximum Y coordinate

	for (int i = 0; i < 15; i++) {
		int randomX = (float)rand() / RAND_MAX * (maxX - minX) + minX;
		int randomY = (float)rand() / RAND_MAX * (maxY - minY) + minY;
		backX[i] = randomX;
		backY[i] = randomY;
	}
	std::cout << "backX" << std::endl;
	for (int i = 0; i < 5; i++) {
		std::cout << backX[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "backY" << std::endl;
	for (int i = 0; i < 5; i++) {
		std::cout << backY[i] << " ";
	}
}


void generateMoonPositions() {
	// Define the ranges for X and Y coordinates
	float minX = 80;  // Minimum X coordinate
	float maxX = 620;   // Maximum X coordinate
	float minY = 80;  // Minimum Y coordinate
	float maxY = 520;   // Maximum Y coordinate

	for (int i = 0; i < 2; i++) {
		int randomX = (float)rand() / RAND_MAX * (maxX - minX) + minX;
		int randomY = (float)rand() / RAND_MAX * (maxY - minY) + minY;
		while ((abs(randomX - playerX) <= 45 && abs(randomY - playerY) <= 45) || (abs(randomX - 75) <= 45 && abs(randomY - 75) <= 45)) {
			randomX = (float)rand() / RAND_MAX * (maxX - minX) + minX;
			randomY = (float)rand() / RAND_MAX * (maxY - minY) + minY;
		}
		// check that i am not on an obstacle 
		for (int i = 0;i < 5;i++) {
			while (abs(randomX - obstacleX[i]) <= 45 && abs(randomY - obstacleY[i]) <= 45) {
				randomX = (float)rand() / RAND_MAX * (maxX - minX) + minX;
				randomY = (float)rand() / RAND_MAX * (maxY - minY) + minY;
			}
		}
		// check that i am not on a collectable
		for (int i = 0;i < 5;i++) {
			while (abs(randomX - collectableX[i]) <= 45 && abs(randomY - collectableY[i]) <= 45) {
				randomX = (float)rand() / RAND_MAX * (maxX - minX) + minX;
				randomY = (float)rand() / RAND_MAX * (maxY - minY) + minY;
			}
		}
		// check that i am not on a moon powerUp
		for (int i = 0;i < 2;i++) {
			while (abs(randomX - moonX[i]) <= 45 && abs(randomY - moonY[i]) <= 45) {
				randomX = (float)rand() / RAND_MAX * (maxX - minX) + minX;
				randomY = (float)rand() / RAND_MAX * (maxY - minY) + minY;
			}
		}
		moonX[i] = randomX;
		moonY[i] = randomY;
	}
	std::cout << "collectableX" << std::endl;
	for (int i = 0; i < 2; i++) {
		std::cout << moonX[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "collectableY" << std::endl;
	for (int i = 0; i < 2; i++) {
		std::cout << moonY[i] << " ";
	}
}


void generateRocketPositions() {
	// Define the ranges for X and Y coordinates
	float minX = 80;  // Minimum X coordinate
	float maxX = 620;   // Maximum X coordinate
	float minY = 80;  // Minimum Y coordinate
	float maxY = 520;   // Maximum Y coordinate

	for (int i = 0; i < 2; i++) {
		int randomX = (float)rand() / RAND_MAX * (maxX - minX) + minX;
		int randomY = (float)rand() / RAND_MAX * (maxY - minY) + minY;
		while ((abs(randomX - playerX) <= 45 && abs(randomY - playerY) <= 45) || (abs(randomX - 75) <= 45 && abs(randomY - 75) <= 45)) {
			randomX = (float)rand() / RAND_MAX * (maxX - minX) + minX;
			randomY = (float)rand() / RAND_MAX * (maxY - minY) + minY;
		}
		// check that i am not on an obstacle 
		for (int i = 0;i < 5;i++) {
			while (abs(randomX - obstacleX[i]) <= 45 && abs(randomY - obstacleY[i]) <= 45) {
				randomX = (float)rand() / RAND_MAX * (maxX - minX) + minX;
				randomY = (float)rand() / RAND_MAX * (maxY - minY) + minY;
			}
		}
		// check that i am not on a collectable
		for (int i = 0;i < 5;i++) {
			while (abs(randomX - collectableX[i]) <= 45 && abs(randomY - collectableY[i]) <= 45) {
				randomX = (float)rand() / RAND_MAX * (maxX - minX) + minX;
				randomY = (float)rand() / RAND_MAX * (maxY - minY) + minY;
			}
		}
		// check that i am not on a moon powerUp
		for (int i = 0;i < 2;i++) {
			while (abs(randomX - moonX[i]) <= 45 && abs(randomY - moonY[i]) <= 45) {
				randomX = (float)rand() / RAND_MAX * (maxX - minX) + minX;
				randomY = (float)rand() / RAND_MAX * (maxY - minY) + minY;
			}
		}
		rocketX[i] = randomX;
		rocketY[i] = randomY;
	}
	std::cout << "collectableX" << std::endl;
	for (int i = 0; i < 2; i++) {
		std::cout << moonX[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "collectableY" << std::endl;
	for (int i = 0; i < 2; i++) {
		std::cout << moonY[i] << " ";
	}
}

void Display() {


	if (!end && health > 0) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// This is the word "Health Bar"
		glColor3f(0, 0, 0);
		char* p0s[20];
		sprintf((char*)p0s, "Health Bar: ");
		print(20, 650, (char*)p0s);


		// This is the health bar and we can modify it using the float "health"
		const float sep = 3;
		const float barHeight = 10;
		float bar = 625;
		glBegin(GL_QUADS);
		glColor3f(0.0f, 0.0f, 0.0f);
		for (float i = 0; i < health; i += 1) {
			glVertex2f(150, bar);
			glVertex2f(200, bar);
			glVertex2f(200, bar + barHeight);
			glVertex2f(150, bar + barHeight);
			bar += barHeight + 3;
		}
		glEnd();

		// This is the word "Score" with the actual score displayed beside it 
		glColor3f(0, 0, 0);
		sprintf((char*)p0s, "Score: %d", score);
		print(300, 650, (char*)p0s);







		// This is the word "Time" with the actual time displayed beside it 
		glColor3f(0, 0, 0);
		sprintf((char*)p0s, "Time: %d", gameTime);
		print(500, 650, (char*)p0s);


		// This is a separator between the game board and time and score 
		glBegin(GL_LINES);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 600.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(700.0f, 600.0f, 0.0f);
		glEnd();


		// Board background 
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_QUADS);
		glVertex2f(55, 55);
		glVertex2f(645, 55);
		glVertex2f(645, 545);
		glVertex2f(55, 545);
		glEnd();


		// This is the player model 
		glutSpecialFunc(spe);

		glPushMatrix();
		glTranslated(playerX, playerY, 0.0);
		glRotated(playerAngle, 0.0, 0.0, 1.0);
		glTranslated(-playerX, -playerY, 0.0);
		const float astronautSize = 20.0;
		// Player 
		glColor3f(1.0, 1.0, 1.0); // White color
		glBegin(GL_POLYGON);
		glVertex2f(playerX - 10, playerY - 15);
		glVertex2f(playerX + 10, playerY - 15);
		glVertex2f(playerX + 10, playerY + 15);
		glVertex2f(playerX - 10, playerY + 15);
		glEnd();

		glColor3f(1.0, 1.0, 1.0); // Grey color
		int headSegments = 100;
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(playerX, playerY + 21); // Center point
		for (int j = 0; j <= headSegments; j++) {
			float angle = 2.0 * 3.1415926f * j / headSegments;
			float dx = 7 * cos(angle);
			float dy = 7 * sin(angle);
			glVertex2f(playerX + dx, playerY + 21 + dy);
		}
		glEnd();

		glColor3f(0.0, 0.0, 0.0); // Black color
		int innerSegments = 100;
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(playerX, playerY + 21); // Center point
		for (int j = 0; j <= innerSegments; j++) {
			float angle = 2.0 * 3.1415926f * j / innerSegments;
			float dx = 5 * cos(angle);
			float dy = 5 * sin(angle);
			glVertex2f(playerX + dx, playerY + 21 + dy);
		}
		glEnd();


		glColor3f(0.5, 0.5, 0.5); // Grey color
		glPointSize(3.0);
		glBegin(GL_POINTS);
		glVertex2f(playerX + 2, playerY + 21);
		glEnd();

		glColor3f(1.0, 1.0, 1.0); // White color
		glLineWidth(5.0);
		glBegin(GL_LINES);
		glVertex2f(playerX + 5, playerY - 15);
		glVertex2f(playerX + 5, playerY - 20);
		glVertex2f(playerX - 5, playerY - 15);
		glVertex2f(playerX - 5, playerY - 20);
		glVertex2f(playerX - 5, playerY - 20);
		glVertex2f(playerX, playerY - 25);
		glVertex2f(playerX + 5, playerY - 20);
		glVertex2f(playerX + 10, playerY - 25);
		glVertex2f(playerX - 9, playerY + 12);
		glVertex2f(playerX - 18, playerY + 20);
		glVertex2f(playerX + 9, playerY + 12);
		glVertex2f(playerX + 18, playerY + 5);


		glEnd();

		glColor3f(0.0, 0.0, 0.0); // Black color
		glLineWidth(2.0);
		glBegin(GL_LINES);
		glVertex2f(playerX + 5, playerY + 15);
		glVertex2f(playerX + 10, playerY + 1);
		glVertex2f(playerX - 5, playerY + 15);
		glVertex2f(playerX - 10, playerY + 1);
		glEnd();

		glPopMatrix();

		// Background points 
		for (int i = 0;i < 15;i++) {
			glPushMatrix();
			glTranslated(350, 300, 0);
			//glScaled(scaleFactor, scaleFactor, 1.0);
			glRotated(rotationAngle, 0, 0, 1);
			glTranslated(-350, -300, 0);
			glBegin(GL_POINTS);
			glPointSize(0.3);
			glColor3f(1.0, 1.0, 1.0);
			glVertex2f(backX[i], backY[i]);
			glEnd();
			glPopMatrix();

		}




		// Four scene boundaries 
		// Left scene boundary
		glBegin(GL_LINES);
		glLineWidth(6.0);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(45.0f, 45.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(45.0f, 555.0f, 0.0f);
		glEnd();
		glBegin(GL_LINES);
		glLineWidth(6.0);
		glColor3f(.0f, 0.0f, 0.0f);
		glVertex3f(50.0f, 50.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(50.0f, 550.0f, 0.0f);
		glEnd();
		// Bottom scene boundary
		glBegin(GL_LINES);
		glLineWidth(6.0);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(45.0f, 45.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(655.0f, 45.0f, 0.0f);
		glEnd();
		glBegin(GL_LINES);
		glLineWidth(6.0);
		glColor3f(.0f, 0.0f, 0.0f);
		glVertex3f(50.0f, 50.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(650.0f, 50.0f, 0.0f);
		glEnd();
		// Right scene boundary
		glBegin(GL_LINES);
		glLineWidth(6.0);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(655.0f, 45.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(655.0f, 555.0f, 0.0f);
		glEnd();
		glBegin(GL_LINES);
		glLineWidth(6.0);
		glColor3f(.0f, 0.0f, 0.0f);
		glVertex3f(650.0f, 50.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(650.0f, 550.0f, 0.0f);
		glEnd();
		// Upper scene boundary
		glBegin(GL_LINES);
		glLineWidth(6.0);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(45.0f, 555.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(655.0f, 555.0f, 0.0f);
		glEnd();
		glBegin(GL_LINES);
		glLineWidth(6.0);
		glColor3f(.0f, 0.0f, 0.0f);
		glVertex3f(50.0f, 550.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(650.0f, 550.0f, 0.0f);
		glEnd();


		// Four Obstacles (Asteroids) 


		for (int i = 0; i < 5; i++) {
			glPushMatrix();
			glTranslated(obstacleX[i], obstacleY[i], 0);
			glRotated(rotationAngle, 0, 0, 1);
			glTranslated(-obstacleX[i], -obstacleY[i], 0.0);
			int segments = 200;
			glColor3f(0.4, 0.2, 0.0); // Brown color
			glBegin(GL_TRIANGLE_FAN);
			glVertex2f(obstacleX[i], obstacleY[i]); // Center point
			for (int j = 0; j <= segments; j++) {
				float angle = 2.0 * 3.1415926f * float(j) / float(segments);
				float dx = 20 * cos(angle);
				float dy = 20 * sin(angle);
				glVertex2f(obstacleX[i] + dx, obstacleY[i] + dy);
			}
			glEnd();
			glColor3f(1.0, 0.5, 0.0);
			for (int l = 0; l < 4; l++) {
				glBegin(GL_TRIANGLE_FAN);
				for (int j = 0; j <= segments; j++) {
					float angle = 2.0 * 3.1415926f * float(j) / float(segments);
					float dx = 0.4 * 20 * cos(angle);
					float dy = 0.2 * 20 * sin(angle);
					if (l % 2 == 0) {
						glVertex2f(obstacleX[i] + dx, obstacleY[i] + dy);
					}
					else {
						glVertex2f(obstacleX[i] + 0.7 * dx, obstacleY[i] + dy);
					}
				}
				glEnd();
			}
			glPointSize(5.0);
			glBegin(GL_POINTS);
			glPointSize(9.0);
			glColor3f(1.0, 0.0, 0.0);
			glVertex2f(obstacleX[i] - 5, obstacleY[i] - 8);
			glPointSize(7.0);
			glColor3f(1.0, 0.8, 0.8);
			glVertex2f(obstacleX[i] + 7, obstacleY[i] - 10);
			glPointSize(4.0);
			glColor3f(1.0, 1.0, .0);
			glVertex2f(obstacleX[i] - 10, obstacleY[i] + 8);
			glEnd();

			glPopMatrix();

		}




		// Collectables (stars) 
		for (int i = 0;i < 5;i++) {
			glPushMatrix();
			glTranslated(collectableX[i], collectableY[i], 0);
			glScaled(scaleFactor, scaleFactor, 1.0);
			glTranslated(-collectableX[i], -collectableY[i], 0);
			float yellow[3] = { 1.0, 1.0, 0.0 };
			float orange[3] = { 1.0, 0.5, 0.0 };

			// Draw the four-pointed star
			glBegin(GL_TRIANGLES);

			// Triangle 1 (upward)
			glColor3fv(yellow);
			glVertex2f(collectableX[i] - 5, collectableY[i]);
			glVertex2f(collectableX[i] + 5, collectableY[i]);
			glVertex2f(collectableX[i], collectableY[i] + 20);

			// Triangle 2 (rightward)
			glColor3fv(yellow);
			glVertex2f(collectableX[i], collectableY[i] + 5);
			glVertex2f(collectableX[i], collectableY[i] - 5);
			glVertex2f(collectableX[i] + 20, collectableY[i]);

			glEnd();
			glBegin(GL_TRIANGLES);
			// Triangle 3 (downward)
			glColor3fv(yellow);
			glVertex2f(collectableX[i] - 5, collectableY[i]);
			glVertex2f(collectableX[i] + 5, collectableY[i]);
			glVertex2f(collectableX[i], collectableY[i] - 20);

			// Triangle 4 (leftward)
			glColor3fv(yellow);
			glVertex2f(collectableX[i], collectableY[i] + 5);
			glVertex2f(collectableX[i], collectableY[i] - 5);
			glVertex2f(collectableX[i] - 20, collectableY[i]);
			glEnd();

			glBegin(GL_POINTS);
			glPointSize(5.0);
			glColor3f(1.0, 0.5, 0.0);
			glVertex2f(collectableX[i], collectableY[i] + 20);
			glVertex2f(collectableX[i] + 20, collectableY[i]);
			glVertex2f(collectableX[i], collectableY[i] - 20);
			glVertex2f(collectableX[i] - 20, collectableY[i]);


			glEnd();
			glPopMatrix();
		}


		// Moon powerUp
		for (int i = 0;i < 2;i++) {
			glPushMatrix();
			glTranslated(moonX[i], moonY[i], 0);
			glScaled(scaleFactor, scaleFactor, 1.0);
			glTranslated(-moonX[i], -moonY[i], 0.0);
			glColor3f(0.2, 0.2, 0.2);
			int segments = 200;
			glBegin(GL_TRIANGLE_FAN);
			glVertex2f(moonX[i], moonY[i]); // Center point
			for (int j = 0; j <= segments; j++) {
				float angle = 2.0 * 3.1415926f * j / segments;
				float dx = 20 * cos(angle);
				float dy = 20 * sin(angle);
				glVertex2f(moonX[i] + dx, moonY[i] + dy);
			}
			glEnd();

			glColor3f(1.0, 1.0, 1.0); // White color
			glPointSize(5.0);
			glBegin(GL_POINTS);
			glVertex2f(moonX[i] + 10, moonY[i] + 10);
			glVertex2f(moonX[i] - 10, moonY[i] - 10);
			glEnd();

			glColor3f(0.5, 0.5, 0.55); // White color
			glPointSize(9.0);
			glBegin(GL_POINTS);
			glVertex2f(moonX[i], moonY[i]);
			glEnd();

			glColor3f(0.0, 0.0, 0.0); // Black color
			glPointSize(5.0);
			glBegin(GL_POINTS);
			glVertex2f(moonX[i] + 15, moonY[i] - 5);
			glVertex2f(moonX[i] - 15, moonY[i] + 5);
			glEnd();
			glPopMatrix();



		}


		// Rocket powerUp
		for (int i = 0;i < 2;i++) {
			float rocketSize = 25;
			glPushMatrix();
			glTranslated(rocketX[i], rocketY[i], 0);
			glScaled(scaleFactor, scaleFactor, 1.0);
			glTranslated(-rocketX[i], -rocketY[i], 0);

			glColor3f(0.5, 0.5, 0.5); // Grey color
			glBegin(GL_POLYGON);
			glVertex2f(rocketX[i] - rocketSize / 2, rocketY[i] - rocketSize / 2);
			glVertex2f(rocketX[i] + rocketSize / 2, rocketY[i] - rocketSize / 2);
			glVertex2f(rocketX[i] + rocketSize / 2, rocketY[i] + rocketSize / 2);
			glVertex2f(rocketX[i] - rocketSize / 2, rocketY[i] + rocketSize / 2);
			glEnd();


			glColor3f(1.0, 0.0, 0.0); // Red color
			glBegin(GL_TRIANGLES);
			glVertex2f(rocketX[i] + rocketSize / 2, rocketY[i] + rocketSize / 2);
			glVertex2f(rocketX[i] - rocketSize / 2, rocketY[i] + rocketSize / 2);
			glVertex2f(rocketX[i], rocketY[i] + 3 * rocketSize / 4);
			glEnd();

			glColor3f(1.0, 0.5, 0.0); // Orange color
			glBegin(GL_LINES);
			glVertex2f(rocketX[i] - rocketSize / 4, rocketY[i] - rocketSize / 2);
			glVertex2f(rocketX[i] - rocketSize / 4, rocketY[i] - 3 * rocketSize / 4);
			glVertex2f(rocketX[i] + rocketSize / 4, rocketY[i] - rocketSize / 2);
			glVertex2f(rocketX[i] + rocketSize / 4, rocketY[i] - 3 * rocketSize / 4);
			glVertex2f(rocketX[i], rocketY[i] - rocketSize / 2);
			glVertex2f(rocketX[i], rocketY[i] - 3 * rocketSize / 4);
			glEnd();


			glColor3f(1.0, 1.0, 0.0); // Yellow color
			glPointSize(3.0);
			glBegin(GL_POINTS);
			glVertex2f(rocketX[i], rocketY[i] - 5);
			glVertex2f(rocketX[i], rocketY[i] + 5);
			glEnd();

			glPopMatrix();


		}

		// GOAL (earth) 
		glPushMatrix();
		glTranslated(75, 75, 0);
		glRotated(rotationAngle, 0, 0, 1);
		glTranslated(-75, -75, 0.0);
		int segments = 200;
		glBegin(GL_TRIANGLE_FAN);
		glColor3f(0.0, 0.0, 1.0);
		glVertex2f(75, 75); // Center point
		for (int j = 0; j <= segments; j++) {
			float angle = 2.0 * 3.1415926f * float(j) / float(segments);
			float dx = 20 * cos(angle);
			float dy = 20 * sin(angle);
			glVertex2f(75 + dx, 75 + dy);
		}
		glEnd();
		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_TRIANGLES);
		glVertex2f(70, 70);
		glVertex2f(65, 85);
		glVertex2f(75, 85);
		glVertex2f(70, 70);
		glVertex2f(75, 55);
		glVertex2f(80, 70);
		glEnd();
		glPointSize(5.0);
		glBegin(GL_POINTS);
		glVertex2f(72, 75);
		glVertex2f(73, 78);
		glVertex2f(64, 70);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex2f(75, 80);
		glVertex2f(78, 85);
		glVertex2f(82, 82);
		glVertex2f(80, 78);
		glVertex2f(80, 80);
		glVertex2f(83, 85);
		glVertex2f(87, 82);
		glVertex2f(85, 75);
		glVertex2f(85, 67);
		glVertex2f(88, 72);
		glVertex2f(92, 69);
		glVertex2f(90, 65);
		glEnd();
		glPopMatrix();
		glFlush();




	}
	else {
		if (health == 0) {
			glClear(GL_COLOR_BUFFER_BIT);
			// This is the word "Health Bar"
			glColor3f(1, 0, 0);
			char* p0s[20];
			sprintf((char*)p0s, "GAME OVER YOU LOST! ");
			print(200, 350, (char*)p0s);
			glFlush();
		}
		else {
			glClear(GL_COLOR_BUFFER_BIT);
			// This is the word "Health Bar"
			glColor3f(0, 0.5, 0);
			char* p0s[20];
			const int finalTime = gameTime;
			sprintf((char*)p0s, "YOU SCORED: %d in %d Seconds", score, 120 - finalTime);
			print(150, 350, (char*)p0s);
			glFlush();
		}
	}


}


void main(int argc, char** argr) {
	glutInit(&argc, argr);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowSize(700, 700);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Collector");
	lastTime = time(NULL);
	glutTimerFunc(1000, timer, 0);
	glutTimerFunc(0, updateRotation, 0);
	glutTimerFunc(0, updateScale, 0);
	glutDisplayFunc(Display);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glPointSize(9.0);
	glLineWidth(3.0);
	gluOrtho2D(0.0, 700.0, 0.0, 700.0);
	std::srand(std::time(nullptr));
	generateBackPositions();
	generateObstaclePositions();
	generateCollectablePositions();
	generateMoonPositions();
	generateRocketPositions();
	glutMainLoop();
}
