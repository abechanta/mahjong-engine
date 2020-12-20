#ifndef MJPAI_H
#define MJPAI_H

#include "mjglobal.h"
#include <string>

class MJPAI
{
	friend	class MJPAICOUNT;

	//
	// definition
	//
public:
	enum SUIT {
		MANZU		= 1,	// CHARACTERS
		SOUZU		= 2,	// BAMBOOS
		PINZU		= 3,	// DOTS
		KAZE		= 4,	// WINDS
		SANGEN		= 5,	// DRAGONS
	};

	enum FACE {
		UNKNOWN		= 0,	// 不明
		MAN1		= 1,	// マンズ１
		MAN2		= 2,	// マンズ２
		MAN3		= 3,	// マンズ３
		MAN4		= 4,	// マンズ４
		MAN5		= 5,	// マンズ５
		MAN6		= 6,	// マンズ６
		MAN7		= 7,	// マンズ７
		MAN8		= 8,	// マンズ８
		MAN9		= 9,	// マンズ９
		SOU1		= 10,	// ソーズ１
		SOU2		= 11,	// ソーズ２
		SOU3		= 12,	// ソーズ３
		SOU4		= 13,	// ソーズ４
		SOU5		= 14,	// ソーズ５
		SOU6		= 15,	// ソーズ６
		SOU7		= 16,	// ソーズ７
		SOU8		= 17,	// ソーズ８
		SOU9		= 18,	// ソーズ９
		PIN1		= 19,	// ピンズ１
		PIN2		= 20,	// ピンズ２
		PIN3		= 21,	// ピンズ３
		PIN4		= 22,	// ピンズ４
		PIN5		= 23,	// ピンズ５
		PIN6		= 24,	// ピンズ６
		PIN7		= 25,	// ピンズ７
		PIN8		= 26,	// ピンズ８
		PIN9		= 27,	// ピンズ９
		TON			= 28,	// 風牌 東
		NAN			= 29,	// 風牌 南
		SHA			= 30,	// 風牌 西
		PEI			= 31,	// 風牌 北
		HAKU		= 32,	// 字牌 白
		HATU		= 33,	// 字牌 発
		CHUN		= 34,	// 字牌 中
		MAX			= 35,	// 
	};

	enum OPTION {
		STANDING	= (1 << 0),
		POPPED		= (1 << 1),
		SIDEWAYS	= (1 << 2),
		CLOSED		= (1 << 3),
		DISABLED	= (1 << 4),
		LOST		= (1 << 5),
	};

	enum MARK {
		REACH		= (1 << 0),
		CHI			= (2 << 0),
		PON			= (3 << 0),
		KAN			= (4 << 0),
		TUMO		= (5 << 0),
		RON			= (6 << 0),
		_MASK		= (7 << 0),
		TENPAI		= (1 << 3),
		TUMOGIRI	= (1 << 4),
	};

	//
	// property
	//
private:
	uint8_t					m_Face:7;		// 種類
	uint8_t					m_Red:1;		// 赤
	uint8_t					m_Option;		// 状態
	uint8_t					m_Mark;			// マーカー

	//
	// method
	//
public:
	MJPAI(uint8_t Face, uint8_t Red, uint8_t Option = 0, uint8_t Mark = 0);
	MJPAI(uint8_t Code = 0);
	MJPAI(const MJPAI &Other);

	size_t	GetCode(bool bFaceOnly) const;
private:
	void	AddFace(ssize_t Delta);
public:
	MJPAI	&operator =(const MJPAI &Other);
	MJPAI	operator +(ssize_t Other) const;
	MJPAI	operator -(ssize_t Other) const;
	MJPAI	&operator +=(ssize_t Other);
	MJPAI	&operator -=(ssize_t Other);
	MJPAI	&operator ++() { return operator +=(1); }
	MJPAI	&operator ++(int) { return operator +=(1); }
	MJPAI	&operator --() { return operator -=(1); }
	MJPAI	&operator --(int) { return operator -=(1); }

	MJPAI	&operator |=(size_t Other);
	MJPAI	&operator &=(size_t Other);

	bool	operator ==(const MJPAI &Other) const;
	bool	operator !=(const MJPAI &Other) const;
	bool	operator <(const MJPAI &Other) const;
	bool	operator <=(const MJPAI &Other) const;
	bool	operator >(const MJPAI &Other) const;
	bool	operator >=(const MJPAI &Other) const;

	bool	IsValid() const;
	bool	IsKazuhai() const;
	bool	IsJihai() const;
	bool	IsChuchan() const;
	bool	IsYaochu() const;
	size_t	GetNumber() const;
	size_t	GetSuit() const;
	size_t	GetRed() const;
	std::string	GetString() const;

	uint8_t	GetOption() const;
	bool	GetOption(uint8_t Option) const;
	void	SetOption(uint8_t Option);
	uint8_t	GetMark() const;
	void	SetMark(uint8_t Mark);

	void	Shade();
};

#endif	// MJPAI_H
