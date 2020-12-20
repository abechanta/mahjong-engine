#include "mjpaicount.h"
#include <algorithm>
using namespace std;

//----------------------------------------------------------------------------
MJPAICOUNT::MJPAICOUNT()
{
	resize(MJPAI::MAX, 0);

} /* end of MJPAICOUNT::MJPAICOUNT */



MJPAICOUNT::MJPAICOUNT(const MJPAI &Other)
{
	resize(MJPAI::MAX, 0);

	at(Other.IsValid() ? Other.m_Face : MJPAI::UNKNOWN)++;

} /* end of MJPAICOUNT::MJPAICOUNT */



MJPAICOUNT::MJPAICOUNT(const MJPAIARRAY &Other)
{
	resize(MJPAI::MAX, 0);

	MJPAIARRAY::const_iterator	it;

	for (it = Other.begin(); it != Other.end(); ++it) {
		at(it->IsValid() ? it->m_Face : MJPAI::UNKNOWN)++;
	}

} /* end of MJPAICOUNT::MJPAICOUNT */



MJPAICOUNT::MJPAICOUNT(const MJPAICOUNT &Other)
{
	resize(MJPAI::MAX, 0);

	*this = Other;

} /* end of MJPAICOUNT::MJPAICOUNT */



//----------------------------------------------------------------------------
MJPAICOUNT	&MJPAICOUNT::operator =(const MJPAI &Other)
{
	resize(MJPAI::MAX, 0);

	at(Other.IsValid() ? Other.m_Face : MJPAI::UNKNOWN)++;
	return *this;

} /* end of MJPAICOUNT::operator = */



MJPAICOUNT	&MJPAICOUNT::operator =(const MJPAIARRAY &Other)
{
	resize(MJPAI::MAX, 0);

	MJPAIARRAY::const_iterator	it;

	for (it = Other.begin(); it != Other.end(); ++it) {
		at(it->IsValid() ? it->m_Face : MJPAI::UNKNOWN)++;
	}
	return *this;

} /* end of MJPAICOUNT::operator = */



MJPAICOUNT	&MJPAICOUNT::operator =(const MJPAICOUNT &Other)
{
	resize(MJPAI::MAX, 0);

	MJPAICOUNT::const_iterator	src;
	MJPAICOUNT::iterator	dst;

	for (src = Other.begin(), dst = this->begin(); (src != Other.end()) && (dst != this->end()); ++src, ++dst) {
		*dst = *src;
	}
	return *this;

} /* end of MJPAICOUNT::operator = */



MJPAICOUNT	MJPAICOUNT::operator +(const MJPAI &Other) const
{
	MJPAICOUNT	ret = *this;

	ret.at(Other.IsValid() ? Other.m_Face : MJPAI::UNKNOWN)++;
	return ret;

} /* end of MJPAICOUNT::operator + */



MJPAICOUNT	MJPAICOUNT::operator +(const MJPAIARRAY &Other) const
{
	MJPAICOUNT	ret = *this;
	MJPAIARRAY::const_iterator	it;

	for (it = Other.begin(); it != Other.end(); ++it) {
		ret.at(it->IsValid() ? it->m_Face : MJPAI::UNKNOWN)++;
	}
	return ret;

} /* end of MJPAICOUNT::operator + */



MJPAICOUNT	MJPAICOUNT::operator +(const MJPAICOUNT &Other) const
{
	MJPAICOUNT	ret = *this;
	MJPAICOUNT::const_iterator	src;
	MJPAICOUNT::iterator	dst;

	for (src = Other.begin(), dst = ret.begin(); (src != Other.end()) && (dst != ret.end()); ++src, ++dst) {
		*dst += *src;
	}
	return ret;

} /* end of MJPAICOUNT::operator + */



MJPAICOUNT	MJPAICOUNT::operator -(const MJPAI &Other) const
{
	MJPAICOUNT	ret = *this;

	if (ret.at(Other.IsValid() ? Other.m_Face : MJPAI::UNKNOWN) > 0) {
		ret.at(Other.IsValid() ? Other.m_Face : MJPAI::UNKNOWN)--;
	} else {
		MJASSERT(false);
	}
	return ret;

} /* end of MJPAICOUNT::operator - */



MJPAICOUNT	MJPAICOUNT::operator -(const MJPAIARRAY &Other) const
{
	MJPAICOUNT	ret = *this;
	MJPAIARRAY::const_iterator	it;

	for (it = Other.begin(); it != Other.end(); ++it) {
		if (ret.at(it->IsValid() ? it->m_Face : MJPAI::UNKNOWN) > 0) {
			ret.at(it->IsValid() ? it->m_Face : MJPAI::UNKNOWN)--;
		} else {
			MJASSERT(false);
		}
	}
	return ret;

} /* end of MJPAICOUNT::operator - */



MJPAICOUNT	MJPAICOUNT::operator -(const MJPAICOUNT &Other) const
{
	MJPAICOUNT	ret = *this;
	MJPAICOUNT::const_iterator	src;
	MJPAICOUNT::iterator	dst;

	for (src = Other.begin(), dst = ret.begin(); (src != Other.end()) && (dst != ret.end()); ++src, ++dst) {
		if (*dst >= *src) {
			*dst -= *src;
		} else {
			MJASSERT(false);
			*dst = 0;
		}
	}
	return ret;

} /* end of MJPAICOUNT::operator - */



MJPAICOUNT	&MJPAICOUNT::operator +=(const MJPAI &Other)
{
	at(Other.IsValid() ? Other.m_Face : MJPAI::UNKNOWN)++;
	return *this;

} /* end of MJPAICOUNT::operator += */



MJPAICOUNT	&MJPAICOUNT::operator +=(const MJPAIARRAY &Other)
{
	MJPAIARRAY::const_iterator	it;

	for (it = Other.begin(); it != Other.end(); ++it) {
		at(it->IsValid() ? it->m_Face : MJPAI::UNKNOWN)++;
	}
	return *this;

} /* end of MJPAICOUNT::operator += */



MJPAICOUNT	&MJPAICOUNT::operator +=(const MJPAICOUNT &Other)
{
	MJPAICOUNT::const_iterator	src;
	MJPAICOUNT::iterator	dst;

	for (src = Other.begin(), dst = this->begin(); (src != Other.end()) && (dst != this->end()); ++src, ++dst) {
		*dst += *src;
	}
	return *this;

} /* end of MJPAICOUNT::operator += */



MJPAICOUNT	&MJPAICOUNT::operator -=(const MJPAI &Other)
{
	if (at(Other.IsValid() ? Other.m_Face : MJPAI::UNKNOWN) > 0) {
		at(Other.IsValid() ? Other.m_Face : MJPAI::UNKNOWN)--;
	} else {
		MJASSERT(false);
	}
	return *this;

} /* end of MJPAICOUNT::operator -= */



MJPAICOUNT	&MJPAICOUNT::operator -=(const MJPAIARRAY &Other)
{
	MJPAIARRAY::const_iterator	it;

	for (it = Other.begin(); it != Other.end(); ++it) {
		if (at(it->IsValid() ? it->m_Face : MJPAI::UNKNOWN) > 0) {
			at(it->IsValid() ? it->m_Face : MJPAI::UNKNOWN)--;
		} else {
			MJASSERT(false);
		}
	}
	return *this;

} /* end of MJPAICOUNT::operator -= */



MJPAICOUNT	&MJPAICOUNT::operator -=(const MJPAICOUNT &Other)
{
	MJPAICOUNT::const_iterator	src;
	MJPAICOUNT::iterator	dst;

	for (src = Other.begin(), dst = this->begin(); (src != Other.end()) && (dst != this->end()); ++src, ++dst) {
		if (*dst > *src) {
			*dst -= *src;
		} else {
			MJASSERT(false);
			*dst = 0;
		}
	}
	return *this;

} /* end of MJPAICOUNT::operator -= */



//----------------------------------------------------------------------------
bool	MJPAICOUNT::operator ==(const MJPAICOUNT &Other) const
{
	return equal(this->begin(), this->end(), Other.begin());

} /* end of MJPAICOUNT::operator == */



bool	MJPAICOUNT::operator !=(const MJPAICOUNT &Other) const
{
	return !equal(this->begin(), this->end(), Other.begin());

} /* end of MJPAICOUNT::operator != */



//----------------------------------------------------------------------------
size_t	MJPAICOUNT::operator [](const MJPAI &Other) const
{
	return at(Other.IsValid() ? Other.m_Face : MJPAI::UNKNOWN);

} /* end of MJPAICOUNT::operator [] */



//----------------------------------------------------------------------------
bool	MJPAICOUNT::Include(const MJPAICOUNT &Other) const
{
	MJPAICOUNT::const_iterator	src, dst;

	for (src = this->begin(), dst = Other.begin(); (src != this->end()) && (dst != Other.end()); ++src, ++dst) {
		if (*src < *dst) {
			return false;
		}
	}
	return true;

} /* end of MJPAICOUNT::Include */



MJPAICOUNT	&MJPAICOUNT::Union(const MJPAICOUNT &Other)
{
	MJPAICOUNT::const_iterator	src;
	MJPAICOUNT::iterator	dst;

	for (src = Other.begin(), dst = this->begin(); (src != Other.end()) && (dst != this->end()); ++src, ++dst) {
		*dst = max(*src, *dst);
	}
	return *this;

} /* end of MJPAICOUNT::Union */



MJPAICOUNT	&MJPAICOUNT::Intersect(const MJPAICOUNT &Other)
{
	MJPAICOUNT::const_iterator	src;
	MJPAICOUNT::iterator	dst;

	for (src = Other.begin(), dst = this->begin(); (src != Other.end()) && (dst != this->end()); ++src, ++dst) {
		*dst = min(*src, *dst);
	}
	return *this;

} /* end of MJPAICOUNT::Intersect */



string	MJPAICOUNT::GetString() const
{
	string	ret;

	ret.append("MANZU:");
	for (size_t ii = MJPAI::MAN1; ii <= MJPAI::MAN9; ii++) {
		ret.append(1, '0' + at(ii));
	}
	ret.append(" SOUZU:");
	for (size_t ii = MJPAI::SOU1; ii <= MJPAI::SOU9; ii++) {
		ret.append(1, '0' + at(ii));
	}
	ret.append(" PINZU:");
	for (size_t ii = MJPAI::PIN1; ii <= MJPAI::PIN9; ii++) {
		ret.append(1, '0' + at(ii));
	}
	ret.append(" KAZE:");
	for (size_t ii = MJPAI::TON; ii <= MJPAI::PEI; ii++) {
		ret.append(1, '0' + at(ii));
	}
	ret.append(" SANGEN:");
	for (size_t ii = MJPAI::HAKU; ii <= MJPAI::CHUN; ii++) {
		ret.append(1, '0' + at(ii));
	}
	ret.append(" ?:");
	if (at(MJPAI::UNKNOWN) > 9) {
		ret.append("9+");
	} else {
		ret.append(1, '0' + at(MJPAI::UNKNOWN));
	}

	return ret;

} /* end of MJPAICOUNT::GetString */



//----------------------------------------------------------------------------
