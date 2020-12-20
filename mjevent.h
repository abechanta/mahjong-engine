#ifndef MJEVENT_H
#define MJEVENT_H

#include "mjglobal.h"
#include "mjpai.h"
#include "mjpaiarray.h"

class MJEVENT
{
	//
	// definition
	//
public:
	enum MJEVENTTYPE {
		NONE,
		ACK,				// 

		GAME_START,			// ゲーム開始
		GAME_END,			// ゲーム終了
		ROUND_START,		// ラウンド開始
		ROUND_END,			// ラウンド終了

		DORA_OPEN,			// ドラ開示（MJPAIARRAY）
		HAIPAI4,			// 配牌４枚（誰が、MJPAIARRAY）
		HAIPAI1,			// 配牌１枚（誰が、MJPAI）
		HAIPAI_END,			// 配牌!!!

		TUMO,				// 牌山から１枚ツモる（誰が、MJPAI）
		RINSHAN,			// 王牌から１枚ツモる（誰が、MJPAI）
		SUTE,				// 手牌から１枚切る（誰が、MJPAI、リーチフラグ、ツモ切りフラグ、テンパイフラグ）
		ANKAN,				// 暗カン（誰が、MJPAIARRAY）
		KAKAN,				// 加カン（誰が、MJPAI）

		CHI,				// チー（誰が、誰の、MJPAI、MJPAIARRAY、タイム）
		PON,				// ポン（誰が、誰の、MJPAI、MJPAIARRAY、タイム）
		KAN,				// 明カン（誰が、誰の、MJPAI、MJPAIARRAY、タイム）

		AGARI,				// ツモアガリ、ロンアガリ（誰が、誰の、MJPAI）
		NAGARE,				// 流局、途中流局（九種公九倒牌、４人リーチ、四風子連打、四開槓、トリプルロン）
	};

	//
	// property
	//
public:
	size_t					m_Action;		// イベント・プレイヤイベント
	size_t					m_ActionMask;	// 
	size_t					m_Pid;			// 自分の PID
	size_t					m_OpponentPid;	// 相手の PID
//	size_t					m_Flag;			// 
//	size_t					m_Time;			// プレイヤイベントの発生時間
	MJPAI					m_Pai;			// 
	MJPAIARRAY				m_PaiArray;		// 

	//
	// method
	//
public:
	MJEVENT(size_t Action = MJEVENT::NONE, size_t ActionMask = 0, size_t Pid = MJ::NOBODY, size_t OpponentPid = MJ::NOBODY, const MJPAI &Pai = MJPAI(), const MJPAIARRAY &PaiArray = MJPAIARRAY());
	MJEVENT	&operator |=(size_t Mask);
	MJEVENT	&operator &=(size_t Mask);
	MJEVENT	&Shade();
};



class MJEVENT_ACK : public MJEVENT
{
public:
	MJEVENT_ACK(size_t Action)
		: MJEVENT(Action, (1 << MJEVENT::ACK))
	{}
};

#endif	// MJEVENT_H
