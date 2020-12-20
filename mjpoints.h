#ifndef MJPOINTS_H
#define MJPOINTS_H

#include "mjglobal.h"

class MJPOINTS
{
	//
	// property
	//
private:
	ssize_t					m_Point;		// 点数
	ssize_t					m_Pool;			// 供託の点数
	ssize_t					m_Chip;			// チップ

public:
	MJPOINTS();
	~MJPOINTS();
	void	Reset(ssize_t Pts);
	ssize_t	GetPoint() const;
	ssize_t	GetChip() const;

	void	AddPool(ssize_t Pts);
	void	RefundPool();
	ssize_t	MovePool();
};

#endif	// MJPOINTS_H
