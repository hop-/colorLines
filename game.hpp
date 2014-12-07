#ifndef _GAME_HPP_
#define _GAME_HPP_

#include "logic.hpp"
#include "baseGui.hpp"
#include "gui.hpp"
#include "events.hpp"

class Game
{
	CLBoard* m_lgc;
	BaseGui* m_gi;
	bool m_running;
public:
	Game();
	~Game();
	void start();
private:
	void mainLoop();
};

#endif //_GAME_HPP_
