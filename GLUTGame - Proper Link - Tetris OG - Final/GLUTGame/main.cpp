//Include libraries
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glut.h>

//Included header files
#include "blocks.h"

//Testing aspect ratios and to maximise screen on gameplay
/*
const int WIDTH = 490;
const int HEIGHT = 600;
const float ASPECT = float(WIDTH) / HEIGHT;   // desired aspect ratio
*/
 /*
void reshapeScene(GLint width, GLint height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	int w = height * ASPECT;           // w is width adjusted for aspect ratio
	int left = (width - w) / 2;
	glViewport(left, 0, w, height);       // fix up the viewport to maintain aspect ratio
	gluOrtho2D(0, WIDTH, HEIGHT, 0);   // only the window is changing, not the camera
	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}
*/

/*
int width = 720;
int height = 480;
char* gameTitle;

float viewingAngle = 45;
float z_near = 1.0f;
float z_far = 500.0f;
*/

typedef struct {
	int width;
	int height;
	char* gameTitle;

	float viewingAngle;
	float z_close;
	float z_far;
}


glutWindow;

glutWindow window;

bool maximiseScreen = false;       					

Blocks shape;

bool gameOver = false;

int score = 0;

int zoomDepth = 40;

float movement_x = -1;
float movement_z = -1;

int d = 10.00;

int vertical_x = 0;
int vertical_z = 0;

int horizontalRotation = 45;
int rotation_speed = 7;

float drop_speed = 0.001;

float drop = d / 2 + 2;


bool stationary = true;
int bottom[10][10][10];

void *font = GLUT_BITMAP_TIMES_ROMAN_24;

//World boundary check

void boundaryCheck(int undox, int undoz) {
	int i, j, k;
	bool draw = false;

	if (drop > -d / 2)
	{
		for (i = 0; i < 3; i++)
		{
			for (j = 0; j < 3; j++)
			{
				for (k = 0; k < 3; k++)
				{
					if (shape.enableBlocks[i][k][j] == 1 && bottom[(int)movement_x + d / 2 + i][(int)drop + d / 2 - k][(int)movement_z + d / 2 + j] != 0)
					{
						draw = true;
						break;
					}
					else if ((shape.enableBlocks[i][k][j] == 1 && movement_x + i > 7) || (shape.enableBlocks[i][k][j] == 1 && movement_z + j > 7))
					{
						draw = true;
						break;
					}
					else if (shape.enableBlocks[i][k][j] == 1 && movement_x + i < -7)
					{
						draw = true;
						break;
					}
					else if (shape.enableBlocks[i][k][2 - j] == 1 && movement_z + 2 - j < -7)
					{
						draw = true;
						break;
					}
				}

				if (draw)	break;
			}

			if (draw)	break;
		}

		for (i = 0; i < 3; i++)
		{
			for (j = 0; j < 3; j++)
			{
				for (k = 0; k < 3; k++)
				{
					if ((shape.enableBlocks[i][k][j] == 1 && movement_x + i < -7))
					{
						draw = true;
						break;
					}
				}

				if (draw)	break;
			}

			if (draw)	break;
		}


		if (draw)
		{
			movement_x = undox;
			movement_z = undoz;
		}
	}
}






void destroyLevel() {
	int count = 0;
	int o = 0;

	for (int k = 0; k < 10; k++)
	{
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (bottom[i][k][j] != 0)
					count++;
			}
		}

		if (count == 100) //10 x 10 = 100 so if there's 100 blocks on one level then level destroy
		{
			o++;
			score++;
			if (k > 0) k--;
		}

		if (o > 0)
		{
			for (int l = 0; l < 10; l++)
			{
				for (int m = 0; m < 10; m++)
				{
					if (k + o < 10)
						bottom[l][k][m] = bottom[l][k + o][m];
					else
						bottom[l][k][m] = 0;
				}
			}
		}

		count = 0;
	}

}



void bottomPlacement() {
	int i, j, k;
	bool draw = false;
	bool yellowBlocks = false;
	bool blueBlocks = false;

	if (drop > -d / 2)
	{
		for (k = 2; k >= 0; k--)
		{
			for (i = 0; i < 3; i++)
			{
				for (j = 0; j < 3; j++)
				{
					if (shape.blockColor == 1)
					{
						if (shape.enableBlocks[i][k][j] == 1 && bottom[(int)movement_x + d / 2 + i][(int)drop + d / 2 - 1 - k][(int)movement_z + d / 2 + j] != 0)
						{
							draw = true;
							break;
						}
					}
					else if (shape.blockColor == 3)
					{
						if (shape.enableBlocks[i][k][j] == 1 && bottom[(int)movement_x + d / 2 + i][(int)drop + d / 2 - k][(int)movement_z + d / 2 + j] != 0)
						{
							bottom[(int)movement_x + d / 2 + i][(int)drop + d / 2 - k][(int)movement_z + d / 2 + j] = 0;
							score++;
						}
					}
					else
					{
						if (shape.enableBlocks[i][k][j] == 1 && bottom[(int)movement_x + d / 2 + i][(int)drop + d / 2 - 1 - k][(int)movement_z + d / 2 + j] != 0
							&& bottom[(int)movement_x + d / 2 + i][(int)drop + d / 2 - 2 - k][(int)movement_z + d / 2 + j] != 0
							&& bottom[(int)movement_x + d / 2 + i][(int)drop + d / 2 - 3 - k][(int)movement_z + d / 2 + j] != 0)
						{
							blueBlocks = true;
							draw = true;
							break;
						}
						else
						{
							if (shape.enableBlocks[i][k][j] == 1 && bottom[(int)movement_x + d / 2 + i][(int)drop + d / 2 - k][(int)movement_z + d / 2 + j] != 0)
							{
								bottom[(int)movement_x + d / 2 + i][(int)drop + d / 2 - k][(int)movement_z + d / 2 + j] = 0;
								score++;
							}
						}
					}
				}

				if (draw)	break;
			}

			if (draw)	break;
		}

		if (draw)
		{
			for (i = 0; i < 3; i++)
			{
				for (j = 0; j < 3; j++)
				{
					for (k = 0; k < 3; k++)
					{
						if (shape.blockColor == 1)
						{
							if (shape.enableBlocks[i][k][j] == 1)
								bottom[(int)movement_x + d / 2 + i][(int)drop + d / 2 - k][(int)movement_z + d / 2 + j] = shape.blockColor;
						}
						else if (shape.blockColor == 2 && blueBlocks)
						{
							if (shape.enableBlocks[i][k][j] == 1)
								bottom[(int)movement_x + d / 2 + i][(int)drop + d / 2 - k][(int)movement_z + d / 2 + j] = shape.blockColor;
						}
						else if (shape.blockColor == 3)
						{
							yellowBlocks = true;
						}
					}
				}
			}
		}

		if (draw && !yellowBlocks)
		{
			destroyLevel();
			if (drop < d / 2 - 1)
			{
				shape.createBlocks();
				shape.initBlocks();
			}
			else
			{
				gameOver = true;
			}

			drop = d / 2;
			movement_x = -1;
			movement_z = -1;
		}
	}

	if (drop < 2 - d / 2)
	{
		for (k = 2; k >= 0; k--)
		{
			for (i = 0; i < 3; i++)
			{
				for (j = 0; j < 3; j++)
				{
					if ((shape.enableBlocks[i][k][j] == 1 && (int)drop + d / 2 - k == 0) || (draw && shape.enableBlocks[i][k][j] == 1))
					{
						bottom[(int)movement_x + d / 2 + i][(int)drop + d / 2 - k][(int)movement_z + d / 2 + j] = shape.blockColor;
						draw = true;
					}
				}
			}
		}

		if (draw)
		{
			destroyLevel();
			shape.createBlocks();
			shape.initBlocks();
			drop = d / 2;
			movement_x = -1;
			movement_z = -1;
		}
	}

}

void drawBottom() {

	for (int j = 0; j < 10; j++)
	{
		for (int k = 0; k < 10; k++)
		{
			for (int l = 0; l < 10; l++)
			{
				if (bottom[j][k][l] != 0)
				{
					glPushMatrix();
					glTranslatef(0, 0, 0);
					glRotatef(horizontalRotation, 0, 1, 0);
					glRotatef(vertical_x, 1, 0, 0);
					glRotatef(vertical_z, 0, 0, 1);

					if (bottom[j][k][l] == 1)
						glColor3f(1, 1, 0);
					else if (bottom[j][k][l] == 2)
						glColor3f(0, 0, 1);
					else
						glColor3f(1, 0, 0);

					glTranslatef(j - d / 2, k - d / 2, l - d / 2);
					glutWireCube(1);
					glutSolidCube(0.99);
					glPopMatrix();
				}
			}
		}
	}

}

void onScreenText(float x, float y, char *String) {
	char *c;

	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 10, 0, 10);

	glColor3f(9.0f, 0.7f, 0.4f);
	glRasterPos2f(x, y);
	glDisable(GL_LIGHTING);

	for (c = String; *c != '\0'; c++)
		glutBitmapCharacter(font, *c);

	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}


void display() {
	float i;
	char scoreText[35];
	char scoreCounter[20];

	strcpy_s(scoreText, "p = play/pause... S C O R E : ");
	sprintf_s(scoreCounter, "%d", score);
	strcat_s(scoreText, scoreCounter);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	onScreenText(1.0f, 1.0f, scoreText);

	glLoadIdentity();
	gluLookAt(zoomDepth, 2, 0, 0, 0, 0, 0, 1, 0);


	glPushMatrix();

	glColor3f(0, 1, 1);
	glTranslatef(0, 0, 0);
	glRotatef(horizontalRotation, 0, 1, 0);
	glRotatef(vertical_x, 1, 0, 0);
	glRotatef(vertical_z, 0, 0, 1);


	glColor3f(0.6, 0.5, 0.8);

	//Use 10.00 symbolising the bottom width

	//glBegin(GL_Lines) for the lines and grid system
	for (i = -10.00 / 2; i <= 10.00 / 2; i++)
	{
		glBegin(GL_LINES);
		glVertex3f(i, 10.00 / 2, -10.00 / 2);
		glVertex3f(i, -10.00 / 2, -10.00 / 2);
		glEnd();
	}

	for (i = -10.00 / 2; i <= 10.00 / 2; i++)
	{
		glBegin(GL_LINES);
		glVertex3f(10.00 / 2, i, -10.00 / 2);
		glVertex3f(-10.00 / 2, i, -10.00 / 2);
		glEnd();
	}

	for (i = -10.00 / 2; i <= 10.00 / 2; i++)
	{
		glBegin(GL_LINES);
		glVertex3f(-10.00 / 2, 10.00 / 2, i);
		glVertex3f(-10.00 / 2, -10.00 / 2, i);
		glEnd();
	}

	for (i = -10.00 / 2; i <= 10.00 / 2; i++)
	{
		glBegin(GL_LINES);
		glVertex3f(-10.00 / 2, i, 10.00 / 2);
		glVertex3f(-10.00 / 2, i, -10.00 / 2);
		glEnd();
	}


	glColor3f(0.9, 0.9, 0.6);
	for (i = -10.00 / 2; i <= 10.00 / 2; i++)
	{
		glBegin(GL_LINES);
		glVertex3f(i, -10.00 / 2, -10.00 / 2);
		glVertex3f(i, -10.00 / 2, 10.00 / 2);
		glEnd();
	}

	for (i = -10.00 / 2; i <= 10.00 / 2; i++)
	{
		glBegin(GL_LINES);
		glVertex3f(10.00 / 2, -10.00 / 2, i);
		glVertex3f(-10.00 / 2, -10.00 / 2, i);
		glEnd();
	}

	glPopMatrix();

	if (!gameOver)
	{
		shape.drawBlocks(d, movement_x, drop, movement_z, horizontalRotation, vertical_x, vertical_z);

		bottomPlacement();
	}
	else
	{
		strcpy_s(scoreText, "G A M E  O V E R");
		onScreenText(4.0f, 5.0f, scoreText);
	}

	drawBottom();

	if (!stationary)
	{
		drop -= drop_speed;
	}

	glutSwapBuffers();
	glFlush();

}

/*
Key handling
*/

//Arrow keys and screen maximisation
void arrowKeysAndMaximiseScreen(int keys, int x, int y) {
	int degrees = 0;

	if (horizontalRotation < 0)
		degrees = horizontalRotation % 360 + 360;
	else
		degrees = horizontalRotation % 360;

	int undox = movement_x;
	int undoz = movement_z;

	switch (keys) {
	case GLUT_KEY_F1:       			// F1 toggles maximiseScreen mode
		maximiseScreen = !maximiseScreen;
		if (maximiseScreen) glutFullScreen();
		else glutReshapeWindow(500, 500);
		break;

	case GLUT_KEY_LEFT:
		if (!stationary) {
			if (degrees <= 45 || degrees > 315)
			{
				if (movement_z < d / 2)
					movement_z++;
			}
			else if (degrees > 135 && degrees <= 225)
			{
				if (movement_z > -d / 2 - 2)
					movement_z--;
			}
			else if (degrees > 45 && degrees <= 135)
			{
				if (movement_x > -d / 2 - 2)
					movement_x--;
			}
			else if (degrees > 225 && degrees <= 315)
			{
				if (movement_x < d / 2)
					movement_x++;
			}
		}

		break;

	case GLUT_KEY_RIGHT:
		if (!stationary) {
			if (degrees <= 45 || degrees > 315)
			{
				if (movement_z > -d / 2 - 2)
					movement_z--;
			}
			else if (degrees > 135 && degrees <= 225)
			{
				if (movement_z < d / 2)
					movement_z++;
			}
			else if (degrees > 45 && degrees <= 135)
			{
				if (movement_x < d / 2)
					movement_x++;
			}
			else if (degrees > 225 && degrees <= 315)
			{
				if (movement_x > -d / 2 - 2)
					movement_x--;
			}
		}
		break;

	case GLUT_KEY_UP:
		if (!stationary) {
			if (degrees <= 45 || degrees > 315)
			{
				if (movement_x > -d / 2 - 2)
					movement_x--;
			}
			else if (degrees > 135 && degrees <= 225)
			{
				if (movement_x < d / 2)
					movement_x++;
			}
			else if (degrees > 45 && degrees <= 135)
			{
				if (movement_z > -d / 2 - 2)
					movement_z--;
			}
			else if (degrees > 225 && degrees <= 315)
			{
				if (movement_z < d / 2)
					movement_z++;
			}
		}
		break;

	case GLUT_KEY_DOWN:
		if (!stationary) {
			if (degrees <= 45 || degrees > 315)
			{
				if (movement_x < d / 2)
					movement_x++;
			}
			else if (degrees > 135 && degrees <= 225)
			{
				if (movement_x > -d / 2 - 2)
					movement_x--;
			}
			else if (degrees > 45 && degrees <= 135)
			{
				if (movement_z < d / 2)
					movement_z++;
			}
			else if (degrees > 225 && degrees <= 315)
			{
				if (movement_z > -d / 2 - 2)
					movement_z--;
			}
		}
		break;

	default:
		break;
	}

	if (!stationary)
		boundaryCheck(undox, undoz);
}

void keyboardInputs(unsigned char key, int mousePositionX, int mousePositionY)
{
	int degrees = 0;

	if (horizontalRotation < 0)
		degrees = horizontalRotation % 360 + 360;
	else
		degrees = horizontalRotation % 360;


	switch (key)
	{

	case 'p':
		if (stationary)
			stationary = false;
		else
			stationary = true;

		break;

	case 'w':
		if (degrees <= 45 || degrees > 315)
		{
			vertical_z += rotation_speed;
		}
		else if (degrees > 135 && degrees <= 225)
		{
			vertical_z -= rotation_speed;
		}
		else if (degrees > 45 && degrees <= 135)
		{
			vertical_x -= rotation_speed;
		}
		else if (degrees > 225 && degrees <= 315)
		{
			vertical_x += rotation_speed;
		}

		break;

	case 's':
		if (degrees <= 45 || degrees > 315)
		{
			vertical_z -= rotation_speed;
		}
		else if (degrees > 135 && degrees <= 225)
		{
			vertical_z += rotation_speed;
		}
		else if (degrees > 45 && degrees <= 135)
		{
			vertical_x += rotation_speed;
		}
		else if (degrees > 225 && degrees <= 315)
		{
			vertical_x -= rotation_speed;
		}

		break;

	case '-':
		if (zoomDepth > 2)
			zoomDepth -= 2;
		break;

	case '=':
		if (zoomDepth < 100)
			zoomDepth += 2;
		break;

	case 'a':
		horizontalRotation += rotation_speed;
		break;

	case 'd':
		horizontalRotation -= rotation_speed;
		break;

	case '1':
		if (!stationary)
			shape.rotateBlocksOnXAxis();
		break;

	case '2':
		if (!stationary)
			shape.rotateBlocksOnYAxis();
		break;

	case '3':
		if (!stationary)
			shape.rotateBlocksOnZAxis();
		break;

	case 27:
		exit(0);
		break;

	default:
		break;
	}
}


void initialise()
{
	// select projection matrix
	glMatrixMode(GL_PROJECTION);

	// set the viewport
	glViewport(0, 0, window.width, window.height);

	// set matrix mode
	glMatrixMode(GL_PROJECTION);

	// reset projection matrix
	glLoadIdentity();
	GLfloat aspect = (GLfloat)window.width / window.height;

	// set up a perspective projection matrix
	gluPerspective(window.viewingAngle, aspect, window.z_close, window.z_far);

	// specify which matrix is the current matrix
	glMatrixMode(GL_MODELVIEW);
	glShadeModel(GL_SMOOTH);

	// specify the clear value for the depth buffer
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// specify implementation-specific hints
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	GLfloat amb_light[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat diffuse[] = { 0.4, 0.4, 0.4, 1 };
	GLfloat specular[] = { 0.4, 0.4, 0.4, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glClearColor(0.0, 0.5, 0.0, 6.0);
}

int main(int argc, char** argv) {

	window.width = 720;
	window.height = 480;
	window.gameTitle == "OpenGL Uni Game";
	window.viewingAngle = 45;
	window.z_close = 1.0f;
	window.z_far = 500.0f;

	for (int i = 0; i < d; i++)
	{
		for (int j = 0; j < d; j++)
		{
			for (int k = 0; k < d; k++)
			{
				bottom[i][j][k] = 0;
			}
		}
	}

	shape.createBlocks();
	shape.initBlocks();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(window.width, window.height);
	glutCreateWindow("OpenGL Uni Game");
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(keyboardInputs);
	glutSpecialFunc(arrowKeysAndMaximiseScreen);

	initialise();
	glutMainLoop();

	return 0;
}