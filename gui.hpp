#ifndef _GUI_HPP_
#define _GUI_HPP_

#include "baseGui.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "logic.hpp"
#include "events.hpp"

#include <iostream>

class Gui :
	public BaseGui
{
	int m_cellSize;
	int m_screenWidth;
	int m_screenHeight;

	CLBoard* m_brd;

	CLPosition m_currentSelection;

	SDL_Window* m_window;

	SDL_Surface* m_surface;

	SDL_Renderer* m_renderer;

	SDL_Rect m_rectSrc;
	SDL_Rect m_rectDst;
	
	SDL_Texture* m_txtr;

	TTF_Font* m_font;
public:
	Gui();
	int init();
	void destroy();
	void render();
	Event* getEvent();
	~Gui(){};
private:
	bool preInit();
	bool initTextConfigs();
	void drawBoard();	
	void drawCellColor(int x, int y);
	void drawSelection();
	void drawScores();
};

#endif //_GUI_HPP_
