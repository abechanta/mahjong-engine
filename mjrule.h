#ifndef MJRULE_H
#define MJRULE_H

#include "mjglobal.h"
#include "mjpaiarray.h"
#include <string>

class MJRULE
{
	//
	// property
	//
public:
	// 進行・流局
	struct SEQ
	{
		size_t		m_PlayerCount:8;			// プレイ人数
		size_t		m_RoundCount:8;				// ラウンドの数（1:東風戦、2:半荘戦、4:一荘戦）
		size_t		m_WampaiCount:8;			// ワンパイの牌数
		size_t		m_TehaiCount:8;				// 手牌の数

		size_t		m_MentsuCount:8;			// 面子の数
		size_t		m_RinshanCount:4;			// リンシャン（カン）の数
		size_t		m_NotenOyanagare:4;			// ノーテン親流れ（MSB..LSB:北場～東場、0:なし、1:あり）
		size_t		m_Keiten:1;					// 形式テンパイ（0:なし、1:あり）
		size_t		m_Hakoshita:1;				// 箱下（0:なし=終了、1:あり）
		size_t		m_HakoshitaReach:1;			// 箱下リーチ（0:なし、1:あり=流局時即終了）
		size_t		m_OhrasuOyanagare:1;		// オーラス親流れ（0:なし、1:あり）
		size_t		m_OhrasuAgariyame:1;		// オーラス親のアガリ止め（0:なし、1:あり）
		size_t		m_TochuRyukyokuRenchan:1;	// 途中流局（0:親流れ、1:連荘）
		size_t		m_DoubleRon:1;				// ダブロン（0:上家取り、1:あり）
		size_t		m_TripleRon:1;				// トリプルロン（0:流局、1:あり）
		size_t		m_NakiPriority:1;			// 鳴き優先順（0:ポン優先、1:発声優先）
		size_t		Rsv:7;						// 
	}			m_Seq;

	// 点数
	struct PTS
	{
		ssize_t		m_StartPoint;				// 原点
		ssize_t		m_SeisanPoint;				// 清算のときの原点
		ssize_t		m_UmaPoint0Plus[4];			// 0 人プラス時のウマ（トップ～ラス）
		ssize_t		m_UmaPoint1Plus[4];			// 1 人プラス時のウマ（トップ～ラス）
		ssize_t		m_UmaPoint2Plus[4];			// 2 人プラス時のウマ（トップ～ラス）
		ssize_t		m_UmaPoint3Plus[4];			// 3 人プラス時のウマ（トップ～ラス）
		ssize_t		m_TobiPoint;				// 飛び賞
		ssize_t		m_YakumanExtraPointTumo;	// 役満ツモ賞
		ssize_t		m_YakumanExtraPointRon;		// 役満ロン賞

		size_t		m_Pao:1;					// 包（0:なし、1:あり）
		size_t		m_Wareme:1;					// 割れ目（0:なし、1:あり）
		size_t		Rsv:30;						// 
	}			m_Pts;

	// ドラ
	struct DORA
	{
		size_t		m_DoraCount:2;				// ダブドラ（0:なし、1:あり、2:ダブドラ）
		size_t		m_UraDora:1;				// 裏ドラ（0:なし、1:あり）
		size_t		m_KanDora:1;				// カンドラ（0:なし、1:あり）
		size_t		m_KanUra:1;					// カン裏ドラ（0:なし、1:あり）
		size_t		m_KanSoku:1;				// カンドラのタイミング（0:暗カンは即乗りでその他は捨て牌直後、1:即乗り）
		size_t		Rsv:26;						// 
	}			m_Dora;

	// 手役
	struct YAKU
	{
		size_t		m_PinfuTumo:1;				// ピンフツモ（0:なし、1:あり）
		size_t		m_Kuitan:1;					// 喰いタン（0:なし、1:あり）
		size_t		m_Atozuke:1;				// 後付け（0:なし、1:あり）
		size_t		m_Chitoitsu25hu2han:1;		// 七対子飜数（0:50符1飜、1:25符2飜）
		size_t		m_RyanhanShibari:1;			// 二飜縛り（0:なし、1:あり）
		size_t		m_Renho:1;					// 人和（0:満貫、1:役満）
		size_t		Rsv:25;						// 
	}			m_Yaku;

	// 役満
	struct YAKUMAN
	{
		size_t		m_Kokushimusou_FuritenRon:1;	// 国士無双: 現物以外のフリテンロン（0:なし、1:あり）
		size_t		m_Kokushimusou_Chankan:1;		// 国士無双: 暗カンに対するチャンカン（0:なし、1:あり）
		size_t		m_Ryuiisou_Hatunashi:1;			// 緑一色: 発なしでも成立（0:しない、1:する）
		size_t		m_Churenpoutou_FuritenRon:1;	// 九連宝橙: 現物以外のフリテンロン（0:なし、1:あり）
		size_t		Rsv:28;						// 
	}			m_Yakuman;

	MJPAIARRAY	m_Set;							// 牌セット

	//
	// method
	//
public:
	MJRULE(const std::string &Name = "");
	MJRULE	&SetRule(const std::string &Name);
};

#endif	// MJRULE_H
