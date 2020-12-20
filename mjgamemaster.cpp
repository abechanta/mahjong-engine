#include "mjgamemaster.h"
#include "mjinterface.h"
#include "mjpaicount.h"
#include <algorithm>
using namespace std;

#define CALL_STATE_FUNC(StateFunc)\
		switch (StateFunc(State + 1)) {\
		case RET_BUSY:\
			return RET_BUSY;\
		case RET_DONE:\
			break;\
		default:\
			MJASSERT(false);\
			return RET_ERR;\
		}

//----------------------------------------------------------------------------
MJGAMEMASTER::MJGAMEMASTER(const MJRULE &Rule)
{
	STATEREF State(&m_StateArray);
	State.SetState(0);
	m_Rule = Rule;

} /* end of MJGAMEMASTER::MJGAMEMASTER */



MJGAMEMASTER::~MJGAMEMASTER()
{
} /* end of MJGAMEMASTER::~MJGAMEMASTER */



//----------------------------------------------------------------------------
// ゲームのセットアップ
//----------------------------------------------------------------------------
ssize_t	MJGAMEMASTER::Start(const std::vector<MJINTERFACE *> &Interface, size_t RandomSeed)
{
	STATEREF State(&m_StateArray);

	if (
		(State.GetState() != 0) ||
		(Interface.size() != m_Rule.m_Seq.m_PlayerCount)
	) {
		return RET_ERR;
	}

	// ゲーム開始準備（管理系情報の初期化）
	State.SetState(1);
	m_RandomSeed = RandomSeed;
	m_Player.clear();
	m_Player.reserve(m_Rule.m_Seq.m_PlayerCount);
	for (size_t Pno = 0; Pno < m_Rule.m_Seq.m_PlayerCount; Pno++) {
		m_Player.push_back(MJPLAYER(Interface[Pno]->m_Name));
		m_Player[Pno].m_Plug.Connect(&Interface[Pno]->m_Plug);
		Interface[Pno]->m_pMaster = this;	// FIXME: not needed
		Interface[Pno]->m_pPlayer = &m_Player[Pno];	// FIXME: not needed
	}
	return RET_DONE;

} /* end of MJGAMEMASTER::Start */



ssize_t	MJGAMEMASTER::Server()
{
	for (size_t Pno = 0; Pno < m_Rule.m_Seq.m_PlayerCount; Pno++) {
		if (!m_Player[Pno].m_Plug.IsRecv()) {
			MJBREAK;
			return RET_BUSY;
		}
	}

#ifdef _DEBUG
	ssize_t	err = GameState_Game(STATEREF(&m_StateArray));
	std::cerr
		<< "ROUND:" << m_RoundCount.GetRoundNo() + 1
		<< (m_RoundCount.IsLastRound() ? "*" : " ")
		<< " / "
		<< "HOMBA:" << m_RoundCount.GetHombaNo()
		<< " / "
		<< "TURN:" << m_Round.m_Turn.GetTurnNo() + 1
		<< " / "
		<< "YAMA LEFT:" << m_Round.m_Yama.GetLeft()
		<< (m_Round.m_Yama.IsEmpty() ? "*" : " ")
		<< " / "
		<< "RINSHAN LEFT:" << m_Round.m_Wampai.GetLeft()
		<< " / "
		<< "DORA:";
	MJPAIARRAY	Dora = m_Round.m_Dora.GetDora();
	for (size_t ii = 0; ii < Dora.size(); ii++) {
		std::cerr << ' ' << Dora[ii].GetString();
	}
	std::cerr << std::endl;
	return err;
#else	// _DEBUG
	return GameState_Game(0, m_StateVar);
#endif	// _DEBUG

} /* end of MJGAMEMASTER::Server */



//----------------------------------------------------------------------------
// ゲームシーケンス
//----------------------------------------------------------------------------
ssize_t	MJGAMEMASTER::GameState_Game(STATEREF State)
{
	enum {
		ST_NOTINGAME,
		ST_START,
		ST_ROUND,
		ST_RESULT,
		ST_END,
		ST_DONE,
	};

	switch (State.GetState()) {
	case ST_NOTINGAME:
		// ゲーム開始前
		MJASSERT(false);
		return RET_ERR;

	case ST_START:
		// ゲームの初期化
		GameStart();
		State.SetState(ST_ROUND);
		return RET_BUSY;

	case ST_ROUND:
		// 場、局、本場
		CALL_STATE_FUNC(GameState_Round);

		if (GameIsFinished()) {
			State.SetState(ST_END);
		} else {
			State.SetState(ST_ROUND);
		}
		return RET_BUSY;

	case ST_END:
		// ゲームの終了
		GameEnd();
		State.SetState(ST_DONE);
		return RET_BUSY;

	case ST_DONE:
		// 接続の切断
		;	// FIXME: 処理がない
		State.SetState(ST_NOTINGAME);
		return RET_DONE;

	default:
		MJASSERT(false);
		return RET_ERR;

	} /* endswitch StateVar[Level] */

	MJASSERT(false);
	return RET_ERR;

} /* end of MJGAMEMASTER::GameState_Game */



ssize_t	MJGAMEMASTER::GameState_Round(STATEREF State)
{
	enum {
		ST_START,
		ST_HAIPAI,
		ST_DORA,
		ST_TURN,
		ST_END,
	};

	switch (State.GetState()) {
	case ST_START:
		// ラウンドの初期化
		RoundStart();
		State.SetState(ST_HAIPAI);
		return RET_BUSY;

	case ST_HAIPAI:
		// 配牌
		CALL_STATE_FUNC(GameState_Haipai);
		State.SetState(ST_DORA);
		return RET_BUSY;

	case ST_DORA:
		// ドラ開示
		CALL_STATE_FUNC(GameState_Dora);
		State.SetState(ST_TURN);
		return RET_BUSY;

	case ST_TURN:
		// 手番
		CALL_STATE_FUNC(GameState_Turn);
		State.SetState(ST_END);
		return RET_BUSY;

	case ST_END:
		// ラウンド結果表示
		RoundEnd();
		return RET_DONE;

	default:
		MJASSERT(false);
		return RET_ERR;

	} /* endswitch StateVar[Level] */

	MJASSERT(false);
	return RET_ERR;

} /* end of MJGAMEMASTER::GameState_Round */



ssize_t	MJGAMEMASTER::GameState_Haipai(STATEREF State)
{
	enum {
		ST_BRANCH,
		ST_HAIPAI4,
		ST_HAIPAI1,
	};

	if (State.GetState() == ST_BRANCH) {
		if (GetPlayer(MJ::TON).m_Hand[0].size() >= m_Rule.m_Seq.m_TehaiCount) {
			HaipaiEnd();
			return RET_DONE;
		}

		// 分岐
		State.SetState(
			(m_Rule.m_Seq.m_TehaiCount - GetPlayer(MJ::TON).m_Hand[0].size() >= 4) ? ST_HAIPAI4 : ST_HAIPAI1
		);
	}

	switch (State.GetState()) {
	case ST_HAIPAI4:
		// 4 枚ずつ配牌
		CALL_STATE_FUNC(GameState_Haipai_x4);
		State.SetState(ST_BRANCH);
		return RET_BUSY;

	case ST_HAIPAI1:
		// 1 枚ずつ配牌
		CALL_STATE_FUNC(GameState_Haipai_x1);
		State.SetState(ST_BRANCH);
		return RET_BUSY;

	case ST_BRANCH:
	default:
		MJASSERT(false);
		return RET_ERR;

	} /* endswitch StateVar[Level] */

	MJASSERT(false);
	return RET_ERR;

} /* end of MJGAMEMASTER::GameState_Haipai */



ssize_t	MJGAMEMASTER::GameState_Haipai_x4(STATEREF State)
{
	enum {
		ST_TON,
		ST_NAN,
		ST_SHA,
		ST_PEI,
	};

	switch (State.GetState()) {
	case ST_TON:
		// 東家へ配牌
		Haipai4(MJ::TON);
		if (m_Rule.m_Seq.m_PlayerCount >= 2) {
			State.SetState(ST_NAN);
			return RET_BUSY;
		}
		return RET_DONE;

	case ST_NAN:
		// 南家へ配牌
		Haipai4(MJ::NAN);
		if (m_Rule.m_Seq.m_PlayerCount >= 3) {
			State.SetState(ST_SHA);
			return RET_BUSY;
		}
		return RET_DONE;

	case ST_SHA:
		// 西家へ配牌
		Haipai4(MJ::SHA);
		if (m_Rule.m_Seq.m_PlayerCount >= 4) {
			State.SetState(ST_PEI);
			return RET_BUSY;
		}
		return RET_DONE;

	case ST_PEI:
		// 北家へ配牌
		Haipai4(MJ::PEI);
		return RET_DONE;

	default:
		MJASSERT(false);
		return RET_ERR;

	} /* endswitch StateVar[Level] */

	MJASSERT(false);
	return RET_ERR;

} /* end of MJGAMEMASTER::GameState_Haipai_x4 */



ssize_t	MJGAMEMASTER::GameState_Haipai_x1(STATEREF State)
{
	enum {
		ST_TON,
		ST_NAN,
		ST_SHA,
		ST_PEI,
	};

	switch (State.GetState()) {
	case ST_TON:
		// 東家へ配牌
		Haipai1(MJ::TON);
		if (m_Rule.m_Seq.m_PlayerCount >= 2) {
			State.SetState(ST_NAN);
			return RET_BUSY;
		}
		return RET_DONE;

	case ST_NAN:
		// 南家へ配牌
		Haipai1(MJ::NAN);
		if (m_Rule.m_Seq.m_PlayerCount >= 3) {
			State.SetState(ST_SHA);
			return RET_BUSY;
		}
		return RET_DONE;

	case ST_SHA:
		// 西家へ配牌
		Haipai1(MJ::SHA);
		if (m_Rule.m_Seq.m_PlayerCount >= 4) {
			State.SetState(ST_PEI);
			return RET_BUSY;
		}
		return RET_DONE;

	case ST_PEI:
		// 北家へ配牌
		Haipai1(MJ::PEI);
		return RET_DONE;

	default:
		MJASSERT(false);
		return RET_ERR;

	} /* endswitch StateVar[Level] */

	MJASSERT(false);
	return RET_ERR;

} /* end of MJGAMEMASTER::GameState_Haipai_x1 */



ssize_t	MJGAMEMASTER::GameState_Dora(STATEREF State)
{
	enum {
		ST_DORA1,
		ST_DORA2,
	};

	switch (State.GetState()) {
	case ST_DORA1:
		// 1 枚目のドラ開示
		OpenDora();
		if (m_Rule.m_Dora.m_DoraCount >= 2) {
			State.SetState(ST_DORA2);
			return RET_BUSY;
		}
		return RET_DONE;

	case ST_DORA2:
		// 2 枚目のドラ開示
		OpenDora();
		return RET_DONE;

	default:
		MJASSERT(false);
		return RET_ERR;

	} /* endswitch StateVar[Level] */

	MJASSERT(false);
	return RET_ERR;

} /* end of MJGAMEMASTER::GameState_Dora */



ssize_t	MJGAMEMASTER::GameState_Turn(STATEREF State)
{
	enum {
		ST_SELECT,
		ST_RYUKYOKU,
		ST_END,
	};

	switch (State.GetState()) {
	case ST_SELECT:
		// ツモ番
		CALL_STATE_FUNC(GameState_Select);
		if (m_Round.m_WinPlayer.GetWinnerCount() > 0) {
			// 勝者が出れば終了
			return RET_DONE;
		}
		State.SetState(m_Round.m_Yama.IsEmpty() ? ST_RYUKYOKU : ST_SELECT);
		return RET_BUSY;

	case ST_RYUKYOKU:
		// 流局
		RoundRyukyoku();
		return RET_DONE;

	default:
		MJASSERT(false);
		return RET_ERR;

	} /* endswitch StateVar[Level] */

	MJASSERT(false);
	return RET_ERR;

} /* end of MJGAMEMASTER::GameState_Turn */



ssize_t	MJGAMEMASTER::GameState_Select(STATEREF State)
{
	enum {
		ST_CHECK_NAKI,
		ST_NAKI,
		ST_TUMO,
	};

	switch (State.GetState()) {
	case ST_CHECK_NAKI:
		// 他プレイヤー手番（ロンアガリ、鳴き）

		// まずはロンアガリ
		CheckOpponentWinner(m_Round.m_Turn.GetTurnPid());
		if (m_Round.m_WinPlayer.GetWinnerCount() > 0) {
			RoundWin();
			return RET_DONE;
		}

		// 次に鳴きチェック
		CheckOpponentNaki(m_Round.m_Turn.GetTurnPid());
		if (m_Round.m_Turn.GoNext()) {	// TURN 変更
			PlayerNaki(m_Round.m_Turn.GetTurnPid());
			State.SetState(ST_NAKI);
		} else {
			PlayerTumo(m_Round.m_Turn.GetTurnPid());
			State.SetState(ST_TUMO);
		}
		return RET_BUSY;

	case ST_NAKI:
		// 鳴き
		CALL_STATE_FUNC(GameState_Naki);
		return RET_DONE;

	case ST_TUMO:
		// ツモ
		CALL_STATE_FUNC(GameState_Tumo);
		return RET_DONE;

	default:
		MJASSERT(false);
		return RET_ERR;

	} /* endswitch StateVar[Level] */

	MJASSERT(false);
	return RET_ERR;

} /* end of MJGAMEMASTER::GameState_Select */



ssize_t	MJGAMEMASTER::GameState_Tumo(STATEREF State)
{
	enum {
		ST_TUMO,
		ST_KAN,
	};

	switch (State.GetState()) {
	case ST_TUMO:
		// プレイヤー手番（ツモアガリ、暗カン、加カン、捨て牌）
		{
			size_t	Pid = m_Round.m_Turn.GetTurnPid();
			MJEVENT	Evt = GetEventPid(Pid);

			switch (Evt.m_Action) {
			case MJEVENT::AGARI:
				// ツモアガリ
				m_Round.m_WinPlayer.Add(Pid, MJ::NOBODY);
				RoundWin();
				return RET_DONE;

			case MJEVENT::ANKAN:
				// 暗カン
				PlayerAnkan(Pid, Evt.m_PaiArray);
				State.SetState(ST_KAN);
				break;

			case MJEVENT::KAKAN:
				// 加カン
				PlayerKakan(Pid, Evt.m_Pai);
				State.SetState(ST_KAN);
				break;

			case MJEVENT::SUTE:
				// 捨て牌
				PlayerSute(Pid, Evt.m_Pai /*, Evt.m_Flag*/);
				return RET_DONE;

			default:
				MJASSERT(false);
				return RET_ERR;

			} /* endswitch Evt.m_Action */
		}
		return RET_BUSY;

	case ST_KAN:
		// カン
		CALL_STATE_FUNC(GameState_Kan);
		if (m_Round.m_WinPlayer.GetWinnerCount() > 0) {
			// 勝者が出れば終了
			return RET_DONE;
		}
		State.SetState(ST_TUMO);
		return RET_BUSY;

	default:
		MJASSERT(false);
		return RET_ERR;

	} /* endswitch StateVar[Level] */

	MJASSERT(false);
	return RET_ERR;

} /* end of MJGAMEMASTER::GameState_Tumo */



ssize_t	MJGAMEMASTER::GameState_Naki(STATEREF State)
{
	enum {
		ST_BRANCH,
		ST_NAKI,
		ST_KAN,
	};

	if (State.GetState() == ST_BRANCH) {
		size_t	Pid = m_Round.m_Turn.GetTurnPid();
		MJEVENT	Evt = GetEventPid(Pid);

		State.SetState(Evt.m_Action == MJEVENT::ACK ? ST_KAN : ST_NAKI);
	}

	switch (State.GetState()) {
	case ST_NAKI:
		// プレイヤー手番（アガリ、捨て牌）
		{
			size_t	Pid = m_Round.m_Turn.GetTurnPid();
			MJEVENT	Evt = GetEventPid(Pid);

			switch (Evt.m_Action) {
			case MJEVENT::AGARI:
				// ツモアガリ
				m_Round.m_WinPlayer.Add(Pid, MJ::NOBODY);
				RoundWin();
				return RET_DONE;

			case MJEVENT::SUTE:
				// 捨て牌
				PlayerSute(Pid, Evt.m_Pai /*, Evt.m_Flag*/);
				return RET_DONE;

			default:
				MJASSERT(false);
				return RET_ERR;

			} /* endswitch Evt.m_Action */
		}
		return RET_BUSY;

	case ST_KAN:
		// カン
		CALL_STATE_FUNC(GameState_Kan);
		if (m_Round.m_WinPlayer.GetWinnerCount() > 0) {
			// 勝者が出れば終了
			return RET_DONE;
		}
		State.SetState(ST_NAKI);
		return RET_BUSY;

	case ST_BRANCH:
	default:
		MJASSERT(false);
		return RET_ERR;

	} /* endswitch StateVar[Level] */

	MJASSERT(false);
	return RET_ERR;

} /* end of MJGAMEMASTER::GameState_Naki */



ssize_t	MJGAMEMASTER::GameState_Kan(STATEREF State)
{
	enum {
		ST_KANDORA,
		ST_RINSHAN,
		ST_END,
	};

	switch (State.GetState()) {
	case ST_KANDORA:
		// チャンカン確認
		CheckOpponentWinner(m_Round.m_Turn.GetTurnPid());
		if (m_Round.m_WinPlayer.GetWinnerCount() > 0) {
			RoundWin();
			return RET_DONE;
		}

		// カンドラ開示
		OpenDora();
		State.SetState(ST_RINSHAN);
		return RET_BUSY;

	case ST_RINSHAN:
		// リンシャンツモ
		PlayerRinshan(m_Round.m_Turn.GetTurnPid());
		return RET_DONE;

	default:
		MJASSERT(false);
		return RET_ERR;

	} /* endswitch StateVar[Level] */

	MJASSERT(false);
	return RET_ERR;

} /* end of MJGAMEMASTER::GameState_Kan */



//----------------------------------------------------------------------------
// ゲームルール
//----------------------------------------------------------------------------
void	MJGAMEMASTER::GameStart()
{
	m_RoundCount.Reset(m_Rule);
	for (size_t Pno = 0; Pno < m_Rule.m_Seq.m_PlayerCount; Pno++) {
		m_Player[Pno].Reset(m_Rule, Pno);
	}
	m_PoolPoint = 0;

	//
	// イベント送信
	//
	SetEventAll(MJEVENT_ACK(MJEVENT::GAME_START));

} /* end of MJGAMEMASTER::GameStart */



void	MJGAMEMASTER::GameEnd()
{
	//
	// イベント送信
	//
	SetEventAll(MJEVENT_ACK(MJEVENT::GAME_END));

} /* end of MJGAMEMASTER::GameEnd */



bool	MJGAMEMASTER::GameIsFinished()
{
	if (!m_Rule.m_Seq.m_Hakoshita) {
		// 箱下なしなら、誰かがマイナスになった時点で終了
		for (size_t Pno = 0; Pno < m_Rule.m_Seq.m_PlayerCount; Pno++) {
			if (m_Player[Pno].m_Points.GetPoint() < 0) {
				return true;
			}
		}
	}

	m_RoundCount.GoNext(
		m_Round.m_WinPlayer.GetWinnerCount() > 0,
		GetPlayer(MJ::TON).m_bAgari,
		GetPlayer(MJ::TON).m_Shanten <= 0,
		m_Round.m_bTochuRyukyoku
	);

	return m_RoundCount.IsFinished();

} /* end of MJGAMEMASTER::GameIsFinished */



void	MJGAMEMASTER::RoundStart()
{
	vector<size_t>	Order;

	m_RoundCount.GetPlayerOrder(Order);
	for (size_t Pid = 0; Pid < m_Rule.m_Seq.m_PlayerCount; Pid++) {
		m_Player[Order[Pid]].ResetRound(Pid);
	}
	m_Round.Reset(m_Rule, m_RoundCount.GetRoundNo() / 4, m_Rule.m_Yaku.m_RyanhanShibari && (m_RoundCount.GetHombaNo() >= 4));

	//
	// イベント送信
	//
	SetEventAll(MJEVENT_ACK(MJEVENT::ROUND_START));

} /* end of MJGAMEMASTER::RoundStart */



void	MJGAMEMASTER::RoundEnd()
{
	//
	// イベント送信
	//
	SetEventAll(MJEVENT_ACK(MJEVENT::ROUND_END));

} /* end of MJGAMEMASTER::RoundEnd */



void	MJGAMEMASTER::RoundWin()
{
	//
	// イベント送信
	//
	SetEventAll(MJEVENT_ACK(MJEVENT::AGARI));

} /* end of MJGAMEMASTER::RoundWin */



void	MJGAMEMASTER::RoundRyukyoku()
{
	//
	// イベント送信
	//
	SetEventAll(MJEVENT_ACK(MJEVENT::NAGARE));

} /* end of MJGAMEMASTER::RoundRyukyoku */



void	MJGAMEMASTER::Haipai4(size_t TurnPid)
{
	MJPAIARRAY	Haipai;

	for (size_t ii = 0; ii < 4; ii++) {
		Haipai += m_Round.m_Yama.PopTumo();
	}
	GetPlayer(TurnPid).m_Hand[0] += Haipai;

	//
	// イベント送信
	//
	SetEventAll(
		MJEVENT(
			MJEVENT::HAIPAI4,
			(1 << MJEVENT::ACK),
			TurnPid,
			MJ::NOBODY,
			MJPAI(),
			Haipai
		),
		TurnPid,
		true,
		(1 << MJEVENT::ACK)
	);

} /* end of MJGAMEMASTER::Haipai4 */



void	MJGAMEMASTER::Haipai1(size_t TurnPid)
{
	MJPAI	Haipai;

	Haipai = m_Round.m_Yama.PopTumo();
	GetPlayer(TurnPid).m_Hand[0] += Haipai;

	//
	// イベント送信
	//
	SetEventAll(
		MJEVENT(
			MJEVENT::HAIPAI1,
			(1 << MJEVENT::ACK),
			TurnPid,
			MJ::NOBODY,
			Haipai,
			MJPAIARRAY()
		),
		TurnPid,
		true,
		(1 << MJEVENT::ACK)
	);

} /* end of MJGAMEMASTER::Haipai1 */



void	MJGAMEMASTER::HaipaiEnd()
{
	for (size_t Pid = 0; Pid < m_Rule.m_Seq.m_PlayerCount; Pid++) {
		GetPlayer(Pid).m_Hand[0].Sort();
	}

	//
	// イベント送信
	//
	SetEventAll(MJEVENT_ACK(MJEVENT::HAIPAI_END));

} /* end of MJGAMEMASTER::HaipaiEnd */



void	MJGAMEMASTER::OpenDora()
{
	MJPAI	Dora;

	Dora = m_Round.m_Wampai.OpenDora();
	m_Round.m_Dora.Add(Dora);

	//
	// イベント送信
	//
	SetEventAll(
		MJEVENT(
			MJEVENT::DORA_OPEN,
			(1 << MJEVENT::ACK),
			MJ::NOBODY,
			MJ::NOBODY,
			Dora,
			MJPAIARRAY()
		)
	);

} /* end of MJGAMEMASTER::OpenDora */



void	MJGAMEMASTER::PlayerTumo(size_t TurnPid)
{
	MJPAI	Tumo;

	// 手牌の処理
	Tumo = m_Round.m_Yama.PopTumo();
	GetPlayer(TurnPid).m_Hand[0].push_back(Tumo);
	// その他の処理
	m_Round.m_KanOpponentPid = MJ::NOBODY;
	m_Round.m_bRinshanNow = false;

	//
	// イベント送信
	//
	SetEventAll(
		MJEVENT(
			MJEVENT::TUMO,
			(
				(1 << MJEVENT::SUTE) |
				(1 << MJEVENT::ANKAN) |
				(1 << MJEVENT::KAKAN) |
				(1 << MJEVENT::AGARI) |
				(1 << MJEVENT::NAGARE)
			),
			TurnPid,
			MJ::NOBODY,
			Tumo,
			MJPAIARRAY()
		),
		TurnPid,
		true,
		(1 << MJEVENT::ACK)
	);

} /* end of MJGAMEMASTER::PlayerTumo */



void	MJGAMEMASTER::PlayerNaki(size_t TurnPid)
{
	size_t	OpponentPid = m_Round.m_Turn.GetNakiOpponentPid();
	size_t	Act = m_Round.m_Turn.GetNakiAction();

	// 手牌の処理
	GetPlayer(TurnPid).m_Hand[0] -= m_Round.m_Turn.GetNakiMentsu();
	GetPlayer(TurnPid).m_Hand.push_back(m_Round.m_Turn.GetNakiFuuro());
	// 河の処理
	GetPlayer(OpponentPid).m_Kawa.HideTail();
	// その他の処理
	if (Act == MJEVENT::KAN) {
		m_Round.m_KanOpponentPid = OpponentPid;
		m_Round.m_KanPlayer.Add(TurnPid);
	} else {
		m_Round.m_KanOpponentPid = MJ::NOBODY;
	}
	m_Round.m_bRinshanNow = false;

	//
	// イベント送信
	//
	SetEventAll(
		MJEVENT(
			Act,
			(Act == MJEVENT::KAN) ? (1 << MJEVENT::ACK) : (1 << MJEVENT::SUTE),
			TurnPid,
			OpponentPid,
			m_Round.m_Turn.GetNakiSute(),
			m_Round.m_Turn.GetNakiMentsu()
		),
		TurnPid,
		false,
		(1 << MJEVENT::ACK)
	);

} /* end of MJGAMEMASTER::PlayerNaki */



void	MJGAMEMASTER::PlayerAnkan(size_t TurnPid, const MJPAIARRAY &Mentsu)
{
	MJPAIARRAY	Fuuro;

	// 手牌の処理
	GetPlayer(TurnPid).m_Hand[0] -= Mentsu;
	Fuuro = Mentsu;
	Fuuro &= ~MJPAI::CLOSED;
	Fuuro[0] |= MJPAI::CLOSED;
	Fuuro[3] |= MJPAI::CLOSED;
	GetPlayer(TurnPid).m_Hand.push_back(Fuuro);
	// その他の処理
	m_Round.m_KanOpponentPid = MJ::NOBODY;
	m_Round.m_KanPlayer.Add(TurnPid);
	m_Round.m_bRinshanNow = false;

	//
	// イベント送信
	//
	SetEventAll(
		MJEVENT(
			MJEVENT::ANKAN,
			(1 << MJEVENT::ACK),
			TurnPid,
			MJ::NOBODY,
			MJPAI(),
			Fuuro
		),
		TurnPid,
		false,
		(m_Rule.m_Yakuman.m_Kokushimusou_Chankan == 1) ? (
			(1 << MJEVENT::AGARI) |
			(1 << MJEVENT::ACK)
		) : (
			(1 << MJEVENT::ACK)
		)
	);

} /* end of MJGAMEMASTER::PlayerAnkan */



void	MJGAMEMASTER::PlayerKakan(size_t TurnPid, const MJPAI &Pai)
{
	// 手牌の処理
	GetPlayer(TurnPid).m_Hand[0] -= Pai;
	for (size_t ii = 1; ii < GetPlayer(TurnPid).m_Hand.size(); ii++) {
		MJPAIARRAY	&Hand = GetPlayer(TurnPid).m_Hand[ii];
		MJPAICOUNT	Cnt(Hand);

		if (Cnt.Include(MJPAICOUNT(Pai))) {
			MJPAI	FuuroPai;

			FuuroPai = Pai;
			FuuroPai &= ~MJPAI::CLOSED;
			Hand.insert(Hand.begin() + 3, FuuroPai);
			break;
		}
	}
	// その他の処理
	m_Round.m_KanOpponentPid = MJ::NOBODY;
	m_Round.m_KanPlayer.Add(TurnPid);
	m_Round.m_bRinshanNow = false;

	//
	// イベント送信
	//
	SetEventAll(
		MJEVENT(
			MJEVENT::KAKAN,
			(1 << MJEVENT::ACK),
			TurnPid,
			MJ::NOBODY,
			Pai,
			MJPAIARRAY()
		),
		TurnPid,
		false,
		(
			(1 << MJEVENT::AGARI) |
			(1 << MJEVENT::ACK)
		)
	);

} /* end of MJGAMEMASTER::PlayerKakan */



void	MJGAMEMASTER::PlayerRinshan(size_t TurnPid)
{
	MJPAI	Tumo, Tail;

	// 手牌の処理
	Tail = m_Round.m_Yama.PopTail();
	m_Round.m_Wampai.PushTail(Tail);
	Tumo = m_Round.m_Wampai.PopRinshan();
	GetPlayer(TurnPid).m_Hand[0].push_back(Tumo);
	// その他の処理
	m_Round.m_bRinshanNow = true;

	//
	// イベント送信
	//
	SetEventAll(
		MJEVENT(
			MJEVENT::RINSHAN,
			(
				(1 << MJEVENT::SUTE) |
				(1 << MJEVENT::ANKAN) |
				(1 << MJEVENT::KAKAN) |
				(1 << MJEVENT::AGARI) |
				(1 << MJEVENT::NAGARE)
			),
			TurnPid,
			MJ::NOBODY,
			Tumo,
			MJPAIARRAY()
		),
		TurnPid,
		true,
		(1 << MJEVENT::ACK)
	);

} /* end of MJGAMEMASTER::PlayerRinshan */



void	MJGAMEMASTER::PlayerSute(size_t TurnPid, const MJPAI &Pai /*, size_t Flag*/)
{
	MJPAI	Sute;

	// 手牌の処理
	GetPlayer(TurnPid).m_Hand[0] -= Pai;
	GetPlayer(TurnPid).m_Hand[0].Sort();
	// 河の処理
	Sute = Pai;
	Sute |= MJPAI::DISABLED;
	GetPlayer(TurnPid).m_Kawa.PushSute(Sute);
	// その他の処理
	m_Round.m_KanOpponentPid = MJ::NOBODY;
	m_Round.m_bRinshanNow = false;

	//
	// イベント送信
	//
	vector<size_t>	Mask;

	for (size_t ii = 0; ii < m_Rule.m_Seq.m_PlayerCount; ii++) {
		switch (m_Round.m_Turn.GetPidd(TurnPid, ii)) {
		case MJ::SELF:
			Mask.push_back(
				(1 << MJEVENT::ACK)
			);
			break;

		case MJ::SIMO:
			Mask.push_back(
				FilterActionMask(
					GetPlayer(ii).m_Hand,
					Sute,
					(1 << MJEVENT::CHI) |
					(1 << MJEVENT::PON) |
					(1 << MJEVENT::KAN) |
					(1 << MJEVENT::AGARI) |
					(1 << MJEVENT::ACK)
				)
			);
			break;

		case MJ::TOI:
		case MJ::KAMI:
		default:
			Mask.push_back(
				FilterActionMask(
					GetPlayer(ii).m_Hand,
					Sute,
					(1 << MJEVENT::PON) |
					(1 << MJEVENT::KAN) |
					(1 << MJEVENT::AGARI) |
					(1 << MJEVENT::ACK)
				)
			);
			break;

		} /* endswitch */
	}

	SetEventAll(
		MJEVENT(
			MJEVENT::SUTE,
			(1 << MJEVENT::ACK),	// toriaezu
			TurnPid,
			MJ::NOBODY,
			Sute,
			MJPAIARRAY()
		),
		TurnPid,
		false,
		Mask
	);

} /* end of MJGAMEMASTER::PlayerSute */



size_t	MJGAMEMASTER::FilterActionMask(const MJHAND &Hand, const MJPAI &Sute, size_t Mask)
{
	MJPAICOUNT	Count(Hand[0]);

	if (Mask & (1 << MJEVENT::CHI)) {
		if (
			Sute.IsJihai() ||
			(Sute.GetNumber() >= 3) && ((Count[Sute - 2] == 0) || (Count[Sute - 1] == 0)) ||
			(Sute.GetNumber() >= 2) && (Sute.GetNumber() <= 8) && ((Count[Sute - 1] == 0) || (Count[Sute + 1] == 0)) ||
			(Sute.GetNumber() <= 7) && ((Count[Sute + 1] == 0) || (Count[Sute + 2] == 0))
		) {
			Mask &= ~(1 << MJEVENT::CHI);
		}
	}

	if (Mask & (1 << MJEVENT::PON)) {
		if (
			(Count[Sute] < 2)
		) {
			Mask &= ~(1 << MJEVENT::PON);
		}
	}

	if (Mask & (1 << MJEVENT::KAN)) {
		if (
			(Count[Sute] < 3)
		) {
			Mask &= ~(1 << MJEVENT::KAN);
		}
	}

	if (Mask & (1 << MJEVENT::AGARI)) {
		if (
			true	// FIXME: agari check
		) {
			Mask &= ~(1 << MJEVENT::AGARI);
		}
	}

	return Mask;

} /* end of MJGAMEMASTER::FilterActionMask */



void	MJGAMEMASTER::CheckOpponentWinner(size_t TurnPid)
{
	for (size_t Pidd = 1; Pidd < m_Rule.m_Seq.m_PlayerCount; Pidd++) {
		size_t	Pid = (TurnPid + Pidd) % m_Rule.m_Seq.m_PlayerCount;
		MJEVENT	Evt = GetEventPid(Pid);

		if (Evt.m_Action == MJEVENT::AGARI) {
			m_Round.m_WinPlayer.Add(Pid, TurnPid);
			if (m_Rule.m_Seq.m_DoubleRon == 0) {
				// 
				return;
			}
		}
	}

} /* end of MJGAMEMASTER::CheckOpponentWinner */



void	MJGAMEMASTER::CheckOpponentNaki(size_t TurnPid)
{
	m_Round.m_Turn.ResetNaki();

	// カン、ポン優先
	for (size_t Pidd = 1; Pidd < m_Rule.m_Seq.m_PlayerCount; Pidd++) {
		size_t	Pid = (TurnPid + Pidd) % m_Rule.m_Seq.m_PlayerCount;
		MJEVENT	Evt = GetEventPid(Pid);

		if (Evt.m_Action == MJEVENT::KAN) {
			m_Round.m_Turn.SetNaki(Pid, TurnPid, MJEVENT::KAN, Evt.m_Pai, Evt.m_PaiArray);
		}
		if (Evt.m_Action == MJEVENT::PON) {
			m_Round.m_Turn.SetNaki(Pid, TurnPid, MJEVENT::PON, Evt.m_Pai, Evt.m_PaiArray);
		}
		if (Evt.m_Action == MJEVENT::CHI) {
			m_Round.m_Turn.SetNaki(Pid, TurnPid, MJEVENT::CHI, Evt.m_Pai, Evt.m_PaiArray);
		}
	}

} /* end of MJGAMEMASTER::CheckOpponentNaki */



//----------------------------------------------------------------------------
#if 0
ssize_t	MJGAMEMASTER::GetPno(ssize_t Pid) const
{
	MJASSERT(Pid != MJ::NOBODY);

	return m_RoundOrder.at(Pid);

} /* end of MJGAMEMASTER::GetPno */
#endif



#if 0
ssize_t	MJGAMEMASTER::GetPid(ssize_t Pno) const
{
	MJASSERT(Pno < m_Rule.m_Seq.m_PlayerCount);

	for (size_t Pid = 0; Pid < m_Rule.m_Seq.m_PlayerCount; Pid++) {
		if (Pno == m_RoundOrder.at(Pid)) {
			return Pid;
		}
	}

	MJASSERT(false);
	return MJ::NOBODY;

} /* end of MJGAMEMASTER::GetPid */
#endif



MJPLAYER	&MJGAMEMASTER::GetPlayer(size_t Pid)
{
	MJASSERT(Pid != MJ::NOBODY);

	return m_Player[m_RoundCount.GetPlayer(Pid)];

} /* end of MJGAMEMASTER::GetPlayer */



#if 0
MJEVENT	MJGAMEMASTER::GetEvent(const MJINTERFACE *pInterface)
{
	MJASSERT(pInterface);

	ssize_t	err;
	MJEVENT	Evt;

	err = m_Plug[pInterface].Recv(Evt);
	MJASSERT(err == RET_DONE);
	return Evt;

} /* end of MJGAMEMASTER::GetEvent */



MJEVENT	MJGAMEMASTER::GetEventPno(ssize_t Pno)
{
	MJASSERT(Pno < m_Rule.m_Seq.m_PlayerCount);

	return GetEvent(m_Interface.at(Pno));

} /* end of MJGAMEMASTER::GetEventPno */
#endif



MJEVENT	MJGAMEMASTER::GetEventPid(size_t Pid)
{
	MJASSERT(Pid != MJ::NOBODY);

	MJEVENT	Evt;

	GetPlayer(Pid).m_Plug.Recv(Evt);
	return Evt;

} /* end of MJGAMEMASTER::GetEventPid */



void	MJGAMEMASTER::SetEventPno(size_t Pno, const MJEVENT &Event)
{
	MJASSERT(Pno < m_Player.size());

	m_Player[Pno].m_Plug.Send(Event);

} /* end of MJGAMEMASTER::SetEventPno */



void	MJGAMEMASTER::SetEventAll(const MJEVENT &Event)
{
	for (size_t Pno = 0; Pno < m_Rule.m_Seq.m_PlayerCount; Pno++) {
		SetEventPno(Pno, Event);
	}

} /* end of MJGAMEMASTER::SetEventAll */



void	MJGAMEMASTER::SetEventAll(const MJEVENT &Event, size_t Pid, bool bShade, size_t Mask)
{
	for (size_t Pno = 0; Pno < m_Rule.m_Seq.m_PlayerCount; Pno++) {
		MJEVENT	Evt = Event;
		if ((Pid != MJ::NOBODY) && (Pno != m_RoundCount.GetPlayer(Pid))) {
			if (bShade) {
				Evt = Evt.Shade();
			}
			Evt &= 0;
			Evt |= Mask;
		}
		SetEventPno(Pno, Evt);
	}

} /* end of MJGAMEMASTER::SetEventAll */



void	MJGAMEMASTER::SetEventAll(const MJEVENT &Event, size_t Pid, bool bShade, const vector<size_t> &Mask)
{
	for (size_t Pno = 0; Pno < m_Rule.m_Seq.m_PlayerCount; Pno++) {
		MJEVENT	Evt = Event;
		if ((Pid != MJ::NOBODY) && (Pno != m_RoundCount.GetPlayer(Pid))) {
			if (bShade) {
				Evt = Evt.Shade();
			}
		}
		Evt &= 0;
		Evt |= Mask[m_Player[Pno].m_Pid];
		SetEventPno(Pno, Evt);
	}

} /* end of MJGAMEMASTER::SetEventAll */



//----------------------------------------------------------------------------
