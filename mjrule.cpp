#include "mjrule.h"
using namespace std;

//----------------------------------------------------------------------------
MJRULE::MJRULE(const string &Name)
{
	if (Name != "") {
		SetRule(Name);
	}

} /* end of MJRULE::MJRULE */



MJRULE	&MJRULE::SetRule(const string &Name)
{
	if (Name == "JapanProfessionalMahjongLeagueRuleA") {
		// 日本プロ麻雀連盟 競技ルール A
		m_Seq.m_PlayerCount = 4;
		m_Seq.m_RoundCount = 8;
		m_Seq.m_WampaiCount = 14;
		m_Seq.m_TehaiCount = 13;
		m_Seq.m_MentsuCount = 4;
		m_Seq.m_NotenOyanagare = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
		m_Seq.m_Keiten = 1;
		m_Seq.m_Hakoshita = 0;
		m_Seq.m_HakoshitaReach = 0;
		m_Seq.m_OhrasuOyanagare = 1;
		m_Seq.m_OhrasuAgariyame = 1;
		m_Seq.m_TochuRyukyokuRenchan = 1;
		m_Seq.m_DoubleRon = 0;
		m_Seq.m_TripleRon = 0;
		m_Seq.m_NakiPriority = 0;
		m_Pts.m_StartPoint = 30000;
		m_Pts.m_SeisanPoint = 30000;
		m_Pts.m_UmaPoint0Plus[0] = 8000;
		m_Pts.m_UmaPoint0Plus[1] = 4000;
		m_Pts.m_UmaPoint0Plus[2] = -4000;
		m_Pts.m_UmaPoint0Plus[3] = -8000;
		m_Pts.m_UmaPoint1Plus[0] = 12000;
		m_Pts.m_UmaPoint1Plus[1] = -1000;
		m_Pts.m_UmaPoint1Plus[2] = -3000;
		m_Pts.m_UmaPoint1Plus[3] = -8000;
		m_Pts.m_UmaPoint2Plus[0] = 8000;
		m_Pts.m_UmaPoint2Plus[1] = 4000;
		m_Pts.m_UmaPoint2Plus[2] = -4000;
		m_Pts.m_UmaPoint2Plus[3] = -8000;
		m_Pts.m_UmaPoint3Plus[0] = 8000;
		m_Pts.m_UmaPoint3Plus[1] = 3000;
		m_Pts.m_UmaPoint3Plus[2] = 1000;
		m_Pts.m_UmaPoint3Plus[3] = -12000;
		m_Pts.m_TobiPoint = 0;
		m_Pts.m_YakumanExtraPointTumo = 0;
		m_Pts.m_YakumanExtraPointRon = 0;
		m_Pts.m_Pao = 1;
		m_Pts.m_Wareme = 0;
		m_Dora.m_DoraCount = 1;
		m_Dora.m_UraDora = 1;
		m_Dora.m_KanDora = 1;
		m_Dora.m_KanUra = 1;
		m_Dora.m_KanSoku = 1;
		m_Yaku.m_PinfuTumo = 1;
		m_Yaku.m_Kuitan = 1;
		m_Yaku.m_Atozuke = 1;
		m_Yaku.m_RyanhanShibari = 0;
		m_Yaku.m_Chitoitsu25hu2han = 1;
		m_Yaku.m_Renho = 0;
		m_Yakuman.m_Kokushimusou_FuritenRon = 0;
		m_Yakuman.m_Kokushimusou_Chankan = 1;
		m_Yakuman.m_Ryuiisou_Hatunashi = 1;
		m_Yakuman.m_Churenpoutou_FuritenRon = 0;
		for (size_t ii = MJPAI::MAN1; ii < MJPAI::MAX; ii++) {
			m_Set += MJPAI(ii, 0);
		}
		for (size_t ii = MJPAI::MAN1; ii < MJPAI::MAX; ii++) {
			m_Set += MJPAI(ii, 0);
		}
		for (size_t ii = MJPAI::MAN1; ii < MJPAI::MAX; ii++) {
			m_Set += MJPAI(ii, 0);
		}
		for (size_t ii = MJPAI::MAN1; ii < MJPAI::MAX; ii++) {
			m_Set += MJPAI(ii, 0);
		}
	}

	return *this;

} /* end of MJRULE::SetRule */



//----------------------------------------------------------------------------
