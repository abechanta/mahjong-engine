#ifndef MJROUNDCOUNT_H
#define MJROUNDCOUNT_H

#include "mjglobal.h"
#include "mjrule.h"
#include <vector>

class MJROUNDCOUNT
{
	//
	// property
	//
private:
	MJRULE					m_Rule;			// ルール
	std::vector<size_t>		m_Order;		// プレイヤー席順（Pid → Pno）
	size_t					m_RoundNo;		// 場、局
	size_t					m_HombaNo;		// 本場
	bool					m_bFinished;	// 終了かどうか

	//
	// method
	//
public:
	MJROUNDCOUNT();
	~MJROUNDCOUNT();

	void	Reset(const MJRULE &Rule);
	void	GetPlayerOrder(std::vector<size_t> &Order) const;
	size_t	GetPlayer(size_t Pid) const;
	size_t	GetRoundNo() const;
	size_t	GetHombaNo() const;
	bool	IsFinished() const;

	bool	GoNext(bool bAnyoneWon, bool bTonWon, bool bTonTempai, bool bTochu);
	bool	IsLastRound() const;
};

#endif	// MJROUNDCOUNT_H
