#ifndef MJTURN_H
#define MJTURN_H

#include "mjglobal.h"
#include "mjevent.h"
#include "mjpai.h"
#include "mjpaiarray.h"

class MJTURN
{
	//
	// property
	//
private:
	size_t					m_PlayerCount;	// プレイヤー数
	size_t					m_TurnNo;		// ターン番号
	size_t					m_TurnPid;		// 手番プレイヤー
	bool					m_bNakiTurn;	// 鳴き成立
	size_t					m_NakiPid;		// 鳴き成立プレイヤー
	size_t					m_NakiOpponentPid;	// 鳴かれたプレイヤー
	size_t					m_NakiAction;	// 鳴き種類
	MJPAI					m_NakiSute;		// 鳴き成立時の鳴き牌
	MJPAIARRAY				m_NakiMentsu;	// 鳴き成立時の鳴き面子

	//
	// method
	//
public:
	MJTURN();
	~MJTURN();

	void	Reset(size_t PlayerCount);
	size_t	GetTurnNo() const;
	size_t	GetTurnPid() const;
	size_t	GetNakiOpponentPid() const;
	size_t	GetNakiAction() const;
	MJPAI	GetNakiSute() const;
	MJPAIARRAY	GetNakiMentsu() const;

	void	ResetNaki();
	void	SetNaki(size_t Pid, size_t OpponentPid, size_t Action, const MJPAI &Sute, const MJPAIARRAY &Mentsu);
	bool	GoNext();
	MJPAIARRAY	GetNakiFuuro() const;

	size_t	GetPidd(size_t Pid, size_t OpponentPid) const;
};

#endif	// MJTURN_H
