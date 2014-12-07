#include "gui.hpp"
#include "test.hpp"

Gui::Gui():
		//m_cellSize(100),
		//m_screenWidth(9 * m_cellSize),
		//m_screenHeight(9 * m_cellSize),
		m_brd(CLBoard::getInstance()),
		m_window(NULL),
		m_surface(NULL),
		m_renderer(NULL),
		m_txtr(NULL)
{}

bool Gui::preInit()
{
	m_surface = IMG_Load("./textures/textures.png");
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
		m_screenHeight = 9 * m_cellSize + 50;
	} else {
		return false;
	}
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
	SDL_FreeSurface(m_surface);
	m_surface = NULL;
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
	SDL_Quit();
}

void Gui::render()
{
	SDL_RenderClear(m_renderer);
	drawBoard();
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
	while (gameEvent == NULL) {
		if(SDL_WaitEvent(&sdlEvent)) {
			switch (sdlEvent.type) {
			  case SDL_MOUSEBUTTONDOWN:
				if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
					gameEvent = new Event;
					gameEvent->type = EV_SELECT;
					// TMP
					gameEvent->x = sdlEvent.button.x / (m_screenWidth / BOARD_SIZE);
					gameEvent->y = sdlEvent.button.y / (m_screenWidth / BOARD_SIZE);
				}
			  break;
			  case SDL_QUIT:
				gameEvent = new Event;
				gameEvent->type = EV_QUIT;
			  break;
			  case SDL_KEYUP:
			  	switch (sdlEvent.key.keysym.scancode) {
				  case SDL_SCANCODE_F2:
				  	gameEvent = new Event;
					gameEvent->type = EV_RESET;
				  default:
				  break;
				}
			  break;
			}
		}
	}
	return gameEvent;
}
