#include "mjpai.h"
#include <string>
using namespace std;

//----------------------------------------------------------------------------
MJPAI::MJPAI(uint8_t Face, uint8_t Red, uint8_t Option, uint8_t Mark)
	: m_Face(Face),
	m_Red(Red),
	m_Option(Option),
	m_Mark(Mark)
{
} /* end of MJPAI::MJPAI */



MJPAI::MJPAI(uint8_t Code)
	: m_Face((Code >> 1) & 0x7f),
	m_Red((Code >> 0) & 0x01),
	m_Option(0),
	m_Mark(0)
{
	if (
		(Code == 0xFF) ||
		(m_Face >= FACE::MAX)
	) {
		m_Face	= FACE::UNKNOWN;
		m_Red	= 0;
	}

} /* end of MJPAI::MJPAI */



MJPAI::MJPAI(const MJPAI &Other)
	: m_Face(Other.m_Face),
	m_Red(Other.m_Red),
	m_Option(Other.m_Option),
	m_Mark(Other.m_Mark)
{
} /* end of MJPAI::MJPAI */



//----------------------------------------------------------------------------
size_t	MJPAI::GetCode(bool bFaceOnly) const
{
	if (!IsValid()) {
		return 0xFF;
	}
	return (m_Face << 1) | ((bFaceOnly ? 0 : m_Red) << 0);

} /* end of MJPAI::GetCode */



MJPAI	&MJPAI::operator =(const MJPAI &Other)
{
	m_Face		= Other.m_Face;
	m_Red		= Other.m_Red;
	m_Option	= Other.m_Option;
	m_Mark		= Other.m_Mark;
	return *this;

} /* end of MJPAI::operator = */



void	MJPAI::AddFace(ssize_t Delta)
{
	switch (GetSuit()) {
	case MANZU:
		if (Delta < 0) {
			Delta = -Delta;
			Delta = 9 - (Delta % 9);
		}
		m_Face = FACE::MAN1 + ((GetNumber() - 1 + Delta) % 9);
		break;
	case SOUZU:
		if (Delta < 0) {
			Delta = -Delta;
			Delta = 9 - (Delta % 9);
		}
		m_Face = FACE::SOU1 + ((GetNumber() - 1 + Delta) % 9);
		break;
	case PINZU:
		if (Delta < 0) {
			Delta = -Delta;
			Delta = 9 - (Delta % 9);
		}
		m_Face = FACE::PIN1 + ((GetNumber() - 1 + Delta) % 9);
		break;
	case KAZE:
		if (Delta < 0) {
			Delta = -Delta;
			Delta = 4 - (Delta % 4);
		}
		m_Face = FACE::TON + ((GetNumber() - 1 + Delta) % 4);
		break;
	case SANGEN:
		if (Delta < 0) {
			Delta = -Delta;
			Delta = 3 - (Delta % 3);
		}
		m_Face = FACE::HAKU + ((GetNumber() - 1 + Delta) % 3);
		break;
	default:
		break;
	} /* endswitch GetSuit() */

} /* end of MJPAI::AddFace */



MJPAI	MJPAI::operator +(int Other) const
{
	MJPAI	ret = *this;

	ret.AddFace(Other);
	return ret;

} /* end of MJPAI::operator + */



MJPAI	MJPAI::operator -(int Other) const
{
	MJPAI	ret = *this;

	ret.AddFace(-Other);
	return ret;

} /* end of MJPAI::operator - */



MJPAI	&MJPAI::operator +=(int Other)
{
	AddFace(Other);
	return *this;

} /* end of MJPAI::operator += */



MJPAI	&MJPAI::operator -=(int Other)
{
	AddFace(-Other);
	return *this;

} /* end of MJPAI::operator -= */



MJPAI	&MJPAI::operator |=(size_t Other)
{
	m_Option |= Other;
	return *this;

} /* end of MJPAI::operator |= */



MJPAI	&MJPAI::operator &=(size_t Other)
{
	m_Option &= Other;
	return *this;

} /* end of MJPAI::operator &= */



//----------------------------------------------------------------------------
bool	MJPAI::operator ==(const MJPAI &Other) const
{
	return GetCode(true) == Other.GetCode(true);

} /* end of MJPAI::operator == */



bool	MJPAI::operator !=(const MJPAI &Other) const
{
	return GetCode(true) != Other.GetCode(true);

} /* end of MJPAI::operator != */



bool	MJPAI::operator <(const MJPAI &Other) const
{
	return GetCode(false) < Other.GetCode(false);

} /* end of MJPAI::operator < */



bool	MJPAI::operator <=(const MJPAI &Other) const
{
	return GetCode(false) <= Other.GetCode(false);

} /* end of MJPAI::operator <= */



bool	MJPAI::operator >(const MJPAI &Other) const
{
	return GetCode(false) > Other.GetCode(false);

} /* end of MJPAI::operator > */



bool	MJPAI::operator >=(const MJPAI &Other) const
{
	return GetCode(false) >= Other.GetCode(false);

} /* end of MJPAI::operator >= */



//----------------------------------------------------------------------------
bool	MJPAI::IsValid() const
{
	return (
		(FACE::UNKNOWN < m_Face) && (m_Face < FACE::MAX)
	);

} /* end of MJPAI::IsValid */



bool	MJPAI::IsKazuhai() const
{
	return (
		(FACE::MAN1 <= m_Face) && (m_Face <= FACE::MAN9) ||
		(FACE::SOU1 <= m_Face) && (m_Face <= FACE::SOU9) ||
		(FACE::PIN1 <= m_Face) && (m_Face <= FACE::PIN9)
	);

} /* end of MJPAI::IsKazuhai */



bool	MJPAI::IsJihai() const
{
	return (
		(FACE::TON <= m_Face) && (m_Face <= FACE::PEI) ||
		(FACE::HAKU <= m_Face) && (m_Face <= FACE::CHUN)
	);

} /* end of MJPAI::IsJihai */



bool	MJPAI::IsChuchan() const
{
	return (
		(FACE::MAN1 < m_Face) && (m_Face < FACE::MAN9) ||
		(FACE::SOU1 < m_Face) && (m_Face < FACE::SOU9) ||
		(FACE::PIN1 < m_Face) && (m_Face < FACE::PIN9)
	);

} /* end of MJPAI::IsChuchan */



bool	MJPAI::IsYaochu() const
{
	return (
		(m_Face == FACE::MAN1) ||
		(m_Face == FACE::MAN9) ||
		(m_Face == FACE::SOU1) ||
		(m_Face == FACE::SOU9) ||
		(m_Face == FACE::PIN1) ||
		(m_Face == FACE::PIN9) ||
		(FACE::TON <= m_Face) && (m_Face <= FACE::PEI) ||
		(FACE::HAKU <= m_Face) && (m_Face <= FACE::CHUN)
	);

} /* end of MJPAI::IsYaochu */



size_t	MJPAI::GetNumber() const
{
	if (m_Face <= FACE::UNKNOWN) {
		return 0;
	}
	if (m_Face <= FACE::MAN9) {
		return m_Face - (FACE::MAN1 - 1);
	}
	if (m_Face <= FACE::SOU9) {
		return m_Face - (FACE::SOU1 - 1);
	}
	if (m_Face <= FACE::PIN9) {
		return m_Face - (FACE::PIN1 - 1);
	}
	if (m_Face <= FACE::PEI) {
		return m_Face - (FACE::TON - 1);
	}
	if (m_Face <= FACE::CHUN) {
		return m_Face - (FACE::HAKU - 1);
	}
	return 0;

} /* end of MJPAI::GetNumber */



size_t	MJPAI::GetSuit() const
{
	if (m_Face <= FACE::UNKNOWN) {
		return 0;
	}
	if (m_Face <= FACE::MAN9) {
		return SUIT::MANZU;
	}
	if (m_Face <= FACE::SOU9) {
		return SUIT::SOUZU;
	}
	if (m_Face <= FACE::PIN9) {
		return SUIT::PINZU;
	}
	if (m_Face <= FACE::PEI) {
		return SUIT::KAZE;
	}
	if (m_Face <= FACE::CHUN) {
		return SUIT::SANGEN;
	}
	return 0;

} /* end of MJPAI::GetSuit */



size_t	MJPAI::GetRed() const
{
	return m_Red;

} /* end of MJPAI::GetRed */



string	MJPAI::GetString() const
{
	string	ret;

	switch (GetSuit()) {
	case SUIT::MANZU:
		ret.append("ma");
		ret.append(1, '0' + GetNumber());
		break;

	case SUIT::SOUZU:
		ret.append("so");
		ret.append(1, '0' + GetNumber());
		break;

	case SUIT::PINZU:
		ret.append("pi");
		ret.append(1, '0' + GetNumber());
		break;

	case SUIT::KAZE:
		switch (m_Face) {
		case FACE::TON:
			ret.append("TON");
			break;

		case FACE::NAN:
			ret.append("NAN");
			break;

		case FACE::SHA:
			ret.append("SHA");
			break;

		case FACE::PEI:
			ret.append("PEI");
			break;

		} /* endswitch m_Face */
		break;

	case SUIT::SANGEN:
		switch (m_Face) {
		case FACE::HAKU:
			ret.append("HAK");
			break;

		case FACE::HATU:
			ret.append("HAT");
			break;

		case FACE::CHUN:
			ret.append("CHU");
			break;

		} /* endswitch m_Face */
		break;

	default:
		ret.append("???");
		break;

	} /* endswitch GetSuit() */

	return ret;

} /* end of MJPAI::GetString */



//----------------------------------------------------------------------------
uint8_t	MJPAI::GetOption() const
{
	return m_Option;

} /* end of MJPAI::GetOption */



bool	MJPAI::GetOption(uint8_t Option) const
{
	return (m_Option & Option) ? true : false;

} /* end of MJPAI::GetOption */



void	MJPAI::SetOption(uint8_t Option)
{
	m_Option = Option;

} /* end of MJPAI::SetOption */



uint8_t	MJPAI::GetMark() const
{
	return m_Mark;

} /* end of MJPAI::GetMark */



void	MJPAI::SetMark(uint8_t Mark)
{
	m_Mark = Mark;

} /* end of MJPAI::SetMark */



void	MJPAI::Shade()
{
	m_Face = FACE::UNKNOWN;
	m_Red = 0;

} /* end of MJPAI::Shade */



//----------------------------------------------------------------------------
