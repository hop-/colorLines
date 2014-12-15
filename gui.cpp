#include "gui.hpp"
#include <iostream>
#include "test.hpp"

using namespace ColorLines;
using namespace SDL_ColorLines;

Gui::Gui():
		m_lockGameEvents(false),
		m_brd(Board::getInstance()),
		m_currentSelection(m_brd->getSelection()),
		m_window(NULL),
		m_surface(NULL),
		m_renderer(NULL),
		m_txtr(NULL),
		m_fontScore(NULL),
		m_fontGameOver(NULL),
		m_playerScore(-1),
		m_bestScore(-1)
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
	int fontSize = 2 * m_cellSize;
	SDL_Surface* tmp_surface = NULL;
	SDL_Color testColor = {255, 0, 0};
	do {
		TTF_CloseFont(m_fontScore);
		m_fontScore = TTF_OpenFont("./resources/font.ttf",
	        	              fontSize);
		if (m_fontScore == NULL) {
			return false;
		}
		tmp_surface = TTF_RenderText_Solid(m_fontScore, "0", testColor);
		if (--fontSize < 1) {
			return false;
		}
		if (NULL == m_fontGameOver && m_screenWidth >= 11 * tmp_surface->w) {
			m_fontGameOver = TTF_OpenFont("./resources/font.ttf",
						      fontSize);
			if (m_fontGameOver == NULL) {
				return false;
			}
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
	TTF_Quit();
	SDL_Quit();
}

void Gui::render()
{
	if (m_brd->isChange() || m_lockGameEvents) {
		SDL_RenderClear(m_renderer);
		drawBoard();
		drawScores();
		drawCommings();
		if (!m_brd->isNotFill())
			drawGameOver();
		m_brd->changesCatched();
	}
	SDL_RenderPresent(m_renderer);
}

void Gui::drawBoard()
{
	m_rectDst.w = m_cellSize;
	m_rectDst.h = m_cellSize;
	for (int x = 0; x < 9; ++x) {
		for (int y = 0; y < 9; ++y) {
			m_rectDst.x = x * m_cellSize;
			m_rectDst.y = y * m_cellSize;
			drawCell(m_brd->getCell(x, y));
		}
	}
	drawSelection();
}

void Gui::drawSelection()
{
	if (!m_brd->isSelected()) {
		return;
	}
	Position p = m_brd->getSelection();
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
	if (m_playerScore != m_brd->getPlayerScore()) {
		m_playerScore +=2; //m_brd->getPlayerScore();
		if (m_playerScore) {
			m_bestScore = m_brd->getBestScore();
		}
		if (!m_bestScore || m_playerScore > m_bestScore) {
			m_playerScoreColor.r = 0;
			m_playerScoreColor.g = 255;
			m_playerScoreColor.b = 0; 
			
			m_bestScoreColor.r = 100;
			m_bestScoreColor.g = 100;
			m_bestScoreColor.b = 100;
		} else {
			int halfBest =  m_bestScore / 2;
			if (m_playerScore < halfBest) {
				m_playerScoreColor.r = 255;
				m_playerScoreColor.g = m_playerScore
				                       *
						       255.0 / halfBest;
				m_playerScoreColor.b = 0; 
			} else {
				m_playerScoreColor.r = 255 
				                       -
						       (m_playerScore - halfBest)
						       *
						       255.0 / halfBest;
				m_playerScoreColor.g = 255;
				m_playerScoreColor.b = 0; 
			}
			m_bestScoreColor.r = 0;
			m_bestScoreColor.g = 255;
			m_bestScoreColor.b = 0;
		}
	}
	drawScore("Your: ",
		  m_playerScore,
		  m_playerScoreColor,
		  0,
		  9 * m_cellSize,
		  false);
	drawScore("Best: ",
		  m_bestScore,
		  m_bestScoreColor,
		  9 * m_cellSize,
		  9 * m_cellSize,
		  true);
}

void Gui::drawScore(std::string pre, int score, SDL_Color color ,int x, int y, bool fromRight)
{
	std::stringstream ss;
	ss << score;
	std::string scoreTxt = pre + ss.str();
	AlignWidth w;
	if (fromRight) {
		w = SDL_ColorLines::w_right;
	} else {
		w = SDL_ColorLines::w_left;
	}
	drawText(scoreTxt, color,
		 m_fontScore,
		 x, y,
		 SDL_ColorLines::h_top,
		 w);
}

void Gui::drawText(std::string text, SDL_Color color, TTF_Font* font, int x, int y, AlignHeight h, AlignWidth w) 
{
	m_surface = TTF_RenderText_Solid(font,
	                                 text.c_str(),
					 color);
	SDL_Texture* tmptxtr = SDL_CreateTextureFromSurface(m_renderer,
							    m_surface);
	SDL_Rect tmp_rect;
	switch (w) {
	  case SDL_ColorLines::w_left:
		tmp_rect.x = x;
	  break;
	  case SDL_ColorLines::w_center:
		tmp_rect.x = x - m_surface->w / 2;
	  break;
	  case SDL_ColorLines::w_right:
		tmp_rect.x = x - m_surface->w;
	  break;
	}
	switch (h) {
	  case SDL_ColorLines::h_top:
		tmp_rect.y = y;
	  break;
	  case SDL_ColorLines::h_center:
		tmp_rect.y = y - m_surface->h / 2;
	  break;
	  case SDL_ColorLines::h_bottom:
		tmp_rect.y = y - m_surface->w;
	  break;
	}
	tmp_rect.w = m_surface->w;
	tmp_rect.h = m_surface->h;
	SDL_RenderCopy(m_renderer, tmptxtr, NULL, &tmp_rect);
	SDL_DestroyTexture(tmptxtr);
	tmptxtr = NULL;
	SDL_FreeSurface(m_surface);
	m_surface = NULL;
}

void Gui::drawCommings()
{
	std::vector<Cell*> tmpCommings = m_brd->getCommingColors();
	m_rectDst.w = m_screenHeight - 9 * m_cellSize;
	m_rectDst.h = m_rectDst.w;
	m_rectDst.y = 9 * m_cellSize;
	int dx = 0 - m_rectDst.w;
	for (int i = 0; i < static_cast<int>(tmpCommings.size()); ++i, dx += m_rectDst.w) {
			m_rectDst.x = m_screenWidth / 2 - m_rectDst.w / 2 -dx;
			drawCell(tmpCommings[i]);
	}
}

void Gui::drawCell(Cell* cell)
{
	m_rectSrc.x = 0;
	m_rectSrc.y = 0;
	SDL_RenderCopy(m_renderer,
	               m_txtr,
		       &m_rectSrc,
		       &m_rectDst);
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

void Gui::drawGameOver()
{
	drawText("Game Over",
		 m_playerScoreColor,
		 m_fontGameOver,
		 m_screenWidth / 2,
		 m_screenHeight / 2,
		 SDL_ColorLines::h_center,
		 SDL_ColorLines::w_center);
	m_lockGameEvents = true;
}

Event* Gui::getEvent()
{
	Event* gameEvent = NULL;
	SDL_Event sdlEvent;
	if(SDL_WaitEvent(&sdlEvent)) {
		gameEvent = new Event;
		if (!m_lockGameEvents) {
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
		} else {
			if (sdlEvent.type == SDL_MOUSEBUTTONDOWN ) {
				gameEvent->type = EV_RESET;
				m_lockGameEvents = false;
			}
		}
	}
	return gameEvent;
}
