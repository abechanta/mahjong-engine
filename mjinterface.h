#ifndef MJINTERFACE_H
#define MJINTERFACE_H

#include "mjglobal.h"
#include "mjevent.h"
#include "mjplug.h"
#include "mjpoints.h"
#include <string>
class MJGAMEMASTER;
class MJPOINTS;
class MJPLAYER;

class MJINTERFACE
{
	friend	class MJGAMEMASTER;

	//
	// definition
	//
protected:
	enum RET {
		RET_ERR		= MJ::RET_ERR,
		RET_BUSY	= MJ::RET_BUSY,
		RET_DONE	= MJ::RET_DONE,
	};

	//
	// property
	//
public:
	MJGAMEMASTER			*m_pMaster;		// FIXME: 直接参照してはいけない（デバッグ用） → インターフェース経由で参照させる
	MJPLAYER				*m_pPlayer;		// FIXME: 直接参照してはいけない（デバッグ用）

protected:
	std::string				m_Name;			// 表示名
	MJPLUG<MJEVENT>			m_Plug;			// 通信ポイント

	//
	// method
	//
public:
	MJINTERFACE(const std::string &Name);
	virtual	~MJINTERFACE();

	ssize_t	SendEvent(const MJEVENT &Event);
	ssize_t	Server();
private:
	virtual	ssize_t	Select(const MJEVENT &Event) = 0;
};

#endif	// MJINTERFACE_H
