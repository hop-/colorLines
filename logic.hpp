#ifndef _CL_ATTRIBUTES_HPP_
#define _CL_ATTRIBUTES_HPP_

#include <stdlib.h>
#include <time.h>

#include <vector>
#include <iostream>
#include <string>
#include <fstream>

#define BOARD_SIZE	9

namespace ColorLines {

	enum Color {
			COLOR1,
			COLOR2,
			COLOR3,
			COLOR4,
			COLOR5,
			COLOR6,
			COLOR7,
			NOCOLOR
	};

	struct Position {
		int x;
		int y;

		Position(int posX, int posY);
		bool isCorrect();
	};

	bool operator!=(Position& a, Position& b);
	bool operator==(Position& a, Position& b);

	class Score
	{
		int m_best;
		int m_player;
		std::string m_file;
	public:
		Score();
		~Score();
		void newPlayerScore(int newScore);
		int getPlayerScore();
		int getBest();
	private:
		void read();
		void newBest();
	};

	class Cell
	{
		Color color;
	public:
		Cell();
		Color getColor();
		bool setColor(Color newColor);
		bool setColor(Cell* &c);
		void resetColor();
	};

	class Board
	{
		Cell* board[9][9];
		Position m_currentSelection;
		bool m_isSelected;
		Cell* nexts[3];
		Score m_score;
		static Board* m_instance;
	public:
		static Board* getInstance();
		~Board();
		void select(int posX, int posY);
		void select(Position p);
		std::vector<Cell*> getCommingColors();
		Cell* getCell(int x, int y);
		void reset();
		bool isNotFill();
		Position getSelection();
		bool isSelected();

		int getBestScore();
		int getPlayerScore();
	protected:
		Board();
		void generateNexts();
		void clearLines(Position p);
		int getNmOfFreeCells();	
		void putNextsToBoard();
		bool hasWay(Position p);
	};
}
#endif
