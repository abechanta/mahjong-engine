#ifndef MJPLUG_H
#define MJPLUG_H

#include "mjglobal.h"

template <class T>
class MJPLUG
{
//	friend	class MJPLUG<T>;

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
private:
	MJPLUG<T>				*m_pPlug;		// 相手の通信プラグ
	bool					m_bRecvBusy;	// メッセージ受信済みかどうか
	T						m_RecvMsg;		// 受信したメッセージ

	//
	// method
	//
public:
	MJPLUG<T>()
		: m_pPlug(NULL),
		m_bRecvBusy(false),
		m_RecvMsg()
	{
	} /* end of MJPLUG::MJPLUG */

	~MJPLUG<T>()
	{
		Disconnect();

	} /* end of MJPLUG::~MJPLUG */

private:
	void	SetPlug(MJPLUG<T> *pPlug)
	{
		m_pPlug = pPlug;
		m_bRecvBusy = false;
		m_RecvMsg = T();

	} /* end of MJPLUG::SetPlug */

public:
	ssize_t	Connect(MJPLUG<T> *pPlug)
	{
		MJASSERT(pPlug != NULL);

		this->Disconnect();
		pPlug->Disconnect();

		this->SetPlug(pPlug);
		pPlug->SetPlug(this);

		// 次に送信できるようにするため
		// マスター側は受信済み状態にする
		m_bRecvBusy = true;
		return RET_DONE;

	} /* end of MJPLUG::Connect */

	ssize_t	Disconnect()
	{
		if (m_pPlug == NULL) {
			return RET_ERR;
		}

		MJPLUG<T> *pPlug = m_pPlug;

		// 自分から接続を解除する
		// さもないと永久に相手の解除待ちになってしまう
		this->SetPlug(NULL);
		pPlug->SetPlug(NULL);
		return RET_DONE;

	} /* end of MJPLUG::Disconnect */

private:
	ssize_t	_Send(const T &Msg)
	{
		if (m_bRecvBusy) {
			// すでに受信済み
			return RET_BUSY;
		}

		m_bRecvBusy = true;
		m_RecvMsg = Msg;
		return RET_DONE;

	} /* end of MJPLUG::_Send */

public:
	ssize_t	Send(const T &Msg)
	{
		if (m_pPlug == NULL) {
			return RET_ERR;
		}

		ssize_t	err;

		err = m_pPlug->_Send(Msg);
		if (err == RET_DONE) {
			m_bRecvBusy = false;
		}
		return err;

	} /* end of MJPLUG::Send */

	bool	IsRecv() const
	{
		return m_bRecvBusy ? RET_DONE : RET_BUSY;

	} /* end of MJPLUG::IsRecv */

	ssize_t	Recv(T &Msg)
	{
		Msg = m_RecvMsg;
		return m_bRecvBusy ? RET_DONE : RET_BUSY;

	} /* end of MJPLUG::Recv */
};

#endif	// MJPLUG_H
