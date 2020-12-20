#ifndef MJKAWA_H
#define MJKAWA_H

#include "mjglobal.h"
#include "mjpai.h"
#include "mjpaiarray.h"

class MJKAWA
{
	//
	// property
	//
private:
	MJPAIARRAY				m_Kawa;			// 河

	//
	// method
	//
public:
	MJKAWA();
	~MJKAWA();

	void	Reset();
	bool	IsEmpty() const;

	void	PushSute(const MJPAI &Sute);
	void	HideTail();
};

#endif	// MJKAWA_H
