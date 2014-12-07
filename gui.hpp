#ifndef _GUI_HPP_
#define _GUI_HPP_

#include "baseGui.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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

	SDL_Window* m_window;

	SDL_Surface* m_surface;

	SDL_Renderer* m_renderer;

	SDL_Rect m_rectSrc;
	SDL_Rect m_rectDst;
	
	SDL_Texture* m_txtr;
	SDL_Texture* m_txtrCell;
	SDL_Texture* m_txtrRedBall;
	SDL_Texture* m_txtrGreenBall;
	SDL_Texture* m_txtrBlueBall;
	SDL_Texture* m_txtrCyanBall;
	SDL_Texture* m_txtrYellowBall;
	SDL_Texture* m_txtrOrangeBall;
	SDL_Texture* m_txtrMagentaBall;
public:
	Gui();
	int init();
	void destroy();
	void render();
	Event* getEvent();
	~Gui(){};
private:
	bool preInit();
	void drawBoard();	
	void drawCellColor(int x, int y);
};

#endif //_GUI_HPP_
