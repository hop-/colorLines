#ifndef _GUI_HPP_
#define _GUI_HPP_

#include "baseGui.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "logic.hpp"
#include "events.hpp"

#include <string>
#include <sstream>
#include <vector>

namespace SDL_ColorLines {

	enum AlignHeight {
		h_top,
		h_center,
		h_bottom
	};

	enum AlignWidth {
		w_left,
		w_center,
		w_right
	};

	class Gui :
		public BaseGui
	{
		int m_cellSize;
		int m_screenWidth;
		int m_screenHeight;
		bool m_lockGameEvents;

		ColorLines::Board* m_brd;

		ColorLines::Position m_currentSelection;

		SDL_Window* m_window;

		SDL_Surface* m_surface;

		SDL_Renderer* m_renderer;

		SDL_Rect m_rectSrc;
		SDL_Rect m_rectDst;

		SDL_Color m_playerScoreColor;
		SDL_Color m_bestScoreColor;
	
		SDL_Texture* m_txtr;

		TTF_Font* m_fontScore;
		TTF_Font* m_fontGameOver;

		int m_playerScore;
		int m_bestScore;
	public:
		Gui();
		int init();
		void destroy();
		void render();
		ColorLines::Event* getEvent();
		~Gui(){};
	private:
		bool preInit();
		bool initTextConfigs();
		void drawBoard();	
		void drawCell(ColorLines::Cell* cell);
		void drawSelection();
		void drawScores();
		void drawScore(std::string pre, int score, SDL_Color color ,int x, int y, bool fromRight);
		void drawText(std::string text, SDL_Color color, TTF_Font* font, int x, int y, AlignHeight h, AlignWidth w);
		void drawCommings();
		void drawGameOver();
	};
}
#endif //_GUI_HPP_
