#ifndef _BASEGUI_HPP_
#define _BASEGUI_HPP_

#include "events.hpp"

class BaseGui
{
public:
	virtual int init() = 0;
	virtual void destroy() = 0;
	virtual void render() = 0;
	virtual Event* getEvent() = 0;
	virtual ~BaseGui(){};
};



#endif //_BASEGUI_HPP_
