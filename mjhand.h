#ifndef MJHAND_H
#define MJHAND_H

#include "mjglobal.h"
#include "mjpaiarray.h"
#include <vector>

class MJHAND : public std::vector<MJPAIARRAY>
{
	//
	// property
	//
private:

	//
	// method
	//
public:
	MJHAND();
	~MJHAND();

	void	Reset();
};

#endif	// MJHAND_H
