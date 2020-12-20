#include "mjkanplayer.h"

//----------------------------------------------------------------------------
MJKANPLAYER::MJKANPLAYER()
{
} /* end of MJKANPLAYER::MJKANPLAYER */



MJKANPLAYER::~MJKANPLAYER()
{
} /* end of MJKANPLAYER::~MJKANPLAYER */



//----------------------------------------------------------------------------
void	MJKANPLAYER::Reset(size_t KanCount)
{
	m_KanCount = KanCount;
	m_KanPid.clear();
	m_bSukantsuAvailable = true;

} /* end of MJKANPLAYER::Reset */



bool	MJKANPLAYER::IsAvailable() const
{
	return m_KanPid.size() < m_KanCount;

} /* end of MJKANPLAYER::IsAvailable */



bool	MJKANPLAYER::IsAvailableSukantsu() const
{
	return m_bSukantsuAvailable;

} /* end of MJKANPLAYER::IsAvailableSukantsu */



//----------------------------------------------------------------------------
void	MJKANPLAYER::Add(size_t Pid)
{
	MJASSERT(m_KanPid.size() < m_KanCount);

	m_KanPid.push_back(Pid);
	if (m_bSukantsuAvailable) {
		m_bSukantsuAvailable = (m_KanPid[0] == Pid);
	}

} /* end of MJKANPLAYER::Add */



//----------------------------------------------------------------------------
