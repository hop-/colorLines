#include "gui.hpp"
#include <iostream>
#include "test.hpp"

Gui::Gui():
		m_brd(CLBoard::getInstance()),
		m_currentSelection(m_brd->getSelection()),
		m_window(NULL),
		m_surface(NULL),
		m_renderer(NULL),
		m_txtr(NULL)
		//m_font(TTF_OpenFont("./textures/Capture_it.ttf", 24))
		//m_textColor({255, 255, 255})
{}

bool Gui::preInit()
{
	m_surface = IMG_Load("./resources/texture.png");
	if (m_surface != NULL) {
		if (m_surface->w / m_surface->h  != 2 ||
		    m_surface->w % 4 != 0 ||
		    m_surface->h % 2 != 0) {
			return false;
		}
		m_cellSize = m_surface->w / 4;
		m_rectSrc.w = m_cellSize;
		m_rectSrc.h = m_cellSize;
		m_rectDst.w = m_cellSize;
		m_rectDst.h = m_cellSize;
		m_screenWidth = 9 * m_cellSize;
		m_screenHeight = 9 * m_cellSize;
	} else {
		return false;
	}
	return true;
}

bool Gui::initTextConfigs()
{
	TTF_Init();
	int fontSize = m_cellSize;
	SDL_Surface* tmp_surface = NULL;
	SDL_Color testColor = {255, 0, 0};
	do {
		m_font = TTF_OpenFont("./resources/font.ttf",
	        	              fontSize);
		if (m_font == NULL) {
			return false;
		}
		tmp_surface = TTF_RenderText_Solid(m_font, "0", testColor);
		if (--fontSize < 1) {
			return false;
		}
	} while (m_screenWidth < tmp_surface->w * 24 + 3 * tmp_surface->h);
	m_screenHeight += tmp_surface->h;
	SDL_FreeSurface(tmp_surface);
	tmp_surface = NULL;
	return true;
}

int Gui::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return -1;
	}
	if (!preInit()) {
		return -1;
	}
	if (!initTextConfigs()){
		return -1;
	}
	m_window = SDL_CreateWindow("Interesting Game :)",
				    SDL_WINDOWPOS_UNDEFINED,
				    SDL_WINDOWPOS_UNDEFINED,
				    m_screenWidth,
				    m_screenHeight,
				    SDL_WINDOW_SHOWN);
	if (m_window == NULL) {
			return -1;
	}
	m_renderer = SDL_CreateRenderer(m_window,
				        -1,
					SDL_RENDERER_ACCELERATED);
	if (m_renderer == NULL) {
		return -1;
	}
	m_txtr = SDL_CreateTextureFromSurface(m_renderer, m_surface);
	if (m_txtr == NULL) {
		return -1;
	}
	render();
	return 0;
}

void Gui::destroy()
{
	SDL_DestroyRenderer(m_renderer);
	m_renderer = NULL;
	SDL_DestroyWindow(m_window);
	m_window = NULL;
	SDL_DestroyTexture(m_txtr);
	m_txtr = NULL;
	SDL_FreeSurface(m_surface);
	m_surface = NULL;
	SDL_Quit();
}

void Gui::render()
{
	SDL_RenderClear(m_renderer);
	drawBoard();
	drawScores();
	SDL_RenderPresent(m_renderer);
}

void Gui::drawBoard()
{
	for (int x = 0; x < 9; ++x) {
		for (int y = 0; y < 9; ++y) {
			m_rectSrc.x = 0;
			m_rectSrc.y = 0;
			m_rectDst.x = x * m_cellSize;
			m_rectDst.y = y * m_cellSize;
			SDL_RenderCopy(m_renderer,
			               m_txtr,
				       &m_rectSrc,
				       &m_rectDst);
			drawCellColor(x, y);
		}
	}
	drawSelection();
}

void Gui::drawSelection()
{
	if (!m_brd->isSelected()) {
		return;
	}
	CLPosition p = m_brd->getSelection();
	SDL_Rect rect;
	rect.x = m_cellSize * p.x;
	rect.y = m_cellSize * p.y;
	rect.w = m_cellSize;
	rect.h = m_cellSize;
	SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
	for (int i = 0; i < m_cellSize * 4 / 100; ++i) {
		SDL_RenderDrawRect(m_renderer, &rect);
		rect.x++;
		rect.y++;
		rect.w-=2;
		rect.h-=2;
	}
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
	m_currentSelection = p;
}

void Gui::drawScores()
{
	SDL_Color yourColor = {255, 0, 0};
	m_surface = TTF_RenderText_Solid(m_font, "Your: 230", yourColor);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(m_renderer, m_surface);
	SDL_Rect tmp_rect;
	tmp_rect.y = 9 * m_cellSize;
	tmp_rect.x = 0;
	tmp_rect.w = m_surface->w;
	tmp_rect.h = m_surface->h;
	SDL_RenderCopy(m_renderer, Message, NULL, &tmp_rect);

	SDL_Color bestColor = {0, 255, 0};
	m_surface = TTF_RenderText_Solid(m_font, "Best: 1230", bestColor);
	Message = SDL_CreateTextureFromSurface(m_renderer, m_surface);
	tmp_rect.x = 9 * m_cellSize - m_surface->w;
	tmp_rect.w = m_surface->w;
	tmp_rect.h = m_surface->h;
	SDL_RenderCopy(m_renderer, Message, NULL, &tmp_rect);
}

void Gui::drawCellColor(int x, int y)
{
	CLCell* cell = m_brd->getCell(x, y);
	switch (cell->getColor()) {
	  case COLOR1:
		m_rectSrc.x = m_cellSize;
		m_rectSrc.y = 0;
	  break;
	  case COLOR2:
		m_rectSrc.x = 2 * m_cellSize;
		m_rectSrc.y = 0;
	  break;
	  case COLOR3:
		m_rectSrc.x = 3 * m_cellSize;
		m_rectSrc.y = 0;
	  break;
	  case COLOR4:
		m_rectSrc.x = 0;
		m_rectSrc.y = m_cellSize;
	  break;
	  case COLOR5:
		m_rectSrc.x = m_cellSize;
		m_rectSrc.y = m_cellSize;
	  break;
	  case COLOR6:
		m_rectSrc.x = 2 * m_cellSize;
		m_rectSrc.y = m_cellSize;
	  break;
	  case COLOR7:
		m_rectSrc.x = 3 * m_cellSize;
		m_rectSrc.y = m_cellSize;
	  default:
	  break;
	}
	if (m_rectSrc.x != 0 || m_rectSrc.y != 0) {
		SDL_RenderCopy(m_renderer,
			       m_txtr,
			       &m_rectSrc,
			       &m_rectDst);
	}
}

Event* Gui::getEvent()
{
	Event* gameEvent = NULL;
	SDL_Event sdlEvent;
	if(SDL_WaitEvent(&sdlEvent)) {
		gameEvent = new Event;
		switch (sdlEvent.type) {
		  case SDL_MOUSEBUTTONDOWN:
			if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
				gameEvent->type = EV_SELECT;
				gameEvent->x = sdlEvent.button.x / (m_screenWidth / BOARD_SIZE);
				gameEvent->y = sdlEvent.button.y / (m_screenWidth / BOARD_SIZE);
			}
		  break;
		  case SDL_QUIT:
			gameEvent->type = EV_QUIT;
		  break;
		  case SDL_KEYUP:
		  	switch (sdlEvent.key.keysym.scancode) {
			  case SDL_SCANCODE_F2:
				gameEvent->type = EV_RESET;
			  default:
			  break;
			}
		  break;
		}
	}
	return gameEvent;
}
