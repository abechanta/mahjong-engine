#include "mjturn.h"

//----------------------------------------------------------------------------
MJTURN::MJTURN()
{
} /* end of MJTURN::MJTURN */



MJTURN::~MJTURN()
{
} /* end of MJTURN::~MJTURN */



//----------------------------------------------------------------------------
void	MJTURN::Reset(size_t PlayerCount)
{
	m_PlayerCount = PlayerCount;
	m_TurnNo = 0;
	m_TurnPid = MJ::NOBODY;
	m_bNakiTurn = false;
	m_NakiPid = MJ::NOBODY;
	m_NakiOpponentPid = MJ::NOBODY;
	m_NakiAction = MJEVENT::NONE;
	m_NakiSute = MJPAI();
	m_NakiMentsu = MJPAIARRAY();

} /* end of MJROUNDCOUNT::Reset */



size_t	MJTURN::GetTurnNo() const
{
	return m_TurnNo;

} /* end of MJTURN::GetTurnNo */



size_t	MJTURN::GetTurnPid() const
{
	return m_TurnPid;

} /* end of MJTURN::GetTurnPid */



size_t	MJTURN::GetNakiOpponentPid() const
{
	return m_bNakiTurn ? m_NakiOpponentPid : MJ::NOBODY;

} /* end of MJTURN::GetNakiOpponentPid */



size_t	MJTURN::GetNakiAction() const
{
	return m_bNakiTurn ? m_NakiAction : MJEVENT::NONE;

} /* end of MJTURN::GetNakiAction */



MJPAI	MJTURN::GetNakiSute() const
{
	return m_bNakiTurn ? m_NakiSute : MJPAI();

} /* end of MJTURN::GetNakiSute */



MJPAIARRAY	MJTURN::GetNakiMentsu() const
{
	return m_bNakiTurn ? m_NakiMentsu : MJPAIARRAY();

} /* end of MJTURN::GetNakiMentsu */



//----------------------------------------------------------------------------
void	MJTURN::ResetNaki()
{
	m_NakiPid = MJ::NOBODY;

} /* end of MJTURN::ResetNaki */



void	MJTURN::SetNaki(size_t Pid, size_t OpponentPid, size_t Action, const MJPAI &Sute, const MJPAIARRAY &Mentsu)
{
	MJASSERT(Pid != MJ::NOBODY);
	MJASSERT(OpponentPid != MJ::NOBODY);
	MJASSERT((Action == MJEVENT::CHI) || (Action == MJEVENT::PON) || (Action == MJEVENT::KAN));
	MJASSERT((Action != MJEVENT::CHI) || (Mentsu.size() == 2));
	MJASSERT((Action != MJEVENT::PON) || (Mentsu.size() == 2));
	MJASSERT((Action != MJEVENT::KAN) || (Mentsu.size() == 3));

	if (
		(m_NakiAction == MJEVENT::NONE) ||
		(m_NakiAction == MJEVENT::CHI)
	) {
		m_NakiPid = Pid;
		m_NakiOpponentPid = OpponentPid;
		m_NakiAction = Action;
		m_NakiSute = Sute;
		m_NakiSute &= ~MJPAI::CLOSED;
		m_NakiMentsu = Mentsu;
		m_NakiMentsu &= ~MJPAI::CLOSED;
	}

} /* end of MJTURN::SetNaki */



bool	MJTURN::GoNext()
{
	if (m_NakiPid != MJ::NOBODY) {
		if (m_NakiPid < m_TurnPid) {
			m_TurnNo++;
		}
		m_TurnPid = m_NakiPid;
		m_bNakiTurn = true;	// 鳴きでした
		return m_bNakiTurn;
	}

	m_bNakiTurn = false;	// 鳴きじゃない
	if (
		(m_TurnNo == 0) &&
		(m_TurnPid == MJ::NOBODY)
	) {
		m_TurnPid = MJ::TON;
	} else if (++m_TurnPid >= m_PlayerCount) {
		m_TurnNo++;
		m_TurnPid = MJ::TON;
	}
	return m_bNakiTurn;

} /* end of MJTURN::GoNext */



MJPAIARRAY	MJTURN::GetNakiFuuro() const
{
	if (!m_bNakiTurn) {
		return MJPAIARRAY();
	}

	MJPAI	Pai;
	MJPAIARRAY	Fuuro;

	Fuuro += m_NakiMentsu;

	Pai = m_NakiSute;
	Pai |= MJPAI::SIDEWAYS;
	if (m_NakiAction == MJEVENT::CHI) {
		Fuuro.push_front(Pai);
	} else {
		switch (GetPidd(m_NakiPid, m_NakiOpponentPid)) {
		case MJ::SIMO:
			Fuuro.push_back(Pai);
			break;
		case MJ::TOI:
			Fuuro = m_NakiMentsu;
			Fuuro.insert(Fuuro.begin() + 1, Pai);
			break;
		case MJ::KAMI:
			Fuuro.push_front(Pai);
			break;
		case MJ::SELF:
		default:
			MJASSERT(false);
			break;
		} /* endswitrch GetPidd(m_NakiPid, m_NakiOpponentPid) */
	}
	Fuuro &= ~MJPAI::CLOSED;

	return Fuuro;

} /* end of MJTURN::GetNakiFuuro */



//----------------------------------------------------------------------------
size_t	MJTURN::GetPidd(size_t Pid, size_t OpponentPid) const
{
	MJASSERT(Pid != MJ::NOBODY);
	MJASSERT(OpponentPid != MJ::NOBODY);

	if (Pid < OpponentPid) {
		return OpponentPid - Pid;
	} else if (Pid > OpponentPid) {
		return m_PlayerCount - (Pid - OpponentPid);
	}
	/* if (Pid == OpponentPid) */
	return MJ::SELF;

} /* end of MJTURN::GetPidd */



//----------------------------------------------------------------------------
