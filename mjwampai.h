#ifndef MJWAMPAI_H
#define MJWAMPAI_H

#include "mjglobal.h"
#include "mjpai.h"
#include "mjpaiarray.h"

class MJWAMPAI
{
	//
	// property
	//
private:
	MJPAIARRAY				m_Wampai;			// 王牌
	size_t					m_RinshanLeft;		// リンシャン残り回数
	size_t					m_DoraOffset;		// 次のドラ開示位置

	//
	// method
	//
public:
	MJWAMPAI();
	~MJWAMPAI();

	void	Reset(const MJPAIARRAY &PaiSet, size_t RinshanCount);
	size_t	GetLeft() const;

	MJPAI	OpenDora();
	MJPAIARRAY	OpenUraDora();
	MJPAI	PopRinshan();
	void	PushTail(const MJPAI &Pai);
};

#endif	// MJWAMPAI_H
