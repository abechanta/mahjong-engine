#include "mjwinplayer.h"

//----------------------------------------------------------------------------
MJWINPLAYER::MJWINPLAYER()
{
} /* end of MJWINPLAYER::MJWINPLAYER */



MJWINPLAYER::~MJWINPLAYER()
{
} /* end of MJWINPLAYER::~MJWINPLAYER */



//----------------------------------------------------------------------------
void	MJWINPLAYER::Reset()
{
	m_WinPid.clear();

} /* end of MJWINPLAYER::Reset */



size_t	MJWINPLAYER::GetWinnerCount() const
{
	return static_cast<size_t>(m_WinPid.size());

} /* end of MJWINPLAYER::GetWinnerCount */



//----------------------------------------------------------------------------
void	MJWINPLAYER::Add(size_t Pid, size_t OpponentPid)
{
	m_WinPid.push_back(WINNER(Pid, OpponentPid));

} /* end of MJWINPLAYER::Add */



//----------------------------------------------------------------------------
