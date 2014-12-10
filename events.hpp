#ifndef _EVENTS_HPP_
#define _EVENTS_HPP_

namespace ColorLines {

	enum EventTypes {
			EV_SELECT,
			EV_QUIT,
			EV_RESET,
			EV_NOEVENT
	};

	struct Event
	{
		EventTypes type;
		int x, y;
	};

}
#endif //_EVENTS_HPP_
