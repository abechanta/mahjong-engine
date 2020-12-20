#ifndef MJDORA_H
#define MJDORA_H

#include "mjglobal.h"
#include "mjpai.h"
#include "mjpaiarray.h"

class MJDORA
{
	//
	// property
	//
private:
	MJPAIARRAY				m_Disp;			// ドラ表示牌
	MJPAIARRAY				m_Dora;			// ドラ

	//
	// method
	//
public:
	MJDORA();
	~MJDORA();

	void	Reset();
	MJPAIARRAY	GetDora() const;
	bool	IsDora(const MJPAI &Pai) const;
	size_t	CountDora(const MJPAIARRAY &Pai) const;

	void	Add(const MJPAI &Pai);
	void	Add(const MJPAIARRAY &Pai);
};

#endif	// MJDORA_H
