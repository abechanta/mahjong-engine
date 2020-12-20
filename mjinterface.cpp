#include "mjinterface.h"
using namespace std;

//----------------------------------------------------------------------------
MJINTERFACE::MJINTERFACE(const string &Name)
	: m_Name(Name)
{
} /* end of MJINTERFACE::MJINTERFACE */



MJINTERFACE::~MJINTERFACE()
{
} /* end of MJINTERFACE::~MJINTERFACE */



//----------------------------------------------------------------------------
ssize_t	MJINTERFACE::SendEvent(const MJEVENT &Event)
{
	return m_Plug.Send(Event);

} /* end of MJINTERFACE::SendEvent */



ssize_t	MJINTERFACE::Server()
{
	MJEVENT	RecvEvent;

	if (m_Plug.Recv(RecvEvent) != RET_DONE) {
		MJBREAK;
		return RET_BUSY;
	}

	return Select(RecvEvent);

} /* end of MJINTERFACE::Server */
