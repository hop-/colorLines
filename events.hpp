#ifndef _EVENTS_HPP_
#define _EVENTS_HPP_

enum EventTypes {
		EV_SELECT,
		EV_QUIT,
		EV_RESET,
		EV_NOEVENT
};

enum Action{
		AC_NOACTION,
};

struct Event
{
	EventTypes type;
	int x, y;
	Action action;
};

#endif //_EVENTS_HPP_
