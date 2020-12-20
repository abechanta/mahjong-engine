#ifndef MJYAMA_H
#define MJYAMA_H

#include "mjglobal.h"
#include "mjpai.h"
#include "mjpaiarray.h"

class MJYAMA
{
	//
	// property
	//
private:
	MJPAIARRAY				m_Yama;			// 牌山

	//
	// method
	//
public:
	MJYAMA();
	~MJYAMA();

	MJPAIARRAY	Reset(const MJPAIARRAY &PaiSet, size_t WampaiCount);
	bool	IsEmpty() const;
	size_t	GetLeft() const;

	MJPAI	PopTumo();
	MJPAI	PopTail();
};

#endif	// MJYAMA_H
