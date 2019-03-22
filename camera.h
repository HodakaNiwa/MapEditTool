//*****************************************************************************
//
//     カメラの処理[camera.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    カメラクラスの定義
//*****************************************************************************
class CCamera
{
public:    // 誰でもアクセス可能
	CCamera();
	~CCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);

	void SetPosV(const D3DXVECTOR3 posV);
	void SetPosR(const D3DXVECTOR3 posR);
	void SetRot(const D3DXVECTOR3 Rot);
	void SetMove(const bool bMove);

	D3DXVECTOR3 GetPosV(void);
	D3DXVECTOR3 GetPosR(void);
	D3DXVECTOR3 GetRot(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	D3DXVECTOR3  m_PosV;          // 視点
	D3DXVECTOR3  m_PosR;          // 注視点
	D3DXVECTOR3  m_Rot;           // 向き
	D3DXVECTOR3  m_VecU;          // 上方向ベクトル
	D3DXMATRIX   m_MtxProjection; // プロジェクションマトリックス
	D3DXMATRIX   m_MtxView;       // ビューマトリックス
	D3DVIEWPORT9 m_ViewPort;      // ビューポート(描画領域)
	float        m_fLength;       // 距離
	bool         m_bMove;         // カメラを動かすかどうか
};

#endif