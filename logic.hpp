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

struct CLPosition {
	int x;
	int y;

	CLPosition(int posX, int posY);
	bool isCorrect();
};

bool operator!=(CLPosition& a, CLPosition& b);
bool operator==(CLPosition& a, CLPosition& b);

class CLCell
{
	CLColor color;
public:
	CLCell();
	CLColor getColor();
	bool setColor(CLColor newColor);
	bool setColor(CLCell* &c);
	void resetColor();
};

class CLBoard
{
	CLCell* board[9][9];
	CLPosition m_currentSelection;
	bool m_isSelected;
	int m_score;
	CLCell* nexts[3];
	static CLBoard* m_instance;
public:
	static CLBoard* getInstance();
	~CLBoard();
	void select(int posX, int posY);
	void select(CLPosition p);
	int getCommingColors();
	CLCell* getCell(int x, int y);
	void reset();
	bool isNotFill();
	CLPosition getSelection();
	bool isSelected();
protected:
	CLBoard();
	void generateNexts();
	void clearLines(CLPosition p);
	int getNmOfFreeCells();	
	void putNextsToBoard();
	bool hasWay(CLPosition p);
};

#endif
