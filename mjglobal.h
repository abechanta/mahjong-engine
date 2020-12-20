#ifndef MJGLOBAL_H
#define MJGLOBAL_H

#include <iostream>

typedef unsigned char	uint8_t;
typedef char			int8_t;
typedef unsigned short	uint16_t;
typedef short			int16_t;
typedef unsigned long	uint32_t;
typedef long			int32_t;
typedef unsigned int	size_t;
typedef int				ssize_t;



#define MJBREAK\
	_asm	int		3

#define MJASSERT(cond)\
{\
	if (!(cond)) {\
		std::cerr << __FUNCTION__ << '@' << __LINE__ << ": " << ##(cond) << std::endl;\
	}\
}

namespace MJ
{
	enum RET {
		RET_ERR		= -1,
		RET_BUSY	= 0,
		RET_DONE	= 1,
	};

	enum PNO {
		P1			= 0,	// プレイヤー１
		P2			= 1,	// プレイヤー２
		P3			= 2,	// プレイヤー３
		P4			= 3,	// プレイヤー４
		PMAX		= 4,
	};

	enum PID {
		TON			= 0,	// 東家
		NAN			= 1,	// 南家
		SHA			= 2,	// 西家
		PEI			= 3,	// 北家
		NOBODY		= 99,	// 誰でもない
	};

	enum PIDD {
		SELF		= 0,	// 自家
		SIMO		= 1,	// 下家
		TOI			= 2,	// 対家
		KAMI		= 3,	// 上家
	};

#if 0	//!!!
	enum DIR {
		DIR0	= 0,
		DIR1	= 1,
		DIR2	= 2,
		DIR3	= 3,
	};

	enum ORIGIN {
		BOTTOM_LEFT		= 1,
		BOTTOM_CENTER	= 2,
		BOTTOM_RIGHT	= 3,
		MIDDLE_LEFT		= 4,
		MIDDLE_CENTER	= 5,
		MIDDLE_RIGHT	= 6,
		TOP_LEFT		= 7,
		TOP_CENTER		= 8,
		TOP_RIGHT		= 9,
	};
#endif
};

#endif	// MJGLOBAL_H
