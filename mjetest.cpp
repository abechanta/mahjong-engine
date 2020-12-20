// mjetest.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//
//

#include "stdafx.h"
#include "mjevent.h"
#include "mjgamemaster.h"
#include "mjpai.h"
#include "mjpaiarray.h"
#include "mjpaicount.h"
#include "mjinterface.h"
#include <algorithm>
#include <vector>
using namespace std;

#define SAFE_DELETE(ptr)\
{\
	if ((ptr) != NULL) {\
		delete (ptr);\
		(ptr) = NULL;\
	}\
}
#define SAFE_DELETE_ARRAY(ptr)\
{\
	if ((ptr) != NULL) {\
		delete[] (ptr);\
		(ptr) = NULL;\
	}\
}



ostream	&operator <<(ostream &Out, const MJPAI &Other)
{
	Out << Other.GetString() << '[' << Other.GetCode(false) << ']';
	return Out;

} /* end of operator << */



ostream	&operator <<(ostream &Out, const MJPAIARRAY &Other)
{
#if 0
	MJPAIARRAY::const_iterator	it;

	Out << "{ ";
	for (it = Other.begin(); it != Other.end(); ++it) {
		Out << *it << ' ';
	}
	Out << "}";
#else
	Out << Other.GetString();
#endif
	return Out;

} /* end of operator << */



ostream	&operator <<(ostream &Out, const MJPAICOUNT &Other)
{
	Out << Other.GetString();
	return Out;

} /* end of operator << */



//----------------------------------------------------------------------------
class MJINTERFACE_DEBUG_CUI : public MJINTERFACE
{
private:
	bool				m_bAutoNaki;
	bool				m_bAutoSute;

public:
	MJINTERFACE_DEBUG_CUI(const string &Name, bool bAutoSute, bool bAutoNaki)
		: MJINTERFACE(Name),
		m_bAutoSute(bAutoSute),
		m_bAutoNaki(bAutoNaki)
	{
	} /* end of MJINTERFACE_DEBUG_CUI::MJINTERFACE_DEBUG_CUI */

	ostream	&PrintPid(ostream &ostr, size_t Pid)
	{
		switch (Pid) {
		case MJ::TON:	cout << "TON"; break;
		case MJ::NAN:	cout << "NAN"; break;
		case MJ::SHA:	cout << "SHA"; break;
		case MJ::PEI:	cout << "PEI"; break;
		default:		cout << "???"; break;
		} /* endswitch Pid */

		return ostr;

	} /* end of MJINTERFACE_DEBUG_CUI::PrintPid */

	ostream	&PrintIndex(ostream &ostr, const MJPAIARRAY &Array)
	{
		for (size_t ii = 0; ii < Array.size(); ii++) {
			ostr << " " << (char)('A' + ii) << "  ";
		}
		return ostr;

	} /* end of MJINTERFACE_DEBUG_CUI::PrintIndex */

	ostream	&PrintPai(ostream &ostr, const MJPAIARRAY &Array)
	{
#if 0
		for (size_t ii = 0; ii < Array.size(); ii++) {
			ostr << Array[ii] << " ";
		}
#else
		ostr << Array << " ";
#endif
		return ostr;

	} /* end of MJINTERFACE_DEBUG_CUI::PrintPai */

	ostream	&PrintCmd(ostream &ostr, size_t Mask)
	{
		if (Mask & (1 << MJEVENT::ACK)) {
			cout << "ACK(q)  ";
		}
		if (Mask & (1 << MJEVENT::AGARI)) {
			cout << "AGARI(agari)  ";
		}
		if (Mask & (1 << MJEVENT::CHI)) {
			cout << "CHI(chi A-N A-N)  ";
		}
		if (Mask & (1 << MJEVENT::PON)) {
			cout << "PON(pon A-N A-N)  ";
		}
		if (Mask & (1 << MJEVENT::KAN)) {
			cout << "KAN(kan A-N A-N A-N)  ";
		}
		if (Mask & (1 << MJEVENT::SUTE)) {
			cout << "SUTE(A-N)  ";
		}
		return ostr;

	} /* end of MJINTERFACE_DEBUG_CUI::PrintCmd */

	size_t	ScanCmd(const string &Cmd, size_t Mask)
	{
		if (
			(Mask & (1 << MJEVENT::ACK)) &&
			(Cmd == "q")
		) {
			return MJEVENT::ACK;

		} else if (
			(Mask & (1 << MJEVENT::AGARI)) &&
			(Cmd == "agari")
		) {
			return MJEVENT::AGARI;

		} else if (
			(Mask & (1 << MJEVENT::CHI)) &&
			(Cmd == "chi")
		) {
			return MJEVENT::CHI;

		} else if (
			(Mask & (1 << MJEVENT::PON)) &&
			(Cmd == "pon")
		) {
			return MJEVENT::PON;

		} else if (
			(Mask & (1 << MJEVENT::KAN)) &&
			(Cmd == "kan")
		) {
			return MJEVENT::KAN;

		}
		return 0;

	} /* end of MJINTERFACE_DEBUG_CUI::ScanCmd */

	vector<size_t>	ScanIndex(const string &Index, size_t Limit, size_t ScanCount)
	{
		vector<size_t>	Args;

		if (Index.length() != ScanCount) {
			return Args;
		}
		for (size_t ii = 0; ii < ScanCount; ii++) {
			size_t	Idx = Index[ii] - 'a';

			if (
				(find(Args.begin(), Args.end(), Idx) == Args.end()) &&
				('a' <= Index[ii]) &&
				((uint8_t)Index[ii] < 'a' + Limit)
			) {
				Args.push_back(Idx);
			}
		}
		return Args;

	} /* end of MJINTERFACE_DEBUG_CUI::ScanIndex */

	virtual	ssize_t	Select(const MJEVENT &Event)
	{
		MJEVENT	Evt = Event;

		if (
			(Evt.m_ActionMask == (1 << MJEVENT::ACK)) ||
			m_bAutoNaki && (Evt.m_ActionMask & (1 << MJEVENT::ACK))
		) {
			// 勝手に返信する
			ssize_t	err;

			Evt.m_Action = MJEVENT::ACK;
			err = SendEvent(Evt);
			MJASSERT(err >= 0);
			return RET_DONE;
		}

		PrintPid(cout, m_pPlayer->m_Pid);
		cout << "[" << m_Name << "] Ready:" << endl;

		while (true) {
			cout << "    ";
			PrintIndex(cout, m_pPlayer->m_Hand[0]);
			cout << endl;

			cout << "  ";
			for (size_t ii = 0; ii < m_pPlayer->m_Hand.size(); ii++) {
				PrintPai(cout, m_pPlayer->m_Hand[ii]);
			}
			cout << endl;

			cout << "  ";
			PrintCmd(cout, Evt.m_ActionMask);
			cout << "? ";

			size_t	Act;

			if (m_bAutoSute) {
				cout << endl;

				Evt.m_Action = MJEVENT::SUTE;
				Evt.m_Pai = m_pPlayer->m_Hand[0][m_pPlayer->m_Hand[0].size() - 2];
				cout << Evt.m_Pai << endl;

			} else {
				string	cmd;

				cin >> cmd;
				cout << cmd << endl;

				Act = ScanCmd(cmd, Evt.m_ActionMask);
				vector<size_t>	Args;

				if (
					(Act == MJEVENT::CHI) ||
					(Act == MJEVENT::PON)
				) {
					cin >> cmd;
					Args = ScanIndex(cmd, (size_t)m_pPlayer->m_Hand[0].size(), 2);
					if (Args.size() <= 0) {
						continue;
					}
				} else if (
					(Act == MJEVENT::KAN)
				) {
					cin >> cmd;
					Args = ScanIndex(cmd, (size_t)m_pPlayer->m_Hand[0].size(), 3);
					if (Args.size() <= 0) {
						continue;
					}
				} else if (
					(Act == 0) &&
					(Evt.m_ActionMask & (1 << MJEVENT::SUTE))
				) {
					Act = MJEVENT::SUTE;
					Args = ScanIndex(cmd, (size_t)m_pPlayer->m_Hand[0].size(), 1);
					if (Args.size() <= 0) {
						continue;
					}
				} else if (
					(Act == MJEVENT::ACK) ||
					(Act == MJEVENT::AGARI)
				) {
					;
				} else {
					continue;
				}

				Evt.m_Action = Act;
				if (Args.size() == 1) {
					Evt.m_Pai = m_pPlayer->m_Hand[0][Args[0]];
				} else {
					for (size_t ii = 0; ii < Args.size(); ii++) {
						Evt.m_PaiArray += m_pPlayer->m_Hand[0][Args[0]];
					}
				}
			}

			break;
		}

		{
			ssize_t	err;

			err = SendEvent(Evt);
			MJASSERT(err >= 0);
			return RET_DONE;
		}

	} /* end of MJINTERFACE_DEBUG_CUI::Select */
};



//----------------------------------------------------------------------------
int _tmain(int argc, _TCHAR *argv[])
{
#if 0
	{
		cerr << "# MJPAI TEST #" << endl;

		MJPAI	pai(MJPAI::UNKNOWN, 0);

		cout << "pai = " << pai << endl;

		MJPAI	man1(MJPAI::MAN1, 0);
		MJPAI	man2(MJPAI::MAN2, 0);
		MJPAI	man3(MJPAI::MAN3, 0);
		MJPAI	man4(MJPAI::MAN4, 0);
		MJPAI	man5(MJPAI::MAN5, 0);
		MJPAI	man5r(MJPAI::MAN5, 1);
		MJPAI	man6(MJPAI::MAN6, 0);
		MJPAI	man7(MJPAI::MAN7, 0);
		MJPAI	man8(MJPAI::MAN8, 0);
		MJPAI	man9(MJPAI::MAN9, 0);

		cout << "man1 = " << man1 << endl;
		cout << "man2 = " << man2 << endl;
		cout << "man3 = " << man3 << endl;
		cout << "man4 = " << man4 << endl;
		cout << "man5 = " << man5 << endl;
		cout << "man5r = " << man5r << endl;
		cout << "man6 = " << man6 << endl;
		cout << "man7 = " << man7 << endl;
		cout << "man8 = " << man8 << endl;
		cout << "man9 = " << man9 << endl;

		MJPAI	sou1(MJPAI::SOU1, 0);
		MJPAI	sou2(MJPAI::SOU2, 0);
		MJPAI	sou3(MJPAI::SOU3, 0);
		MJPAI	sou4(MJPAI::SOU4, 0);
		MJPAI	sou5(MJPAI::SOU5, 0);
		MJPAI	sou5r(MJPAI::SOU5, 1);
		MJPAI	sou6(MJPAI::SOU6, 0);
		MJPAI	sou7(MJPAI::SOU7, 0);
		MJPAI	sou8(MJPAI::SOU8, 0);
		MJPAI	sou9(MJPAI::SOU9, 0);

		cout << "sou1 = " << sou1 << endl;
		cout << "sou2 = " << sou2 << endl;
		cout << "sou3 = " << sou3 << endl;
		cout << "sou4 = " << sou4 << endl;
		cout << "sou5 = " << sou5 << endl;
		cout << "sou5r = " << sou5r << endl;
		cout << "sou6 = " << sou6 << endl;
		cout << "sou7 = " << sou7 << endl;
		cout << "sou8 = " << sou8 << endl;
		cout << "sou9 = " << sou9 << endl;

		MJPAI	pin1(MJPAI::PIN1, 0);
		MJPAI	pin2(MJPAI::PIN2, 0);
		MJPAI	pin3(MJPAI::PIN3, 0);
		MJPAI	pin4(MJPAI::PIN4, 0);
		MJPAI	pin5(MJPAI::PIN5, 0);
		MJPAI	pin5r(MJPAI::PIN5, 1);
		MJPAI	pin6(MJPAI::PIN6, 0);
		MJPAI	pin7(MJPAI::PIN7, 0);
		MJPAI	pin8(MJPAI::PIN8, 0);
		MJPAI	pin9(MJPAI::PIN9, 0);

		cout << "pin1 = " << pin1 << endl;
		cout << "pin2 = " << pin2 << endl;
		cout << "pin3 = " << pin3 << endl;
		cout << "pin4 = " << pin4 << endl;
		cout << "pin5 = " << pin5 << endl;
		cout << "pin5r = " << pin5r << endl;
		cout << "pin6 = " << pin6 << endl;
		cout << "pin7 = " << pin7 << endl;
		cout << "pin8 = " << pin8 << endl;
		cout << "pin9 = " << pin9 << endl;

		MJPAI	ton(MJPAI::TON, 0);
		MJPAI	nan(MJPAI::NAN, 0);
		MJPAI	sha(MJPAI::SHA, 0);
		MJPAI	pei(MJPAI::PEI, 0);
		MJPAI	haku(MJPAI::HAKU, 0);
		MJPAI	hatu(MJPAI::HATU, 0);
		MJPAI	chun(MJPAI::CHUN, 0);

		cout << "ton = " << ton << endl;
		cout << "nan = " << nan << endl;
		cout << "sha = " << sha << endl;
		cout << "pei = " << pei << endl;
		cout << "haku = " << haku << endl;
		cout << "hatu = " << hatu << endl;
		cout << "chun = " << chun << endl;

		{
			// コンストラクタ
			MJPAI	c1(MJPAI::PIN1, 0);
			MJPAI	c2(MJPAI::PIN5, 1);
			MJPAI	c3(MJPAI::PIN2 << 1);
			MJPAI	c4(MJPAI::MAX << 1);
			MJPAI	c5(pin3);
			MJPAI	c6(pin5r);

			cout << "c1(MJPAI::PIN1, 0) = " << c1 << endl;
			cout << "c2(MJPAI::PIN5, 1) = " << c2 << endl;
			cout << "c3(MJPAI::PIN2 << 1) = " << c3 << endl;
			cout << "c4(MJPAI::MAX << 1) = " << c4 << endl;
			cout << "c5(pin3) = " << c5 << endl;
			cout << "c6(pin5r) = " << c6 << endl;
		}
		{
			// 加算オペレータ（数字牌）
			MJPAI	a1, a2, a3, a4, a5, a6;

			a1 = sou1;
			a2 = sou1 + 2;
			a3 = sou1 + 1;
			a4 = sou1 + 0;
			a5 = sou1 + -1;
			a6 = sou1 + -2;

			cout << "a1(sou1) = " << a1 << endl;
			cout << "a2(sou1 + 2) = " << a2 << endl;
			cout << "a3(sou1 + 1) = " << a3 << endl;
			cout << "a4(sou1 + 0) = " << a4 << endl;
			cout << "a5(sou1 + -1) = " << a5 << endl;
			cout << "a6(sou1 + -2) = " << a6 << endl;

			a1++;
			a2 += 2;
			a3 += 1;
			a4 += 0;
			a5 += -1;
			a6 += -2;

			cout << "a1(++) = " << a1 << endl;
			cout << "a2(+= 2) = " << a2 << endl;
			cout << "a3(+= 1) = " << a3 << endl;
			cout << "a4(+= 0) = " << a4 << endl;
			cout << "a5(+= -1) = " << a5 << endl;
			cout << "a6(+= -2) = " << a6 << endl;
		}
		{
			// 減算オペレータ（数字牌）
			MJPAI	s1, s2, s3, s4, s5, s6;

			s1 = sou9;
			s2 = sou9 - 2;
			s3 = sou9 - 1;
			s4 = sou9 - 0;
			s5 = sou9 - -1;
			s6 = sou9 - -2;

			cout << "s1(sou9) = " << s1 << endl;
			cout << "s2(sou9 - 2) = " << s2 << endl;
			cout << "s3(sou9 - 1) = " << s3 << endl;
			cout << "s4(sou9 - 0) = " << s4 << endl;
			cout << "s5(sou9 - -1) = " << s5 << endl;
			cout << "s6(sou9 - -2) = " << s6 << endl;

			s1--;
			s2 -= 2;
			s3 -= 1;
			s4 -= 0;
			s5 -= -1;
			s6 -= -2;

			cout << "s1(--) = " << s1 << endl;
			cout << "s2(-= 2) = " << s2 << endl;
			cout << "s3(-= 1) = " << s3 << endl;
			cout << "s4(-= 0) = " << s4 << endl;
			cout << "s5(-= -1) = " << s5 << endl;
			cout << "s6(-= -2) = " << s6 << endl;
		}
		{
			// 加算オペレータ（風牌）
			MJPAI	a1, a2, a3, a4, a5, a6;

			a1 = ton;
			a2 = ton + 2;
			a3 = ton + 1;
			a4 = ton + 0;
			a5 = ton + -1;
			a6 = ton + -2;

			cout << "a1(ton) = " << a1 << endl;
			cout << "a2(ton + 2) = " << a2 << endl;
			cout << "a3(ton + 1) = " << a3 << endl;
			cout << "a4(ton + 0) = " << a4 << endl;
			cout << "a5(ton + -1) = " << a5 << endl;
			cout << "a6(ton + -2) = " << a6 << endl;

			a1++;
			a2 += 2;
			a3 += 1;
			a4 += 0;
			a5 += -1;
			a6 += -2;

			cout << "a1(++) = " << a1 << endl;
			cout << "a2(+= 2) = " << a2 << endl;
			cout << "a3(+= 1) = " << a3 << endl;
			cout << "a4(+= 0) = " << a4 << endl;
			cout << "a5(+= -1) = " << a5 << endl;
			cout << "a6(+= -2) = " << a6 << endl;
		}
		{
			// 減算オペレータ（風牌）
			MJPAI	s1, s2, s3, s4, s5, s6;

			s1 = pei;
			s2 = pei - 2;
			s3 = pei - 1;
			s4 = pei - 0;
			s5 = pei - -1;
			s6 = pei - -2;

			cout << "s1(pei) = " << s1 << endl;
			cout << "s2(pei - 2) = " << s2 << endl;
			cout << "s3(pei - 1) = " << s3 << endl;
			cout << "s4(pei - 0) = " << s4 << endl;
			cout << "s5(pei - -1) = " << s5 << endl;
			cout << "s6(pei - -2) = " << s6 << endl;

			s1--;
			s2 -= 2;
			s3 -= 1;
			s4 -= 0;
			s5 -= -1;
			s6 -= -2;

			cout << "s1(--) = " << s1 << endl;
			cout << "s2(-= 2) = " << s2 << endl;
			cout << "s3(-= 1) = " << s3 << endl;
			cout << "s4(-= 0) = " << s4 << endl;
			cout << "s5(-= -1) = " << s5 << endl;
			cout << "s6(-= -2) = " << s6 << endl;
		}
		{
			// 加算オペレータ（三元牌）
			MJPAI	a1, a2, a3, a4, a5, a6;

			a1 = haku;
			a2 = haku + 2;
			a3 = haku + 1;
			a4 = haku + 0;
			a5 = haku + -1;
			a6 = haku + -2;

			cout << "a1(haku) = " << a1 << endl;
			cout << "a2(haku + 2) = " << a2 << endl;
			cout << "a3(haku + 1) = " << a3 << endl;
			cout << "a4(haku + 0) = " << a4 << endl;
			cout << "a5(haku + -1) = " << a5 << endl;
			cout << "a6(haku + -2) = " << a6 << endl;

			a1++;
			a2 += 2;
			a3 += 1;
			a4 += 0;
			a5 += -1;
			a6 += -2;

			cout << "a1(++) = " << a1 << endl;
			cout << "a2(+= 2) = " << a2 << endl;
			cout << "a3(+= 1) = " << a3 << endl;
			cout << "a4(+= 0) = " << a4 << endl;
			cout << "a5(+= -1) = " << a5 << endl;
			cout << "a6(+= -2) = " << a6 << endl;
		}
		{
			// 減算オペレータ（三元牌）
			MJPAI	s1, s2, s3, s4, s5, s6;

			s1 = chun;
			s2 = chun - 2;
			s3 = chun - 1;
			s4 = chun - 0;
			s5 = chun - -1;
			s6 = chun - -2;

			cout << "s1(chun) = " << s1 << endl;
			cout << "s2(chun - 2) = " << s2 << endl;
			cout << "s3(chun - 1) = " << s3 << endl;
			cout << "s4(chun - 0) = " << s4 << endl;
			cout << "s5(chun - -1) = " << s5 << endl;
			cout << "s6(chun - -2) = " << s6 << endl;

			s1--;
			s2 -= 2;
			s3 -= 1;
			s4 -= 0;
			s5 -= -1;
			s6 -= -2;

			cout << "s1(--) = " << s1 << endl;
			cout << "s2(-= 2) = " << s2 << endl;
			cout << "s3(-= 1) = " << s3 << endl;
			cout << "s4(-= 0) = " << s4 << endl;
			cout << "s5(-= -1) = " << s5 << endl;
			cout << "s6(-= -2) = " << s6 << endl;
		}
		{
			// 比較オペレータ（その１）
			cout << "(man1 == man2) = " << (man1 == man2) << endl;
			cout << "(man1 != man2) = " << (man1 != man2) << endl;
			cout << "(man1 <  man2) = " << (man1 <  man2) << endl;
			cout << "(man1 <= man2) = " << (man1 <= man2) << endl;
			cout << "(man1 >  man2) = " << (man1 >  man2) << endl;
			cout << "(man1 >= man2) = " << (man1 >= man2) << endl;
		}
		{
			// 比較オペレータ（その２）
			cout << "(man1 + 1 == man2) = " << (man1 + 1 == man2) << endl;
			cout << "(man1 + 1 != man2) = " << (man1 + 1 != man2) << endl;
			cout << "(man1 + 1 <  man2) = " << (man1 + 1 <  man2) << endl;
			cout << "(man1 + 1 <= man2) = " << (man1 + 1 <= man2) << endl;
			cout << "(man1 + 1 >  man2) = " << (man1 + 1 >  man2) << endl;
			cout << "(man1 + 1 >= man2) = " << (man1 + 1 >= man2) << endl;
		}
		{
			// 比較オペレータ（その３）
			cout << "(man1 + 2 == man2) = " << (man1 + 2 == man2) << endl;
			cout << "(man1 + 2 != man2) = " << (man1 + 2 != man2) << endl;
			cout << "(man1 + 2 <  man2) = " << (man1 + 2 <  man2) << endl;
			cout << "(man1 + 2 <= man2) = " << (man1 + 2 <= man2) << endl;
			cout << "(man1 + 2 >  man2) = " << (man1 + 2 >  man2) << endl;
			cout << "(man1 + 2 >= man2) = " << (man1 + 2 >= man2) << endl;
		}

		cerr << "# MJPAIARRAY TEST #" << endl;
		{
			// コンストラクタ
			MJPAIARRAY	c1;
			MJPAIARRAY	c2(pin1);
			MJPAIARRAY	c3(c2);

			cout << "c1() = " << c1 << endl;
			cout << "c2(pin1) = " << c2 << endl;
			cout << "c3(c2) = " << c3 << endl;
		}
		{
			// 代入オペレータ
			MJPAIARRAY	a1, a2, a3;

			a1 = a2;
			a2 = MJPAIARRAY() + pin1 + pin2 + pin3;
			a3 = a2;

			cout << "a1(a2) = " << a1 << endl;
			cout << "a2(MJPAIARRAY() + pin1 + pin2 + pin3) = " << a2 << endl;
			cout << "a3(a2) = " << a3 << endl;
			cout << "a3[0] = " << a3[0] << endl;
			cout << "a3[1] = " << a3[1] << endl;
			cout << "a3[2] = " << a3[2] << endl;
		}
		{
			// 加算オペレータ
			MJPAIARRAY	a1, a2, a3;

			a1 = MJPAIARRAY() + pin1 + pin2 + pin3 + pin4 + pin5;
			a2 = MJPAIARRAY() + pin1 + pin2 + pin3;
			a3 = a1 + a2;

			cout << "a1(MJPAIARRAY() + pin1 + pin2 + pin3 + pin4 + pin5) = " << a1 << endl;
			cout << "a2(MJPAIARRAY() + pin1 + pin2 + pin3) = " << a2 << endl;
			cout << "a3(a1 + a2) = " << a3 << endl;

			a3.Sort();

			cout << "Sort(a3) = " << a3 << endl;
		}
		{
			// 加算オペレータ
			MJPAIARRAY	a1, a2, a3;

			a1 = MJPAIARRAY() + pin3 + pin4 + pin5;
			a2 = a1;

			cout << "a1(MJPAIARRAY() + pin3 + pin4 + pin5) = " << a1 << endl;
			cout << "a2(a1) = " << a2 << endl;

			a1 += pin1;
			a1 += pin2;
			a1 += pin3;
			a2 += MJPAIARRAY() + pin1 + pin2 + pin3;

			cout << "a1(+= pin1 + pin2 + pin3) = " << a1 << endl;
			cout << "a2(+= MJPAIARRAY() + pin1 + pin2 + pin3) = " << a2 << endl;

			a2.Sort();

			cout << "Sort(a2) = " << a2 << endl;
		}
		{
			// 比較オペレータ（その１）
			MJPAIARRAY	array1(MJPAIARRAY() + man1 + man2);
			MJPAIARRAY	array2(MJPAIARRAY() + man1 + man1);

			cout << "array1(MJPAIARRAY() + man1 + man2) = " << array1 << endl;
			cout << "array2(MJPAIARRAY() + man1 + man1) = " << array2 << endl;
			cout << "(array1 == array2) = " << (array1 == array2) << endl;
			cout << "(array1 != array2) = " << (array1 != array2) << endl;
			cout << "(array1 <  array2) = " << (array1 <  array2) << endl;
			cout << "(array1 <= array2) = " << (array1 <= array2) << endl;
			cout << "(array1 >  array2) = " << (array1 >  array2) << endl;
			cout << "(array1 >= array2) = " << (array1 >= array2) << endl;
		}
		{
			// 比較オペレータ（その２）
			MJPAIARRAY	array1(MJPAIARRAY() + man1 + man2);
			MJPAIARRAY	array2(MJPAIARRAY() + man1 + man2);

			cout << "array1(MJPAIARRAY() + man1 + man2) = " << array1 << endl;
			cout << "array2(MJPAIARRAY() + man1 + man2) = " << array2 << endl;
			cout << "(array1 == array2) = " << (array1 == array2) << endl;
			cout << "(array1 != array2) = " << (array1 != array2) << endl;
			cout << "(array1 <  array2) = " << (array1 <  array2) << endl;
			cout << "(array1 <= array2) = " << (array1 <= array2) << endl;
			cout << "(array1 >  array2) = " << (array1 >  array2) << endl;
			cout << "(array1 >= array2) = " << (array1 >= array2) << endl;
		}
		{
			// 比較オペレータ（その３）
			MJPAIARRAY	array1(MJPAIARRAY() + man1 + man2);
			MJPAIARRAY	array2(MJPAIARRAY() + man2 + man1);

			cout << "array1(MJPAIARRAY() + man1 + man2) = " << array1 << endl;
			cout << "array2(MJPAIARRAY() + man2 + man1) = " << array2 << endl;
			cout << "(array1 == array2) = " << (array1 == array2) << endl;
			cout << "(array1 != array2) = " << (array1 != array2) << endl;
			cout << "(array1 <  array2) = " << (array1 <  array2) << endl;
			cout << "(array1 <= array2) = " << (array1 <= array2) << endl;
			cout << "(array1 >  array2) = " << (array1 >  array2) << endl;
			cout << "(array1 >= array2) = " << (array1 >= array2) << endl;
		}
		{
			// 比較オペレータ（その４）
			MJPAIARRAY	array1(MJPAIARRAY() + man1 + man2);
			MJPAIARRAY	array2(MJPAIARRAY() + man1 + man3);

			cout << "array1(MJPAIARRAY() + man1 + man2) = " << array1 << endl;
			cout << "array2(MJPAIARRAY() + man1 + man3) = " << array2 << endl;
			cout << "(array1 == array2) = " << (array1 == array2) << endl;
			cout << "(array1 != array2) = " << (array1 != array2) << endl;
			cout << "(array1 <  array2) = " << (array1 <  array2) << endl;
			cout << "(array1 <= array2) = " << (array1 <= array2) << endl;
			cout << "(array1 >  array2) = " << (array1 >  array2) << endl;
			cout << "(array1 >= array2) = " << (array1 >= array2) << endl;
		}

		cerr << "# MJPAICOUNT TEST #" << endl;
		{
			// コンストラクタ
			MJPAICOUNT	c1;
			MJPAICOUNT	c2(man1);
			MJPAICOUNT	c3(MJPAIARRAY() + man1 + man1 + man2 + man3 + man4);
			MJPAICOUNT	c4(c3);

			cout << "c1() = " << endl << "  " << c1 << endl;
			cout << "c2(man1) = " << endl << "  "  << c2 << endl;
			cout << "c3(MJPAIARRAY() + man1 + man1 + man2 + man3 + man4) = " << endl << "  "  << c3 << endl;
			cout << "c4(c3) = " << endl << "  "  << c4 << endl;
		}
		{
			// 代入オペレータ
			MJPAICOUNT	a1, a2, a3;

			a1 = man1;
			a2 = MJPAIARRAY() + man1 + man2 + man3;
			a3 = a1;

			cout << "a1(man1) = " << endl << "  "  << a1 << endl;
			cout << "a2(MJPAIARRAY() + man1 + man2 + man3) = " << endl << "  "  << a2 << endl;
			cout << "a3(a1) = " << endl << "  "  << a3 << endl;
		}
		{
			// 加算オペレータ
			MJPAICOUNT	a1, a2, a3;

			cout << "a1() = " << endl << "  "  << a1 << endl;
			cout << "a2() = " << endl << "  "  << a2 << endl;
			cout << "a3() = " << endl << "  "  << a3 << endl;

			a1 += man1;
			a2 += MJPAIARRAY() + man1 + man2 + man3;
			a3 += a1;

			cout << "a1(+= man1) = " << endl << "  "  << a1 << endl;
			cout << "a2(+= MJPAIARRAY() + man1 + man2 + man3) = " << endl << "  "  << a2 << endl;
			cout << "a3(+= a1) = " << endl << "  "  << a3 << endl;
		}
		{
			// 減算オペレータ
			MJPAICOUNT	s1(MJPAIARRAY() + man1 + man1 + man2 + man3 + man4);
			MJPAICOUNT	s2(s1);
			MJPAICOUNT	s3(s1);

			cout << "s1(MJPAIARRAY() + man1 + man1 + man2 + man3 + man4) = " << endl << "  "  << s1 << endl;
			cout << "s2(s1) = " << endl << "  "  << s2 << endl;
			cout << "s3(s1) = " << endl << "  "  << s3 << endl;

			s1 -= man1;
			s2 -= MJPAIARRAY() + man1 + man1 + man1;
			s3 -= s1;

			cout << "s1(-= man1) = " << endl << "  "  << s1 << endl;
			cout << "s2(-= MJPAIARRAY() + man1 + man1 + man1) = " << endl << "  "  << s2 << endl;
			cout << "s3(-= s1) = " << endl << "  "  << s3 << endl;
		}
		{
			// その他の演算
			MJPAICOUNT	a1(MJPAIARRAY() + ton + ton + ton + nan);
			MJPAICOUNT	a2(MJPAIARRAY() + ton + nan + nan + nan);
			MJPAICOUNT	a3, a4, a5;

			cout << "a1(MJPAIARRAY() + ton + ton + ton + nan) = " << endl << "  " << a1 << endl;
			cout << "a2(MJPAIARRAY() + ton + nan + nan + nan) = " << endl << "  "  << a2 << endl;

			a3 = a1 + a2;
			a4 = a1;
			a4.Union(a2);
			a5 = a1;
			a5.Intersect(a2);

			cout << "a3(a1 + a2) = " << endl << "  "  << a3 << endl;
			cout << "a4(a1 Union a2) = " << endl << "  "  << a4 << endl;
			cout << "a5(a1 Intersect a2) = " << endl << "  "  << a5 << endl;
			cout << "(a1 Include a1) = " << a1.Include(a1) << endl;
			cout << "(a1 Include a2) = " << a1.Include(a2) << endl;
			cout << "(a2 Include a1) = " << a2.Include(a1) << endl;
			cout << "(a2 Include a2) = " << a2.Include(a2) << endl;
			cout << "(a1 Include a4) = " << a1.Include(a4) << endl;
			cout << "(a1 Include a5) = " << a1.Include(a5) << endl;
		}
	}
#endif

#if 1
	//
	// コンソールゲーム開始
	//
	MJGAMEMASTER			*pGame = new MJGAMEMASTER(MJRULE("JapanProfessionalMahjongLeagueRuleA"));
	vector<MJINTERFACE *>	Interface;

	Interface.resize(4, NULL);
	Interface[MJ::P1] = new MJINTERFACE_DEBUG_CUI(string("P1"), true, true);
	Interface[MJ::P2] = new MJINTERFACE_DEBUG_CUI(string("P2"), true, true);
	Interface[MJ::P3] = new MJINTERFACE_DEBUG_CUI(string("P3"), true, true);
	Interface[MJ::P4] = new MJINTERFACE_DEBUG_CUI(string("P4"), true, true);

	pGame->Start(Interface, 0x00000000);
	while (pGame->Server() == MJ::RET_BUSY) {
		Interface[MJ::P1]->Server();
		Interface[MJ::P2]->Server();
		Interface[MJ::P3]->Server();
		Interface[MJ::P4]->Server();
	}

	SAFE_DELETE(Interface[MJ::P4]);
	SAFE_DELETE(Interface[MJ::P3]);
	SAFE_DELETE(Interface[MJ::P2]);
	SAFE_DELETE(Interface[MJ::P1]);
	Interface.clear();
	SAFE_DELETE(pGame);
#endif

	cerr << "end" << endl;

	string	cmd;

	cin >> cmd;

	return 0;

} /* end of main */
