#include "mjround.h"
#include <algorithm>
using namespace std;

//----------------------------------------------------------------------------
MJROUND::MJROUND()
{
} /* end of MJROUND::MJROUND */



MJROUND::~MJROUND()
{
} /* end of MJROUND::~MJROUND */



//----------------------------------------------------------------------------
void	MJROUND::Reset(const MJRULE &Rule, size_t Wind, bool bShibari)
{
	m_Rule = Rule;
	m_Wind = Wind;
	m_bShibari = bShibari;
	{
		MJPAIARRAY	Yama = m_Rule.m_Set;
		MJPAIARRAY	Wampai;

		random_shuffle(Yama.begin(), Yama.end());
		Wampai = m_Yama.Reset(Yama, m_Rule.m_Seq.m_WampaiCount);
		m_Wampai.Reset(Wampai, m_Rule.m_Seq.m_RinshanCount);
	}
	m_Dora.Reset();
	m_KanPlayer.Reset(m_Rule.m_Seq.m_RinshanCount);
	m_WinPlayer.Reset();
	m_Turn.Reset(m_Rule.m_Seq.m_PlayerCount);
	m_KanOpponentPid = MJ::NOBODY;
	m_bRinshanNow = false;
	m_bTochuRyukyoku = false;
//	m_DoraPending = 0;

} /* end of MJROUND::Reset */



//----------------------------------------------------------------------------
