#include "mjroundcount.h"
#include <algorithm>
using namespace std;

//----------------------------------------------------------------------------
MJROUNDCOUNT::MJROUNDCOUNT()
{
} /* end of MJROUNDCOUNT::MJROUNDCOUNT */



MJROUNDCOUNT::~MJROUNDCOUNT()
{
} /* end of MJROUNDCOUNT::~MJROUNDCOUNT */



//----------------------------------------------------------------------------
void	MJROUNDCOUNT::Reset(const MJRULE &Rule)
{
	m_Rule = Rule;
	m_Order.clear();
	for (size_t ii = 0; ii < m_Rule.m_Seq.m_PlayerCount; ii++) {
		m_Order.push_back(MJ::P1 + ii);
	}
	random_shuffle(m_Order.begin(), m_Order.end());
	m_RoundNo = 0;
	m_HombaNo = 0;
	m_bFinished = false;

} /* end of MJROUNDCOUNT::Reset */



void	MJROUNDCOUNT::GetPlayerOrder(vector<size_t> &Order) const
{
	Order.clear();
	Order = m_Order;
	rotate(Order.begin(), Order.begin() + (m_RoundNo % Order.size()), Order.end());

} /* end of MJROUNDCOUNT::GetPlayerOrder */



size_t	MJROUNDCOUNT::GetPlayer(size_t Pid) const
{
	return m_Order[(m_RoundNo + Pid) % m_Order.size()];

} /* end of MJROUNDCOUNT::GetPlayer */



size_t	MJROUNDCOUNT::GetRoundNo() const
{
	return m_RoundNo;

} /* end of MJROUNDCOUNT::GetRoundNo */



size_t	MJROUNDCOUNT::GetHombaNo() const
{
	return m_HombaNo;

} /* end of MJROUNDCOUNT::GetHombaNo */



bool	MJROUNDCOUNT::IsFinished() const
{
	return m_bFinished;

} /* end of MJROUNDCOUNT::IsFinished */



//----------------------------------------------------------------------------
bool	MJROUNDCOUNT::GoNext(bool bAnyoneWon, bool bTonWon, bool bTonTempai, bool bTochu)
{
	if (bTonWon) {
		// 親が上がった
		if (IsLastRound()) {
			// オーラスアガリ止め
			m_bFinished = true;
			return true;	// 親流れ
		}
		m_HombaNo++;
		return false;	// 連荘
	}

	if (bAnyoneWon) {
		// 誰かが上がった
		if (IsLastRound()) {
			// オーラス終了
			m_bFinished = true;
			return true;	// 親流れ
		}
		m_RoundNo++;
		m_HombaNo = 0;
		return true;	// 親流れ
	}

	// 誰も上がらなかった
#if 1
	if (bTochu && m_Rule.m_Seq.m_TochuRyukyokuRenchan) {
		// 途中流局
		m_HombaNo++;
		return false;	// 連荘
	}

	if (
		!m_Rule.m_Seq.m_OhrasuOyanagare &&
		IsLastRound()
	) {
		// オーラスは流れない
		m_HombaNo++;
		return false;	// 連荘
	}

	if (
		!bTonTempai &&
		(m_Rule.m_Seq.m_NotenOyanagare & (1 << (((m_RoundNo / m_Rule.m_Seq.m_PlayerCount) % 4))))
	) {
		// ノーテン親流れ
		if (IsLastRound()) {
			// オーラス終了
			m_bFinished = true;
			return true;	// 親流れ
		}
		m_RoundNo++;
		m_HombaNo++;
		return true;	// 親流れ
	}
#endif

	m_HombaNo++;
	return false;	// 連荘

} /* end of MJROUNDCOUNT::GoNext */



bool	MJROUNDCOUNT::IsLastRound() const
{
	return !m_bFinished && (m_RoundNo == m_Rule.m_Seq.m_RoundCount - 1);

} /* end of MJROUNDCOUNT::IsLastRound */



//----------------------------------------------------------------------------
