//*****************************************************************************
//
//     半球の処理[meshDome.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MESHDOME_H_
#define _MESHDOME_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    半球クラスの定義
//*****************************************************************************
class CMeshDome : public CScene
{
public:    // 誰でもアクセス可能
	CMeshDome(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHDOME);
	~CMeshDome();
	static CMeshDome *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeNormal(void);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetCol(const D3DXCOLOR col);
	void SetRadius(const float fRadius);
	void SetXBlock(const int nXBlock);
	void SetYBlock(const int nYBlock);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXCOLOR GetCol(void);
	float GetRadius(void);
	int GetXBlock(void);
	int GetYBlock(void);
	int GetNumVertex(void);
	int GetNumIndex(void);
	int GetNumPolygon(void);
	D3DXMATRIX GetMtxWorld(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;     // 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;     // インデックスバッファへのポインタ
	LPDIRECT3DTEXTURE9		m_pTexture;	    // テクスチャへのポインタ
	D3DXVECTOR3             m_Pos;          // メッシュドームの座標
	D3DXVECTOR3             m_Rot;          // メッシュドームの向き
	D3DXCOLOR               m_Col;          // メッシュドームの色
	float                   m_fRadius;      // メッシュドームの半径
	int                     m_nXBlock;      // 横の分割数
	int                     m_nYBlock;      // 縦の分割数
	int                     m_nNumVertex;   // 頂点数
	int                     m_nNumIndex;    // インデックス数
	int                     m_nNumPolygon;  // ポリゴン数
	D3DXMATRIX              m_MtxWorld;     // ワールドマトリックス
};

#endif