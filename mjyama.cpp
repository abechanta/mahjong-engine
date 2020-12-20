#include "mjyama.h"

//----------------------------------------------------------------------------
MJYAMA::MJYAMA()
{
} /* end of MJYAMA::MJYAMA */



MJYAMA::~MJYAMA()
{
} /* end of MJYAMA::~MJYAMA */



//----------------------------------------------------------------------------
MJPAIARRAY	MJYAMA::Reset(const MJPAIARRAY &PaiSet, size_t WampaiCount)
{
	MJPAIARRAY	Wampai;

	m_Yama.clear();
	m_Yama = PaiSet;
	m_Yama |= MJPAI::CLOSED;

	for (size_t ii = 0; ii < WampaiCount; ii += 2) {
		MJPAI	lower, upper;

		lower = m_Yama.back();
		m_Yama.pop_back();
		upper = m_Yama.back();
		m_Yama.pop_back();
		Wampai.push_back(upper);
		Wampai.push_back(lower);
	}
	return Wampai;

} /* end of MJYAMA::Reset */



bool	MJYAMA::IsEmpty() const
{
	return m_Yama.empty();

} /* end of MJYAMA::IsEmpty */



size_t	MJYAMA::GetLeft() const
{
	return static_cast<size_t>(m_Yama.size());

} /* end of MJYAMA::GetLeft */



//----------------------------------------------------------------------------
MJPAI	MJYAMA::PopTumo()
{
	MJASSERT(m_Yama.size() > 0);

	MJPAI	Pai;

	Pai = m_Yama.front();
	m_Yama.pop_front();
	return Pai &= ~MJPAI::CLOSED;

} /* end of MJYAMA::PopTumo */



MJPAI	MJYAMA::PopTail()
{
	MJASSERT(m_Yama.size() > 0);

	MJPAI	Pai;

	Pai = m_Yama.back();
	m_Yama.pop_back();
	return Pai;

} /* end of MJYAMA::PopTail */



//----------------------------------------------------------------------------
