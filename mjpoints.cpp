#include "mjpoints.h"

//----------------------------------------------------------------------------
MJPOINTS::MJPOINTS()
{
} /* end of MJPOINTS::MJPOINTS */



MJPOINTS::~MJPOINTS()
{
} /* end of MJPOINTS::~MJPOINTS */



//----------------------------------------------------------------------------
void	MJPOINTS::Reset(ssize_t Pts)
{
	m_Point = Pts;
	m_Pool = 0;
	m_Chip = 0;

} /* end of MJPOINTS::Reset */



ssize_t	MJPOINTS::GetPoint() const
{
	return m_Point;

} /* end of MJPOINTS::GetPoint */



ssize_t	MJPOINTS::GetChip() const
{
	return m_Chip;

} /* end of MJPOINTS::GetChip */



//----------------------------------------------------------------------------
void	MJPOINTS::AddPool(ssize_t Pts)
{
	m_Point -= Pts;
	m_Pool += Pts;

} /* end of MJPOINTS::AddPool */



void	MJPOINTS::RefundPool()
{
	m_Point += m_Pool;
	m_Pool = 0;

} /* end of MJPOINTS::RefundPool */



ssize_t	MJPOINTS::MovePool()
{
	size_t	Pts = m_Pool;

	m_Pool = 0;
	return Pts;

} /* end of MJPOINTS::MovePool */



//----------------------------------------------------------------------------
