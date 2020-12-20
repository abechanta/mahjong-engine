#ifndef MJPAIARRAY_H
#define MJPAIARRAY_H

#include "mjpai.h"
#include <deque>
#include <string>

class MJPAIARRAY : public std::deque<MJPAI>
{
	//
	// property
	//
private:
	bool					m_bSorted;		// ソート済みかどうか

	//
	// method
	//
public:
	MJPAIARRAY();
	MJPAIARRAY(const MJPAI &Other);
	MJPAIARRAY(const MJPAIARRAY &Other);

	MJPAIARRAY	&operator =(const MJPAI &Other);
	MJPAIARRAY	&operator =(const MJPAIARRAY &Other);
	MJPAIARRAY	operator +(const MJPAI &Other) const;
	MJPAIARRAY	operator +(const MJPAIARRAY &Other) const;
	MJPAIARRAY	operator -(const MJPAI &Other) const;
	MJPAIARRAY	operator -(const MJPAIARRAY &Other) const;
	MJPAIARRAY	&operator +=(const MJPAI &Other);
	MJPAIARRAY	&operator +=(const MJPAIARRAY &Other);
	MJPAIARRAY	&operator -=(const MJPAI &Other);
	MJPAIARRAY	&operator -=(const MJPAIARRAY &Other);

	MJPAIARRAY	&operator |=(size_t Other);
	MJPAIARRAY	&operator &=(size_t Other);

	bool	operator ==(const MJPAIARRAY &Other) const;
	bool	operator !=(const MJPAIARRAY &Other) const;
	bool	operator <(const MJPAIARRAY &Other) const;
	bool	operator <=(const MJPAIARRAY &Other) const;
	bool	operator >(const MJPAIARRAY &Other) const;
	bool	operator >=(const MJPAIARRAY &Other) const;

	bool	IsValid() const;
	size_t	GetRed() const;
	std::string	GetString() const;

	void	Sort();
	void	Shade();
};

#endif	// MJPAIARRAY_H
