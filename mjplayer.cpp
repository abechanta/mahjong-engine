#include "mjplayer.h"
using namespace std;

//----------------------------------------------------------------------------
MJPLAYER::MJPLAYER(const string &Name)
{
	m_Name = Name;

} /* end of MJPLAYER::MJPLAYER */



MJPLAYER::~MJPLAYER()
{
} /* end of MJPLAYER::~MJPLAYER */



//----------------------------------------------------------------------------
void	MJPLAYER::Reset(const MJRULE &Rule, size_t Pno)
{
	m_Rule = Rule;
	m_Pno = Pno;
	m_Points.Reset(m_Rule.m_Pts.m_StartPoint);

} /* end of MJPLAYER::Reset */



void	MJPLAYER::ResetRound(size_t Pid)
{
	m_Hand.Reset();
	m_Kawa.Reset();
	m_Pid = Pid;
	m_bOya = (Pid == MJ::TON);
	m_Shanten = 10;
	m_bFuriten = false;
	m_bFuritenNow = false;
	m_bReach = false;
	m_bReachNow = false;
	m_bAgari = false;

} /* end of MJPLAYER::ResetRound */



//----------------------------------------------------------------------------
