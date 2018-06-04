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
#include "blocks.h"

//using namespace std;

Blocks::Blocks() {}
Blocks::~Blocks() {}


//Creation of blocks
void Blocks::createBlocks() {
	blockDimensions = 0;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				enableBlocks[i][j][k] = 0;
			}
		}
	}
}

//Initialise the blocks
void Blocks::initBlocks() {
	srand(time(0));

	bool acceptedBlock = false;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				if (rand() % 100 > 40)
				{
					enableBlocks[i][j][k] = 1;
					blockDimensions++;

					if (blockDimensions > 1)
					{
						if (i > 0 && !acceptedBlock)
						{
							if (enableBlocks[i - 1][j][k] == 1)
								acceptedBlock = true;
						}

						if (j > 0 && !acceptedBlock)
						{
							if (enableBlocks[i][j - 1][k] == 1)
								acceptedBlock = true;
						}

						if (k > 0 && !acceptedBlock)
						{
							if (enableBlocks[i][j][k - 1] == 1)
								acceptedBlock = true;
						}

						if (i < 2 && !acceptedBlock)
						{
							if (enableBlocks[i + 1][j][k] == 1)
								acceptedBlock = true;
						}

						if (j < 2 && !acceptedBlock)
						{
							if (enableBlocks[i][j + 1][k] == 1)
								acceptedBlock = true;
						}

						if (k < 2 && !acceptedBlock)
						{
							if (enableBlocks[i][j][k + 1] == 1)
								acceptedBlock = true;
						}


						if (!acceptedBlock)
						{
							enableBlocks[i][j][k] = 0;
							blockDimensions--;
						}
						else
						{
							acceptedBlock = false;
						}
					}
				}
			}
		}
	}

	if (blockDimensions == 0)
	{
		enableBlocks[1][1][1] = 1;
		blockDimensions++;
	}
}

void Blocks::drawBlocks(int d, float x, float y, float z, int horizontalRotation, int vertical_x, int vertical_z) {
	int r = 0, g = 0, b = 0;

	if (blockDimensions < 10)
	{
		r = 1;
		g = 1;
		blockColor = 1;
	}
	else if (blockDimensions >= 20)
	{
		r = 1;
		blockColor = 3;
	}
	else
	{
		b = 1;
		blockColor = 2;
	}

	for (int j = 0; j < 3; j++)
	{
		for (int k = 0; k < 3; k++)
		{
			for (int l = 0; l < 3; l++)
			{
				if (enableBlocks[j][k][l] == 1)
				{
					glPushMatrix();

					glTranslatef(0, 0, 0);
					glRotatef(horizontalRotation, 0, 1, 0);
					glRotatef(vertical_x, 1, 0, 0);
					glRotatef(vertical_z, 0, 0, 1);

					glColor3f(r, g, b);
					glTranslatef(j + x, y - k, l + z);
					//glutSolidCube(0.99);
					glutWireCube(1);

					glPopMatrix();
				}
			}
		}
	}
}

void Blocks::rotateBlocksOnXAxis() {
	int temp[3][3][3];
	int i, j, k;

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			for (k = 0; k < 3; k++)
				temp[i][j][k] = 0;

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			for (k = 0; k < 3; k++)
				temp[i][j][k] = enableBlocks[i][2 - k][j];

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			for (k = 0; k < 3; k++)
				enableBlocks[i][j][k] = temp[i][j][k];
}

void Blocks::rotateBlocksOnYAxis() {
	int temp[3][3][3];
	int i, j, k;

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			for (k = 0; k < 3; k++)
				temp[i][j][k] = 0;

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			for (k = 0; k < 3; k++)
				temp[i][j][k] = enableBlocks[2 - k][j][i];

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			for (k = 0; k < 3; k++)
				enableBlocks[i][j][k] = temp[i][j][k];
}

void Blocks::rotateBlocksOnZAxis() {
	int temp[3][3][3];
	int i, j, k;

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			for (k = 0; k < 3; k++)
				temp[i][j][k] = 0;

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			for (k = 0; k < 3; k++)
				temp[i][j][k] = enableBlocks[j][2 - i][k];

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			for (k = 0; k < 3; k++)
				enableBlocks[i][j][k] = temp[i][j][k];
}