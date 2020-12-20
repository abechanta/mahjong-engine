#ifndef MJKANPLAYER_H
#define MJKANPLAYER_H

#include "mjglobal.h"
#include <vector>

class MJKANPLAYER
{
	//
	// property
	//
private:
	size_t					m_KanCount;			// カン回数の上限
	std::vector<size_t>		m_KanPid;			// カンしたプレイヤー
	bool					m_bSukantsuAvailable;	// 四カンツ可能かどうか

	//
	// method
	//
public:
	MJKANPLAYER();
	~MJKANPLAYER();

	void	Reset(size_t KanCount);
	bool	IsAvailable() const;
	bool	IsAvailableSukantsu() const;

	void	Add(size_t Pid);
};

#endif	// MJKANPLAYER_H
