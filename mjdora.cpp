#include "mjdora.h"

//----------------------------------------------------------------------------
MJDORA::MJDORA()
{
} /* end of MJDORA::MJDORA */



MJDORA::~MJDORA()
{
} /* end of MJDORA::~MJDORA */



//----------------------------------------------------------------------------
void	MJDORA::Reset()
{
	m_Disp.clear();
	m_Dora.clear();

} /* end of MJDORA::Reset */



MJPAIARRAY	MJDORA::GetDora() const
{
	return m_Dora;

} /* end of MJDORA::GetDora */



bool	MJDORA::IsDora(const MJPAI &Pai) const
{
	for (MJPAIARRAY::const_iterator it = m_Dora.begin(); it != m_Dora.end(); ++it) {
		if (*it + 1 == Pai) {	// FIXME: 赤でも比較に失敗しないようにする
			return true;
		}
	}
	return false;

} /* end of MJDORA::IsDora */



size_t	MJDORA::CountDora(const MJPAIARRAY &Pai) const
{
	size_t	Count = 0;

	for (MJPAIARRAY::const_iterator it = Pai.begin(); it != Pai.end(); ++it) {
		if (IsDora(*it)) {
			Count++;
		}
	}
	return Count;

} /* end of MJDORA::CountDora */



//----------------------------------------------------------------------------
void	MJDORA::Add(const MJPAI &Pai)
{
	MJPAI	Disp = Pai;

	Disp &= ~MJPAI::CLOSED;
	m_Disp.push_back(Disp + 0);
	m_Dora.push_back(Disp + 1);

} /* end of MJDORA::Add */



void	MJDORA::Add(const MJPAIARRAY &Pai)
{
	for (MJPAIARRAY::const_iterator it = Pai.begin(); it != Pai.end(); ++it) {
		Add(*it);
	}

} /* end of MJDORA::Add */



//----------------------------------------------------------------------------
