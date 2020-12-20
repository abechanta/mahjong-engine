#include "mjpaiarray.h"
#include <algorithm>
#include <functional>
using namespace std;

//----------------------------------------------------------------------------
MJPAIARRAY::MJPAIARRAY()
	: m_bSorted(true)
{
} /* end of MJPAIARRAY::MJPAIARRAY */



MJPAIARRAY::MJPAIARRAY(const MJPAI &Other)
	: m_bSorted(true)
{
	this->push_back(Other);

} /* end of MJPAIARRAY::MJPAIARRAY */



MJPAIARRAY::MJPAIARRAY(const MJPAIARRAY &Other)
	: m_bSorted(Other.m_bSorted)
{
#if 0
	this->insert(this->end(), Other.begin(), Other.end());
#else
	for (const_iterator src = Other.begin(); src != Other.end(); ++src) {
		this->push_back(*src);
	}
#endif

} /* end of MJPAIARRAY::MJPAIARRAY */



//----------------------------------------------------------------------------
MJPAIARRAY	&MJPAIARRAY::operator =(const MJPAI &Other)
{
	this->resize(0);
	m_bSorted = true;
	this->push_back(Other);
	return *this;

} /* end of MJPAIARRAY::operator = */



MJPAIARRAY	&MJPAIARRAY::operator =(const MJPAIARRAY &Other)
{
	this->resize(0);
	m_bSorted = Other.m_bSorted;
#if 0
	this->insert(this->end(), Other.begin(), Other.end());
#else
	for (const_iterator src = Other.begin(); src != Other.end(); ++src) {
		this->push_back(*src);
	}
#endif
	return *this;

} /* end of MJPAIARRAY::operator = */



MJPAIARRAY	MJPAIARRAY::operator +(const MJPAI &Other) const
{
	MJPAIARRAY	ret = *this;

	ret.m_bSorted = false;
	ret.push_back(Other);
	return ret;

} /* end of MJPAIARRAY::operator + */



MJPAIARRAY	MJPAIARRAY::operator +(const MJPAIARRAY &Other) const
{
	MJPAIARRAY	ret = *this;

	ret.m_bSorted = false;
#if 0
	ret.insert(ret.end(), Other.begin(), Other.end());
#else
	for (const_iterator src = Other.begin(); src != Other.end(); ++src) {
		ret.push_back(*src);
	}
#endif
	return ret;

} /* end of MJPAIARRAY::operator + */



MJPAIARRAY	MJPAIARRAY::operator -(const MJPAI &Other) const
{
	MJPAIARRAY	ret = *this;

	for (iterator dst = ret.begin(); dst != ret.end(); ++dst) {
		if (*dst == Other) {
			dst = ret.erase(dst);
			break;
		}
	}
//	MJASSERT(ret.size() == this->size() - 1);
	return ret;

} /* end of MJPAIARRAY::operator - */



MJPAIARRAY	MJPAIARRAY::operator -(const MJPAIARRAY &Other) const
{
	MJPAIARRAY	ret = *this;

	for (const_iterator src = Other.begin(); src != Other.end(); ++src) {
		for (iterator dst = ret.begin(); dst != ret.end(); ++dst) {
			if (*dst == *src) {
				dst = ret.erase(dst);
				break;
			}
		}
	}
//	MJASSERT(ret.size() == this->size() - Other.size());
	return ret;

} /* end of MJPAIARRAY::operator - */



MJPAIARRAY	&MJPAIARRAY::operator +=(const MJPAI &Other)
{
	m_bSorted = false;
	this->push_back(Other);
	return *this;

} /* end of MJPAIARRAY::operator += */



MJPAIARRAY	&MJPAIARRAY::operator +=(const MJPAIARRAY &Other)
{
	m_bSorted = false;
#if 0
	this->insert(this->end(), Other.begin(), Other.end());
#else
	const_iterator	it;

	for (it = Other.begin(); it != Other.end(); ++it) {
		this->push_back(*it);
	}
#endif
	return *this;

} /* end of MJPAIARRAY::operator += */



MJPAIARRAY	&MJPAIARRAY::operator -=(const MJPAI &Other)
{
//	size_t	size = this->size();

	for (iterator dst = this->begin(); dst != this->end(); ++dst) {
		if (*dst == Other) {
			dst = this->erase(dst);
			break;
		}
	}
//	MJASSERT(this->size() == size - 1);
	return *this;

} /* end of MJPAIARRAY::operator -= */



MJPAIARRAY	&MJPAIARRAY::operator -=(const MJPAIARRAY &Other)
{
//	size_t	size = this->size();

	for (const_iterator src = Other.begin(); src != Other.end(); ++src) {
		for (iterator dst = this->begin(); dst != this->end(); ++dst) {
			if (*dst == *src) {
				dst = this->erase(dst);
				break;
			}
		}
	}
//	MJASSERT(ret.size() == size - Other.size());
	return *this;

} /* end of MJPAIARRAY::operator -= */



MJPAIARRAY	&MJPAIARRAY::operator |=(size_t Other)
{
	for (iterator dst = this->begin(); dst != this->end(); ++dst) {
		*dst |= Other;
	}
	return *this;

} /* end of MJPAIARRAY::operator |= */



MJPAIARRAY	&MJPAIARRAY::operator &=(size_t Other)
{
	for (iterator dst = this->begin(); dst != this->end(); ++dst) {
		*dst &= Other;
	}
	return *this;

} /* end of MJPAIARRAY::operator &= */



//----------------------------------------------------------------------------
bool	MJPAIARRAY::operator ==(const MJPAIARRAY &Other) const
{
	return equal(this->begin(), this->end(), Other.begin());

} /* end of MJPAIARRAY::operator == */



bool	MJPAIARRAY::operator !=(const MJPAIARRAY &Other) const
{
	return !equal(this->begin(), this->end(), Other.begin());

} /* end of MJPAIARRAY::operator != */



bool	MJPAIARRAY::operator <(const MJPAIARRAY &Other) const
{
	const_iterator	src, dst;

	for (src = this->begin(), dst = Other.begin(); (src != this->end()) && (dst != Other.end()); ++src, ++dst) {
		if (*src == *dst) {
			continue;
		}
		return (*src < *dst);
	}
	return (dst != Other.end());

} /* end of MJPAIARRAY::operator < */



bool	MJPAIARRAY::operator <=(const MJPAIARRAY &Other) const
{
	const_iterator	src, dst;

	for (src = this->begin(), dst = Other.begin(); (src != this->end()) && (dst != Other.end()); ++src, ++dst) {
		if (*src == *dst) {
			continue;
		}
		return (*src < *dst);
	}
	return (
		(dst != Other.end()) ||
		(src == this->end()) && (dst == Other.end())
	);

} /* end of MJPAIARRAY::operator <= */



bool	MJPAIARRAY::operator >(const MJPAIARRAY &Other) const
{
	const_iterator	src, dst;

	for (src = this->begin(), dst = Other.begin(); (src != this->end()) && (dst != Other.end()); ++src, ++dst) {
		if (*src == *dst) {
			continue;
		}
		return (*src > *dst);
	}
	return (src != this->end());

} /* end of MJPAIARRAY::operator > */



bool	MJPAIARRAY::operator >=(const MJPAIARRAY &Other) const
{
	const_iterator	src, dst;

	for (src = this->begin(), dst = Other.begin(); (src != this->end()) && (dst != Other.end()); ++src, ++dst) {
		if (*src == *dst) {
			continue;
		}
		return (*src > *dst);
	}
	return (
		(src != this->end()) ||
		(src == this->end()) && (dst == Other.end())
	);

} /* end of MJPAIARRAY::operator >= */



//----------------------------------------------------------------------------
bool	MJPAIARRAY::IsValid() const
{
	return (find_if(this->begin(), this->end(), mem_fun_ref(&MJPAI::IsValid)) == this->end());

} /* end of MJPAIARRAY::IsValid */



size_t	MJPAIARRAY::GetRed() const
{
	return static_cast<size_t>(count_if(this->begin(), this->end(), mem_fun_ref(&MJPAI::GetRed)));

} /* end of MJPAIARRAY::GetRed */



string	MJPAIARRAY::GetString() const
{
	string	ret;

	ret.append("{");
	for (const_iterator src = this->begin(); src != this->end(); ++src) {
		ret.append(" ");
		ret.append(src->GetString());
	}
	ret.append(" }");

	return ret;

} /* end of MJPAIARRAY::GetString */



//----------------------------------------------------------------------------
void	MJPAIARRAY::Sort()
{
	sort(this->begin(), this->end());
	m_bSorted = true;

} /* end of MJPAIARRAY::Sort */



void	MJPAIARRAY::Shade()
{
	for (iterator dst = this->begin(); dst != this->end(); ++dst) {
		dst->Shade();
	}

} /* end of MJPAIARRAY::Shade */



//----------------------------------------------------------------------------
