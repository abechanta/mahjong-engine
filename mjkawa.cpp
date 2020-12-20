#include "mjkawa.h"

//----------------------------------------------------------------------------
MJKAWA::MJKAWA()
{
} /* end of MJKAWA::MJKAWA */



MJKAWA::~MJKAWA()
{
} /* end of MJKAWA::~MJKAWA */



//----------------------------------------------------------------------------
void	MJKAWA::Reset()
{
	m_Kawa.clear();

} /* end of MJKAWA::Reset */



bool	MJKAWA::IsEmpty() const
{
	return m_Kawa.empty();

} /* end of MJKAWA::IsEmpty */



//----------------------------------------------------------------------------
void	MJKAWA::PushSute(const MJPAI &Sute)
{
	MJPAI	Pai;

	Pai = Sute;
	Pai &= ~MJPAI::CLOSED;
	m_Kawa.push_back(Pai);

} /* end of MJKAWA::PushSute */



void	MJKAWA::HideTail()
{
	MJASSERT(m_Kawa.size() > 0);

	m_Kawa.back() |= MJPAI::LOST;

} /* end of MJKAWA::HideTail */



//----------------------------------------------------------------------------
