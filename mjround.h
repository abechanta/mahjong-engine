#ifndef MJROUND_H
#define MJROUND_H

#include "mjglobal.h"
#include "mjdora.h"
#include "mjkanplayer.h"
#include "mjrule.h"
#include "mjturn.h"
#include "mjwampai.h"
#include "mjwinplayer.h"
#include "mjyama.h"
#include <map>
#include <vector>
class MJPLAYER;

class MJROUND
{
	friend class MJGAMEMASTER;

	//
	// property
	//
private:
	MJRULE					m_Rule;			// ルール
	size_t					m_Wind;			// 場風
	ssize_t					m_PoolPoint;	// 持ち越した供託点棒
	bool					m_bShibari;		// t=縛りアリ
	MJYAMA					m_Yama;			// 牌山
	MJWAMPAI				m_Wampai;		// 王牌
	MJDORA					m_Dora;			// ドラ
	MJKANPLAYER				m_KanPlayer;	// カンをしたプレイヤー
	MJWINPLAYER				m_WinPlayer;	// 勝者
	MJTURN					m_Turn;			// 手番
	size_t					m_KanOpponentPid;	// 明カンをされたプレイヤー
	bool					m_bRinshanNow;	// t=カン直後
	bool					m_bTochuRyukyoku;	// t=途中流局
//	size_t					m_DoraPending;	// カンドラ開示待ち

	//
	// method
	//
public:
	MJROUND();
	virtual	~MJROUND();

	void	Reset(const MJRULE &Rule, size_t Wind, bool bShibari);	// FIXME: ほかにも必要な情報があるのでは？
};

#endif	// MJROUND_H
