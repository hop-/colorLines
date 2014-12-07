#ifndef _CL_ATTRIBUTES_HPP_
#define _CL_ATTRIBUTES_HPP_

#include <stdlib.h>
#include <time.h>

#include <vector>
#include <iostream>

#define BOARD_SIZE	9

enum CLColor {
		COLOR1,
		COLOR2,
		COLOR3,
		COLOR4,
		COLOR5,
		COLOR6,
		COLOR7,
		NOCOLOR
	     };

class CLCell
{
	CLColor color;
	int x;
	int y;
public:
	CLCell(int cellIndex);
	CLCell(int posX, int posY);
	CLColor getColor();
	bool setColor(CLColor newColor);
	bool setColor(CLCell* &c);
	void resetColor();
};

class CLBoard
{
	CLCell* board[9][9];
	int currentX;
	int currentY;
	int m_score;
	CLColor nexts[3];
	static CLBoard* m_instance;
public:
	static CLBoard* getInstance();
	~CLBoard();
	void select(int posX, int posY);
	int getCommingColors();
	CLCell* getCell(int x, int y);
	void reset();
	bool isNotFill();
protected:
	CLBoard();
	void generateNexts();
	void clearLines(int x, int y);
	int getNmOfFreeCells();	
	void putNextsToBoard();
	bool hasWay(int posX, int posY);
};

#endif
