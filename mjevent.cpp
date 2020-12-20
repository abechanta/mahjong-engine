#include "mjevent.h"

//----------------------------------------------------------------------------
MJEVENT::MJEVENT(size_t Action, size_t ActionMask, size_t Pid, size_t OpponentPid, const MJPAI &Pai, const MJPAIARRAY &PaiArray)
	: m_Action(Action),
	m_ActionMask(ActionMask),
	m_Pid(Pid),
	m_OpponentPid(OpponentPid),
	m_Pai(Pai),
	m_PaiArray(PaiArray)
{
} /* end of MJEVENT::MJEVENT */



MJEVENT	&MJEVENT::operator |=(size_t Mask)
{
	m_ActionMask |= Mask;
	return *this;

} /* end of MJEVENT::operator |= */



MJEVENT	&MJEVENT::operator &=(size_t Mask)
{
	m_ActionMask &= Mask;
	return *this;

} /* end of MJEVENT::operator &= */



MJEVENT	&MJEVENT::Shade()
{
	m_Pai.Shade();
	m_PaiArray.Shade();
	return *this;

} /* end of MJEVENT::Shade */



//----------------------------------------------------------------------------
