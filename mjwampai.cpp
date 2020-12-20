#include "mjwampai.h"

//----------------------------------------------------------------------------
MJWAMPAI::MJWAMPAI()
{
} /* end of MJWAMPAI::MJWAMPAI */



MJWAMPAI::~MJWAMPAI()
{
} /* end of MJWAMPAI::~MJWAMPAI */



//----------------------------------------------------------------------------
void	MJWAMPAI::Reset(const MJPAIARRAY &PaiSet, size_t RinshanCount)
{
	m_Wampai.clear();
	m_Wampai = PaiSet;
	m_Wampai |= MJPAI::CLOSED;
	m_RinshanLeft = RinshanCount;
	m_DoraOffset = (RinshanCount + 1) & ~0x01;

} /* end of MJWAMPAI::Reset */



size_t	MJWAMPAI::GetLeft() const
{
	return static_cast<size_t>(m_Wampai.size());

} /* end of MJWAMPAI::GetLeft */



//----------------------------------------------------------------------------
MJPAI	MJWAMPAI::OpenDora()
{
	MJASSERT(m_DoraOffset < m_Wampai.size());

	MJPAI	Dora;

	m_Wampai[m_DoraOffset] &= ~MJPAI::CLOSED;
	Dora = m_Wampai[m_DoraOffset];
	m_DoraOffset += 2;
	return Dora;

} /* end of MJWAMPAI::OpenDora */



MJPAIARRAY	MJWAMPAI::OpenUraDora()
{
	MJPAIARRAY	Dora;
	bool	bOpen = false;

	for (MJPAIARRAY::iterator it = m_Wampai.begin(); it != m_Wampai.end(); ++it) {
		if (bOpen) {
			// 開示されていた牌の次の牌は裏ドラ
			Dora.push_back((*it) &= ~MJPAI::CLOSED);
			bOpen = false;
		} else {
			bOpen = !it->GetOption(MJPAI::CLOSED);
		}
	}
	return Dora;

} /* end of MJWAMPAI::OpenUraDora */



MJPAI	MJWAMPAI::PopRinshan()
{
	MJASSERT(m_Wampai.size() > 0);
	MJASSERT(m_RinshanLeft > 0);
	MJASSERT(m_DoraOffset > 0);

	MJPAI	Pai;

	Pai = m_Wampai.front();
	m_Wampai.pop_front();

	m_RinshanLeft--;
	m_DoraOffset--;
	return Pai &= ~MJPAI::CLOSED;

} /* end of MJWAMPAI::PopRinshan */



void	MJWAMPAI::PushTail(const MJPAI &Pai)
{
	MJPAI	Tail = Pai;

	Tail |= MJPAI::CLOSED;
	m_Wampai.push_back(Tail);

} /* end of MJWAMPAI::PushTail */



//----------------------------------------------------------------------------
