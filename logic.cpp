#include "logic.hpp"
#include "test.hpp"


//
//-----------------------------------------------------------
//
CLCell::CLCell(int cellIndex) : color(NOCOLOR)
{
	y = int(cellIndex / 9);
	x = cellIndex % 9;
}

CLCell::CLCell(int posX, int posY) : color(NOCOLOR)
{
	x = posX;
	y = posY;
}


CLColor CLCell::getColor()
{
	return color;
}

bool CLCell::setColor(CLColor newColor)
{
	if (color == NOCOLOR) {
		color = newColor;
		return true;
	} else {
		return false;
	}
}

void CLCell::resetColor()
{
	color = NOCOLOR;
}

bool CLCell::setColor(CLCell* &c)
{
	CLColor cColor = c->getColor();
	if (color == NOCOLOR && cColor != NOCOLOR) {
		color = cColor;
		c->resetColor();
		return true;
	} else {
		return false;
	}
}
//
//------------------------------------------------------------
//
CLBoard* CLBoard::m_instance = NULL;

CLBoard::CLBoard() : 
		currentX(0),
		currentY(0)
{
	for (int x = 0; x < BOARD_SIZE; ++x) {
		for (int y = 0; y < BOARD_SIZE; ++y) {
			board[x][y] = new CLCell(x, y);
		}
	}
	reset();
}

CLBoard* CLBoard::getInstance()
{
	if (m_instance == NULL) {
		m_instance = new CLBoard();
	}
	return m_instance;
}

CLBoard::~CLBoard()
{
	for (int x = 0; x < BOARD_SIZE; ++x) {
		for (int y = 0; y < BOARD_SIZE; ++y) {
			delete board[x][y];
		}
	}
}

CLCell* CLBoard::getCell(int x, int y)
{
	return board[x][y];
}

void CLBoard::select(int posX, int posY)
{
	if (isNotFill() &&
			!(posX == currentX && posY == currentY) &&
	    		posX >= 0 && posX < 9 &&
	    		posY >= 0 && posY < 9) {
	    	if (board[posX][posY]->getColor() == NOCOLOR &&
		    board[currentX][currentY]->getColor() != NOCOLOR &&
		    hasWay(posX, posY)) {
			board[posX][posY]->setColor(board[currentX][currentY]);
			clearLines(posX, posY);
			putNextsToBoard();
			generateNexts();
		} else {
			currentX = posX;
			currentY = posY;
		}
	}
}

int CLBoard::getCommingColors()
{
	//TODO
	return 0;
}

void CLBoard::reset()
{
	for (int x = 0; x < BOARD_SIZE; ++x) {
		for (int y = 0; y < BOARD_SIZE; ++y) {
			board[x][y]->resetColor();
		}
	}
	m_score = 0;
	srand(time(NULL));
	generateNexts();
	putNextsToBoard();
}

void CLBoard::clearLines(int x, int y)
{}

bool CLBoard::isNotFill()
{
	return  static_cast<bool>(getNmOfFreeCells());
}

void CLBoard::generateNexts()
{
	for (int i = 0; i < 3; ++i) {
		nexts[i] = static_cast<CLColor>(rand() % NOCOLOR);
	}
}

void CLBoard::putNextsToBoard()
{
	if (isNotFill()) {
		for (int i = 0; i < 3; ++i) {
			int r = rand() % getNmOfFreeCells() ;
			int p = -1;
			int x;
			int y;
			for (x = 0; x < BOARD_SIZE; ++x) {
				for (y = 0; y < BOARD_SIZE; ++y) {
					if (board[x][y]->getColor() == NOCOLOR) {
						if (++p == r) {
							break;
						}
					}
				}
				if (p == r) {
					break;
				}
			}
			if (x > 8 || y > 8) {
				_PRINT(r);
				_PRINT(p);
				_PXY(x, y);
			}
			board[x][y]->setColor(nexts[i]);
			clearLines(x, y);
			if (!isNotFill()) {
				break;
			}
		}
	}
}

int CLBoard::getNmOfFreeCells()
{
	int i = 0;
	for (int x = 0 ; x < BOARD_SIZE; ++x) {
		for (int y = 0; y < BOARD_SIZE; ++y) {
			if (board[x][y]->getColor() == NOCOLOR) {
				++i;
			}
		}
	}
	return i;
}

bool CLBoard::hasWay(int posX, int posY)
{
	//TODO
	return true;
}
