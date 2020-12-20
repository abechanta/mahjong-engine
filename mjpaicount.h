#ifndef MJPAICOUNT_H
#define MJPAICOUNT_H

#include "mjpai.h"
#include "mjpaiarray.h"
#include <string>
#include <vector>

class MJPAICOUNT : public std::vector<size_t>
{
	//
	// method
	//
public:
	MJPAICOUNT();
	MJPAICOUNT(const MJPAI &Other);
	MJPAICOUNT(const MJPAIARRAY &Other);
	MJPAICOUNT(const MJPAICOUNT &Other);

	MJPAICOUNT	&operator =(const MJPAI &Other);
	MJPAICOUNT	&operator =(const MJPAIARRAY &Other);
	MJPAICOUNT	&operator =(const MJPAICOUNT &Other);
	MJPAICOUNT	operator +(const MJPAI &Other) const;
	MJPAICOUNT	operator +(const MJPAIARRAY &Other) const;
	MJPAICOUNT	operator +(const MJPAICOUNT &Other) const;
	MJPAICOUNT	operator -(const MJPAI &Other) const;
	MJPAICOUNT	operator -(const MJPAIARRAY &Other) const;
	MJPAICOUNT	operator -(const MJPAICOUNT &Other) const;
	MJPAICOUNT	&operator +=(const MJPAI &Other);
	MJPAICOUNT	&operator +=(const MJPAIARRAY &Other);
	MJPAICOUNT	&operator +=(const MJPAICOUNT &Other);
	MJPAICOUNT	&operator -=(const MJPAI &Other);
	MJPAICOUNT	&operator -=(const MJPAIARRAY &Other);
	MJPAICOUNT	&operator -=(const MJPAICOUNT &Other);

	bool	operator ==(const MJPAICOUNT &Other) const;
	bool	operator !=(const MJPAICOUNT &Other) const;

	size_t	operator [](const MJPAI &Other) const;

	bool	Include(const MJPAICOUNT &Other) const;
	MJPAICOUNT	&Union(const MJPAICOUNT &Other);
	MJPAICOUNT	&Intersect(const MJPAICOUNT &Other);
	std::string	GetString() const;
};

#endif	// MJPAICOUNT_H
