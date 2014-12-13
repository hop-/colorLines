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

bool Position::operator!=(const Position& b)
{
	return (x != b.x || y != b.y);
}

bool Position::operator==(const Position& b)
{
	return (x == b.x && y == b.y);
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
		m_changes(true),
		m_score()
{
	for (int x = 0; x < BOARD_SIZE; ++x) {
		for (int y = 0; y < BOARD_SIZE; ++y) {
			m_board[x][y] = new Cell();
		}
	}
	for (int i = 0; i < 3; ++i) {
		m_nexts[i] = new Cell();
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
			delete m_board[x][y];
		}
	}
	for (int i = 0; i < 3; ++i) {
		delete m_nexts[i];
	}
}

Cell* Board::getCell(int x, int y)
{
	return m_board[x][y];
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
	    m_board[p.x][p.y]->getColor() == NOCOLOR &&
	    m_board[m_currentSelection.x][m_currentSelection.y]->getColor() != NOCOLOR)
	{
		if (hasWay(p)) {
			m_board[p.x][p.y]->setColor(m_board[m_currentSelection.x][m_currentSelection.y]);
			if (!clearLines(p))
				putNextsToBoard();
			m_isSelected = false;
		}
	} else {
		m_currentSelection = p;
		m_isSelected = true;
	}
	m_changes = true;
}

std::vector<Cell*> Board::getCommingColors()
{
	std::vector<Cell*> commings;
	for (int i = 0; i < 3; ++i) {
		commings.push_back(m_nexts[i]);
	}
	return commings;
}

void Board::reset()
{
	for (int x = 0; x < BOARD_SIZE; ++x) {
		for (int y = 0; y < BOARD_SIZE; ++y) {
			m_board[x][y]->resetColor();
		}
	}
	m_score.newPlayerScore(0);
	srand(time(NULL));
	generateNexts();
	putNextsToBoard();
}

bool Board::clearLines(Position p)
{
	int dx, dy;
	std::vector<Cell*> allLines;
	allLines.push_back(m_board[p.x][p.y]);
	for (dx = 0; dx <= 1; ++dx) {
		for (dy = -1; dy <= 1; ++dy) {
			if (dx == 0 && dy < 1) {
				continue;
			}
			std::vector<Cell*> inLine;
			getInLines(&inLine, p, dx, dy);
			getInLines(&inLine, p, -dx, -dy);
			if (inLine.size() >= 4) {
				allLines.insert(allLines.end(),
						inLine.begin(),
						inLine.end());
			}
		}
	}
	if (allLines.size() >= 5) {
		for (int i = 0; i < static_cast<int>(allLines.size()); ++i) {
			allLines[i]->resetColor();
		}
		m_score.newPlayerScore(m_score.getPlayerScore() + 10 * (allLines.size() - 4));
		return true;
	}
	return false;
}

void Board::getInLines(std::vector<Cell*>* l, Position p, int dx, int dy)
{
	Position t;
	for (t = p; t.isCorrect(); t.x += dx, t.y += dy) {
		if (t == p) {
			continue;
		}
		if(m_board[t.x][t.y]->getColor() == m_board[p.x][p.y]->getColor()) {
			l->push_back(m_board[t.x][t.y]);
		} else {
			break;
		}
	}
}

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
		m_nexts[i]->resetColor();
		m_nexts[i]->setColor(static_cast<Color>(rand() % NOCOLOR));
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
					if (m_board[x][y]->getColor() == NOCOLOR) {
						if (++p == r) {
							break;
						}
					}
				}
				if (p == r) {
					break;
				}
			}
			m_board[x][y]->setColor(m_nexts[i]->getColor());
			Position tmp(x, y);
			clearLines(tmp);
			if (!isNotFill()) {
				break;
			}
		}
		generateNexts();
	}
}

int Board::getNmOfFreeCells()
{
	int i = 0;
	for (int x = 0 ; x < BOARD_SIZE; ++x) {
		for (int y = 0; y < BOARD_SIZE; ++y) {
			if (m_board[x][y]->getColor() == NOCOLOR) {
				++i;
			}
		}
	}
	return i;
}

bool Board::hasWay(Position p)
{
	bool tmpBoard[BOARD_SIZE][BOARD_SIZE] = {false};
	recFill(m_currentSelection, p, tmpBoard);
	return tmpBoard[p.x][p.y];
}

void Board::recFill(Position& crnt, const Position& p, bool brd[BOARD_SIZE][BOARD_SIZE])
{
	if (crnt.x != m_currentSelection.x || crnt.y != m_currentSelection.y) {
		if (!crnt.isCorrect() ||
		    brd[crnt.x][crnt.y] ||
		    m_board[crnt.x][crnt.y]->getColor() != NOCOLOR) {
			return;
		}
	}
	brd[crnt.x][crnt.y] = true;
	if (brd[p.x][p.y]) {
		return;
	}
	Position tmp = crnt;
	tmp.x++;
	recFill(tmp, p, brd);
	tmp.x -= 2;
	recFill(tmp, p, brd);
	tmp.x++;
	tmp.y++;
	recFill(tmp, p, brd);
	tmp.y -= 2;
	recFill(tmp, p, brd);
}

bool Board::isChange()
{
	return m_changes;
}

void Board::changesCatched()
{
	m_changes = false;
}

int Board::getBestScore()
{
	return m_score.getBest();
}

int Board::getPlayerScore()
{
	return m_score.getPlayerScore();
}
