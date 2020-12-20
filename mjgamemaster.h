#ifndef MJGAMEMASTER_H
#define MJGAMEMASTER_H

#include "mjglobal.h"
#include "mjpai.h"
#include "mjpaiarray.h"
#include "mjplayer.h"
#include "mjround.h"
#include "mjroundcount.h"
#include "mjrule.h"
#include "mjturn.h"
#include <map>
#include <vector>
class MJINTERFACE;

class MJGAMEMASTER
{
	//
	// definition
	//
private:
	class STATEARRAY
	{
	private:
		std::vector<ssize_t>	m_StateArray;	// 状態スタック

	public:
		ssize_t	GetState(size_t Level)
		{
			if (m_StateArray.size() < Level + 1) {
				m_StateArray.resize(Level + 1, 0);
			}
			return m_StateArray.at(Level);
		}

		void	SetState(size_t Level, ssize_t State)
		{
			m_StateArray.resize(Level + 2);
			m_StateArray[Level + 0] = State;
			m_StateArray[Level + 1] = 0;
		}
	};

	class STATEREF
	{
	private:
		size_t				m_Level;	// 参照レベル
		STATEARRAY			*m_pRef;	// 状態スタックへの参照

	public:
		STATEREF(STATEARRAY *pStateArray)
			: m_Level(0), m_pRef(pStateArray)
		{
		}

		STATEREF(STATEREF &Other)
			: m_Level(Other.m_Level), m_pRef(Other.m_pRef)
		{
		}

		STATEREF operator=(const STATEREF &Other)
		{
			m_Level = Other.m_Level;
			m_pRef = Other.m_pRef;
			return *this;
		}

		STATEREF operator+(size_t Other)
		{
			STATEREF value(*this);
			value.m_Level += Other;
			return value;
		}

		ssize_t	GetState()
		{
			return m_pRef->GetState(m_Level);
		}

		void	SetState(ssize_t State)
		{
			m_pRef->SetState(m_Level, State);
		}
	};

protected:
	enum RET {
		RET_ERR		= MJ::RET_ERR,
		RET_BUSY	= MJ::RET_BUSY,
		RET_DONE	= MJ::RET_DONE,
	};

	//
	// property
	//
private:
	STATEARRAY				m_StateArray;	// 状態変数

	// 変更しないもの
	MJRULE					m_Rule;			// ルール
	size_t					m_RandomSeed;	// 乱数シード
	std::vector<MJPLAYER>	m_Player;		// プレイヤー（登録順）
	MJROUNDCOUNT			m_RoundCount;	// 場、局、本場
	ssize_t					m_PoolPoint;	// 持ち越した供託点棒
	MJROUND					m_Round;		// ラウンド

	//
	// method
	//
public:
	MJGAMEMASTER(const MJRULE &Rule);
	virtual	~MJGAMEMASTER();

	ssize_t			Start(const std::vector<MJINTERFACE *> &Interfaces, size_t RandomSeed);
	ssize_t			Server();

private:
	virtual	ssize_t			GameState_Game(STATEREF State);
	virtual	ssize_t			GameState_Round(STATEREF State);
	virtual	ssize_t			GameState_Haipai(STATEREF State);
	virtual	ssize_t			GameState_Haipai_x4(STATEREF State);
	virtual	ssize_t			GameState_Haipai_x1(STATEREF State);
	virtual	ssize_t			GameState_Dora(STATEREF State);
	virtual	ssize_t			GameState_Turn(STATEREF State);
	virtual	ssize_t			GameState_Select(STATEREF State);
	virtual	ssize_t			GameState_Tumo(STATEREF State);
	virtual	ssize_t			GameState_Naki(STATEREF State);
	virtual	ssize_t			GameState_Kan(STATEREF State);

protected:
	virtual	void			GameStart();
	virtual	void			GameEnd();
	virtual	bool			GameIsFinished();
	virtual	void			RoundStart();
	virtual	void			RoundEnd();
	virtual	void			RoundWin();
	virtual	void			RoundRyukyoku();
	virtual	void			Haipai4(size_t TurnPid);
	virtual	void			Haipai1(size_t TurnPid);
	virtual	void			HaipaiEnd();
	virtual	void			OpenDora();
	virtual	void			PlayerTumo(size_t TurnPid);
	virtual	void			PlayerNaki(size_t TurnPid);
	virtual	void			PlayerAnkan(size_t TurnPid, const MJPAIARRAY &PaiArray);
	virtual	void			PlayerKakan(size_t TurnPid, const MJPAI &Pai);
	virtual	void			PlayerRinshan(size_t TurnPid);
	virtual	void			PlayerSute(size_t TurnPid, const MJPAI &Pai /*, size_t Flag*/);
	virtual	size_t			FilterActionMask(const MJHAND &Hand, const MJPAI &Sute, size_t Mask);
	virtual	void			CheckOpponentWinner(size_t TurnPid);
	virtual	void			CheckOpponentNaki(size_t TurnPid);

private:
	ssize_t			GetPno(ssize_t Pid) const;
	ssize_t			GetPid(ssize_t Order) const;
	MJPLAYER		&GetPlayer(size_t Pid);
	MJEVENT			GetEvent(const MJINTERFACE *pInterface);
	MJEVENT			GetEventPno(size_t Pno);
	MJEVENT			GetEventPid(size_t Pid);
	void			SetEventPno(size_t Pno, const MJEVENT &Event);
	void			SetEventAll(const MJEVENT &Event);
	void			SetEventAll(const MJEVENT &Event, size_t Pid, bool bShade, size_t Mask);
	void			SetEventAll(const MJEVENT &Event, size_t Pid, bool bShade, const std::vector<size_t> &Mask);
};

#endif	// MJGAMEMASTER_H
