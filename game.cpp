#include "game.hpp"

using ColorLines::Event;
using ColorLines::EV_SELECT;
using ColorLines::EV_RESET;
using ColorLines::EV_QUIT;
using ColorLines::EV_NOEVENT;

Game::Game() :
		m_lgc(ColorLines::Board::getInstance()),
		m_gi(new SDL_ColorLines::Gui()),
		m_running(true)
{}

Game::~Game()
{
	delete m_gi;
}

void Game::start()
{
	if (m_gi->init() == 0) {
		mainLoop();
		m_gi->destroy();
	}
}

void Game::mainLoop()
{
	Event* e;
	while (m_running) {
		e = m_gi->getEvent();
		if (e != NULL) {
			switch(e->type){
			  case EV_SELECT:
				m_lgc->select(e->x, e->y);
		        m_gi->render();
			  break;
			  case EV_RESET:
			  	m_lgc->reset();
		        m_gi->render();
			  break;
			  case EV_QUIT:
				m_running = false;
			  default:
			  break;
			}
			delete e;
		}
	}
}
