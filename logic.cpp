#include "logic.hpp"
#include "test.hpp"

using namespace ColorLines;

//
//-----------------------------------------------------------
//

Position::Position(int posX = 0, int posY = 0) :
		x(posX),
		y(posY)
{}

bool Position::isCorrect()
{
	return (x >= 0 && x < BOARD_SIZE &&
		y >= 0 && y < BOARD_SIZE);
}

bool operator!=(Position& a, Position& b)
{
	return (a.x != b.x || a.y != b.y);
}

bool operator==(Position& a, Position& b)
{
	return (a.x == b.x && a.y == b.y);
}

//
//------------------------------------------------------------
//

Score::Score() :
		m_best(0),
		m_player(0),
		m_file("./resources/best")
{
	read();
}

Score::~Score()
{}

void Score::newPlayerScore(int newScore)
{
	m_player = newScore;
	if (m_player > m_best) {
		newBest();
	}
}

int Score::getPlayerScore()
{
	return m_player;
}

int Score::getBest()
{
	return m_best;
}

void Score::read()
{
	std::ifstream file(m_file.c_str(), std::ios::in | std::ios::binary);
	if (file.is_open()) {
		const int n = sizeof(int)/sizeof(char);
		file.read(reinterpret_cast<char*>(&m_best), n);
		file.close();
	}
}

void Score::newBest()
{
	m_best = m_player;
	std::ofstream file(m_file.c_str(), std::ios::out | std::ios::binary);
	if (file.is_open()) {
		const int n = sizeof(int)/sizeof(char);
		file.write(reinterpret_cast<char*>(&m_best), n);
		file.close();
	}
}

//
//-----------------------------------------------------------
//
Cell::Cell() : color(NOCOLOR)
{
}

Color Cell::getColor()
{
	return color;
}

bool Cell::setColor(Color newColor)
{
	if (color == NOCOLOR) {
		color = newColor;
		return true;
	} else {
		return false;
	}
}

void Cell::resetColor()
{
	color = NOCOLOR;
}

bool Cell::setColor(Cell* &c)
{
	Color cColor = c->getColor();
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
Board* Board::m_instance = NULL;

Board::Board() : 
		m_currentSelection(-1, -1),
		m_isSelected(false),
		m_score()
{
	for (int x = 0; x < BOARD_SIZE; ++x) {
		for (int y = 0; y < BOARD_SIZE; ++y) {
			board[x][y] = new Cell();
		}
	}
	for (int i = 0; i < 3; ++i) {
		nexts[i] = new Cell();
	}
	reset();
}

Board* Board::getInstance()
{
	if (m_instance == NULL) {
		m_instance = new Board();
	}
	return m_instance;
}

Board::~Board()
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

Cell* Board::getCell(int x, int y)
{
	return board[x][y];
}

void Board::select(int posX, int posY)
{
	Position p(posX, posY);
	select(p);
}

void Board::select(Position p)
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

std::vector<Cell*> Board::getCommingColors()
{
	std::vector<Cell*> commings;
	for (int i = 0; i < 3; ++i) {
		commings.push_back(nexts[i]);
	}
	return commings;
}

void Board::reset()
{
	for (int x = 0; x < BOARD_SIZE; ++x) {
		for (int y = 0; y < BOARD_SIZE; ++y) {
			board[x][y]->resetColor();
		}
	}
	m_score.newPlayerScore(0);
	srand(time(NULL));
	generateNexts();
	putNextsToBoard();
	generateNexts();
}

void Board::clearLines(Position p)
{}

bool Board::isNotFill()
{
	return  static_cast<bool>(getNmOfFreeCells());
}

Position Board::getSelection()
{
	return m_currentSelection;
}

bool Board::isSelected()
{
	return m_isSelected;
}

void Board::generateNexts()
{
	for (int i = 0; i < 3; ++i) {
		nexts[i]->resetColor();
		nexts[i]->setColor(static_cast<Color>(rand() % NOCOLOR));
	}
}

void Board::putNextsToBoard()
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
			Position tmp(x, y);
			clearLines(tmp);
			if (!isNotFill()) {
				break;
			}
		}
	}
}

int Board::getNmOfFreeCells()
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

bool Board::hasWay(Position p)
{
	//TODO
	return true;
}

int Board::getBestScore()
{
	return m_score.getBest();
}

int Board::getPlayerScore()
{
	return m_score.getPlayerScore();
}
