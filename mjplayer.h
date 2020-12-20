#ifndef MJPLAYER_H
#define MJPLAYER_H

#include "mjglobal.h"
#include "mjevent.h"
#include "mjhand.h"
#include "mjkawa.h"
#include "mjpaiarray.h"
#include "mjplug.h"
#include "mjpoints.h"
#include "mjrule.h"
#include <string>
#include <vector>

class MJPLAYER
{
	friend class MJGAMEMASTER;

	//
	// property
	//
private:
public:	// FIXME: ホントは private にすべき
	std::string				m_Name;			// 表示名
	MJPLUG<MJEVENT>			m_Plug;			// 通信相手

	MJRULE					m_Rule;			// ルール
	MJPOINTS				m_Points;		// プレイヤー点数（登録順）
	MJHAND					m_Hand;			// 手牌（[1～] は鳴き面子）
	MJKAWA					m_Kawa;			// 河

	size_t					m_Pno;			// プレイヤー番号
	size_t					m_Pid;			// 自家
	bool					m_bOya;			// t=親
	ssize_t					m_Shanten;		// シャンテン数（-1=アガリ / 0=テンパイ / 1=一向聴）
	bool					m_bFuriten;		// t=フリテン
	bool					m_bFuritenNow;	// t=フリテン同順
	bool					m_bReach;		// t=リーチ
	bool					m_bReachNow;	// t=リーチ一発圏内
	bool					m_bAgari;		// t=アガリ（チョンボも含む）

	//
	// method
	//
public:
	MJPLAYER(const std::string &Name);
	~MJPLAYER();

	void	Reset(const MJRULE &Rule, size_t Pno);
	void	ResetRound(size_t Pid);
};

#endif	// MJPLAYER_H
