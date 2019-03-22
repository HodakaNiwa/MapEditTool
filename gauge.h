//*****************************************************************************
//
//     ゲージの処理[gauge.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _GAUGE_H_
#define _GAUGE_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "sceneBillboard.h"

//*****************************************************************************
//    ゲージクラスの定義
//*****************************************************************************
class CGauge : public CSceneBillboard
{
public:    // 誰でもアクセス可能
	CGauge(int nPriority = 3, OBJTYPE objType = OBJTYPE_GAUGE);
	~CGauge();

	static CGauge *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fWidth, float fHeight, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void SetGaugeRate(const float fGaugeRate);
	void SetRightWidth(const float fRightWidth);

	float GetGaugeRate(void);
	float GetRightWidth(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	float m_fGaugeRate;    // ゲージの割合
	float m_fRightWidth;   // ゲージの左側の長さ
};

//*****************************************************************************
//    体力ゲージクラスの定義
//*****************************************************************************
class CGaugeLife : public CGauge
{
public:    // 誰でもアクセス可能
	//----------------------
	// 状態
	//----------------------
	typedef enum
	{
		STATE_NONE = 0,
		STATE_CUT,
		STATE_ADD,
		STATE_MAX
	}STATE;

	CGaugeLife(int nPriority = 3, OBJTYPE objType = OBJTYPE_LIFEGAUGE);
	~CGaugeLife();

	static CGaugeLife *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fWidth, float fHeight, int nLife, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void CutGauge(const int nCutValue);
	void AddGauge(const int nAddValue);

	void SetGaugePos(const D3DXVECTOR3 pos);
	void SetState(const STATE State);
	void SetLife(const int nLife);

	STATE GetState(void);
	int GetLife(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	int             m_nCounter;      // ゲージの減らしを管理するカウンター
	STATE           m_State;         // 状態
	int             m_nLife;         // 体力
	CGauge         *m_pWhiteBill;    // 体力ゲージのフレーム
	CGauge         *m_pRedBill;      // 体力ゲージが減った時に表示する赤いゲージ
};

#endif