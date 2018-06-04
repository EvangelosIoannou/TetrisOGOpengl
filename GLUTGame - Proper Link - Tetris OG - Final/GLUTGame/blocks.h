#ifndef BLOCKS_H_
#define BLOCKS_H_

//Handling all the block functions
class Blocks {
public:
	int enableBlocks[3][3][3];
	int blockDimensions;
	int blockColor;

	Blocks();
	virtual ~Blocks();

	void initBlocks();
	void createBlocks();
	void drawBlocks(int d, float x, float y, float z, int horizontalRotation, int vertical_x, int vertical_z);
	void rotateBlocksOnXAxis();
	void rotateBlocksOnYAxis();
	void rotateBlocksOnZAxis();
};

#endif /* BLOCKS_H_ */