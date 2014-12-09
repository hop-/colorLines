#include "logic.hpp"
#include "test.hpp"

//
//-----------------------------------------------------------
//

CLPosition::CLPosition(int posX = 0, int posY = 0) :
		x(posX),
		y(posY)
{}

bool CLPosition::isCorrect()
{
	return (x >= 0 && x < BOARD_SIZE &&
		y >= 0 && y < BOARD_SIZE);
}

bool operator!=(CLPosition& a, CLPosition& b)
{
	return (a.x != b.x || a.y != b.y);
}

bool operator==(CLPosition& a, CLPosition& b)
{
	return (a.x == b.x && a.y == b.y);
}

//
//-----------------------------------------------------------
//
CLCell::CLCell() : color(NOCOLOR)
{
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
		m_currentSelection(-1, -1),
		m_isSelected(false)
{
	for (int x = 0; x < BOARD_SIZE; ++x) {
		for (int y = 0; y < BOARD_SIZE; ++y) {
			board[x][y] = new CLCell();
		}
	}
	for (int i = 0; i < 3; ++i) {
		nexts[i] = new CLCell();
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
	for (int i = 0; i < 3; ++i) {
		delete nexts[i];
	}
}

CLCell* CLBoard::getCell(int x, int y)
{
	return board[x][y];
}

void CLBoard::select(int posX, int posY)
{
	CLPosition p(posX, posY);
	select(p);
}

void CLBoard::select(CLPosition p)
{
	if (!isNotFill() ||
	    !p.isCorrect())
	{
		return;
	}
	if (m_isSelected &&
	    board[p.x][p.y]->getColor() == NOCOLOR &&
	    board[m_currentSelection.x][m_currentSelection.y]->getColor() != NOCOLOR &&
	    hasWay(p))
	{
		board[p.x][p.y]->setColor(board[m_currentSelection.x][m_currentSelection.y]);
		clearLines(p);
		putNextsToBoard();
		generateNexts();
		m_isSelected = false;
	} else {
		m_currentSelection = p;
		m_isSelected = true;
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

void CLBoard::clearLines(CLPosition p)
{}

bool CLBoard::isNotFill()
{
	return  static_cast<bool>(getNmOfFreeCells());
}

CLPosition CLBoard::getSelection()
{
	return m_currentSelection;
}

bool CLBoard::isSelected()
{
	return m_isSelected;
}

void CLBoard::generateNexts()
{
	for (int i = 0; i < 3; ++i) {
		nexts[i]->resetColor();
		nexts[i]->setColor(static_cast<CLColor>(rand() % NOCOLOR));
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
			board[x][y]->setColor(nexts[i]->getColor());
			CLPosition tmp(x, y);
			clearLines(tmp);
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

bool CLBoard::hasWay(CLPosition p)
{
	//TODO
	return true;
}
