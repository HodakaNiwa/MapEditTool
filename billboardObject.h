//*****************************************************************************
//
//     配置ビルボードの処理[billboardObject.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _BILLBOARDOBJECT_H_
#define _BILLBOARDOBJECT_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "sceneBillboard.h"

//*****************************************************************************
//    配置ビルボードクラスの定義
//*****************************************************************************
class CBillboardObject : public CSceneBillboard
{
public:    // 誰でもアクセス可能
	CBillboardObject(int nPriority = 3, OBJTYPE objType = OBJTYPE_BILLBOARDOBJ);
	~CBillboardObject();

	static CBillboardObject *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, bool bLighting = false, bool bDrawAddtive = false, int nTexIdx = 0, float fColRange = 0.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, bool bLighting = false, bool bDrawAddtive = false, int nTexIdx = 0, float fColRange = 0.0f);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	bool Collision(D3DXVECTOR3 *pPos, float fColRange);

	void SetTexIdx(const int nTexIdx);
	void SetLighting(const bool bLighting);
	void SetDrawAddtive(const bool bDrawAddtive);
	void SetColRange(const float fColRange);

	int GetTexIdx(void);
	bool GetLighting(void);
	bool GetDrawAddtive(void);
	float GetColRange(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	int   m_nTexIdx;       // 使用しているテクスチャの番号
	bool  m_bLighting;     // ライティングをするかどうか
	bool  m_bDrawAddtive;  // 加算合成で描画するかしないか
	float m_fColRange;     // 当たり判定を取る範囲
};

//*****************************************************************************
//    配置専用の配置ビルボードクラスの定義
//*****************************************************************************
class CSetBillboardObject : public CBillboardObject
{
public:    // 誰でもアクセス可能
	CSetBillboardObject(int nPriority = 3, OBJTYPE objType = OBJTYPE_BILLBOARD);
	~CSetBillboardObject();

	static CSetBillboardObject *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, bool bLighting = false, bool bDrawAddtive = false, int nTexIdx = 0, float fColRange = 0.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, bool bLighting = false, bool bDrawAddtive = false, int nTexIdx = 0, float fColRange = 0.0f);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ChangeVertex(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
};

#endif