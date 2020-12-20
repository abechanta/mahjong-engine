#ifndef MJWINPLAYER_H
#define MJWINPLAYER_H

#include "mjglobal.h"
#include <vector>

class MJWINPLAYER
{
	//
	// definition
	//
private:
	typedef struct WINNER
	{
		size_t					m_Pid;			// 勝者
		size_t					m_OpponentPid;	// 敗者

		WINNER(size_t Pid, size_t OpponentPid)
			: m_Pid(Pid),
			m_OpponentPid(OpponentPid)
		{
		} /* end of WINNER::WINNER */

	} WINNER;

	//
	// property
	//
private:
	std::vector<WINNER>		m_WinPid;		// 勝者

	//
	// method
	//
public:
	MJWINPLAYER();
	~MJWINPLAYER();

	void	Reset();
	size_t	GetWinnerCount() const;

	void	Add(size_t Pid, size_t OpponentPid);
};

#endif	// MJWINPLAYER_H
