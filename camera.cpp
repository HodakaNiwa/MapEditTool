//*****************************************************************************
//
//     カメラの処理[camera.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debuglog.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define CAMERA_ANGEL_VIEW  (45.0f)    // カメラの画角
#define CAMERA_ANGLE_SPEED (0.01f)    // カメラをマウスで回転させるスピード倍率
#define CAMERA_MOVE_SPEED  (1.3f)     // カメラをマウスで移動させるスピード倍率
#define CAMERA_LENGTH_MIN  (40.0f)    // 視点注視点の距離の最小値
#define CAMERA_LENGTH_MAX  (400.0f)   // 視点注視点の距離の最大値

//=============================================================================
//    コンストラクタ
//=============================================================================
CCamera::CCamera()
{
	// 各種値のクリア
	m_PosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // 視点
	m_PosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // 注視点
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // 向き
	m_VecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // 上方向ベクトル
	m_bMove = true;                             // カメラを動かせるかどうか
	D3DXMatrixIdentity(&m_MtxProjection);       // プロジェクションマトリックス
	D3DXMatrixIdentity(&m_MtxView);             // ビューマトリックス
	m_fLength = 0.0f;                           // 距離
	m_ViewPort.X = 0;                           // 描画領域左上X座標
	m_ViewPort.Y = 0;                           // 描画領域左上Y座標
	m_ViewPort.Width = 0;                       // 描画領域の幅
	m_ViewPort.Height = 0;                      // 描画領域の高さ
	m_ViewPort.MinZ = 0.0f;                     // 描画領域深度バッファ最小値
	m_ViewPort.MaxZ = 0.0f;                     // 描画領域深度バッファ最大値
}

//=============================================================================
//    デストラクタ
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CCamera::Init(void)
{
	// 各種値の初期化
	m_PosV = D3DXVECTOR3(0.0f, 300.0f, -600.0f); // 視点
	m_PosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // 注視点
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // 向き
	m_VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);      // 上方向ベクトル

	// 視点と注視点との距離を計算
	m_fLength = sqrtf((m_PosV.x - m_PosR.x) * (m_PosV.x - m_PosR.x) + (m_PosV.y - m_PosR.y) * (m_PosV.y - m_PosR.y) + (m_PosV.z - m_PosR.z) * (m_PosV.z - m_PosR.z));
	m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
	m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
	m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;

	// ビューポート設定
	m_ViewPort.X = 0;                           // 描画領域左上X座標
	m_ViewPort.Y = 0;                           // 描画領域左上Y座標
	m_ViewPort.Width = SCREEN_WIDTH;            // 描画領域の幅
	m_ViewPort.Height = SCREEN_HEIGHT;          // 描画領域の高さ
	m_ViewPort.MinZ = 0.0f;                     // 描画領域深度バッファ最小値
	m_ViewPort.MaxZ = 1.0f;                     // 描画領域深度バッファ最大値

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
//    更新処理
//=============================================================================
void CCamera::Update(void)
{
	if (m_bMove == true)
	{// カメラを動かせる状態だったら
	    // カメラを動かす
		CInputMouse *pMouse = CManager::GetMouse();
		if (pMouse != NULL)
		{// マウスが取得できた
			if (pMouse->GetMouseAxisZ() != 0)
			{// マウスのホイールボタンがスクロールされた
				m_fLength += 0.1f * pMouse->GetMouseAxisZ();
				if (m_fLength < CAMERA_LENGTH_MIN)
				{// 距離が一定の値より近くなった
					m_fLength = CAMERA_LENGTH_MIN;
				}
				else if (m_fLength > CAMERA_LENGTH_MAX)
				{// 距離が一定の値より遠くなった
					m_fLength = CAMERA_LENGTH_MAX;
				}

				// 視点の計算
				m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
				m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
				m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
			}

			if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) == TRUE && pMouse->GetPress(CInputMouse::DIMS_BUTTON_1) == TRUE)
			{// マウスの左ボタンと右ボタンが同時に押された
				if (pMouse->GetMouseAxisX() != 0 || pMouse->GetMouseAxisY() != 0)
				{// マウスが動かされている
				    // 同時に押されたときの位置から今のマウスカーソルの位置から視点注視点を移動させる向きを計算
					float fAngle = atan2f(((float)pMouse->GetPoint().x - (float)pMouse->GetPointOld().x), ((float)pMouse->GetPoint().y - (float)pMouse->GetPointOld().y));
					fAngle -= m_Rot.y;
					if (fAngle < -D3DX_PI)
					{// 向きが円周率を超えた
						fAngle += D3DX_PI * 2.0f;
					}
					if (fAngle > D3DX_PI)
					{// 向きが円周率を超えた
						fAngle -= D3DX_PI * 2.0f;
					}

					// マウスのX軸,Y軸から移動量を計算
					float fMovement = sqrtf((float)pMouse->GetMouseAxisX() * (float)pMouse->GetMouseAxisX() + (float)pMouse->GetMouseAxisY() * (float)pMouse->GetMouseAxisY());
					fMovement *= CAMERA_MOVE_SPEED;

					// 視点を移動させる
					m_PosR.x += sinf(fAngle) * fMovement;
					m_PosR.z -= cosf(fAngle) * fMovement;

					// 注視点を移動させる
					m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
					m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
				}
			}
			else if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) == TRUE)
			{// マウスの左クリックのみが押された
			    // 向きをマウスの速度に合わせて回転させる
			    // 縦軸
				m_Rot.y += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisX();
				if (m_Rot.y > D3DX_PI)
				{// 向きが円周率を超えた
					m_Rot.y -= D3DX_PI * 2.0f;
				}
				if (m_Rot.y < -D3DX_PI)
				{// 向きが円周率を超えた
					m_Rot.y += D3DX_PI * 2.0f;
				}

				// 横軸
				m_Rot.z += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisY();
				if (m_Rot.z < -D3DX_PI)
				{// 向きが円周率を超えた
					m_Rot.z = -D3DX_PI;
				}
				if (m_Rot.z > 0.0f)
				{// 向きが円周率を超えた
					m_Rot.z = 0.0f;
				}

				// 視点の位置を計算する
				m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
				m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
				m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
			}
			else if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_1) == TRUE)
			{// マウスの右クリックのみが押された
			    // 向きをマウスの速度に合わせて回転させる
			    // 縦軸
				m_Rot.y += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisX();
				if (m_Rot.y > D3DX_PI)
				{// 向きが円周率を超えた
					m_Rot.y -= D3DX_PI * 2.0f;
				}
				if (m_Rot.y < -D3DX_PI)
				{// 向きが円周率を超えた
					m_Rot.y += D3DX_PI * 2.0f;
				}

				// 横軸
				m_Rot.z += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisY();
				if (m_Rot.z < -D3DX_PI)
				{// 向きが円周率を超えた
					m_Rot.z = -D3DX_PI;
				}
				if (m_Rot.z > 0.0f)
				{// 向きが円周率を超えた
					m_Rot.z = 0.0f;
				}

				// 注視点の位置を計算する
				m_PosR.x = m_PosV.x + sinf(m_Rot.y) * m_fLength;
				m_PosR.y = m_PosV.y - cosf(m_Rot.z) * m_fLength;
				m_PosR.z = m_PosV.z + cosf(m_Rot.y) * m_fLength;
			}
		}
	}
}

//=============================================================================
//    カメラの設定
//=============================================================================
void CCamera::SetCamera(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが取得できた
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// デバイスの取得
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // ビューポート(描画領域)の設定
			pDevice->SetViewport(&m_ViewPort);

			// プロジェクションマトリックスの初期化
			D3DXMatrixIdentity(&m_MtxProjection);

			// プロジェクションマトリックスを作成
			D3DXMatrixPerspectiveFovLH(&m_MtxProjection,
				D3DXToRadian(CAMERA_ANGEL_VIEW),
				(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
				10.0f,
				20000.0f);

			// プロジェクションマトリックスの設定
			pDevice->SetTransform(D3DTS_PROJECTION, &m_MtxProjection);

			// ビューマトリックスの初期化
			D3DXMatrixIdentity(&m_MtxView);

			// ビューマトリックスを作成
			D3DXMatrixLookAtLH(&m_MtxView,
				&m_PosV,
				&m_PosR,
				&m_VecU);

			// ビューマトリックスの設定
			pDevice->SetTransform(D3DTS_VIEW, &m_MtxView);
		}
	}
}

//=============================================================================
//    カメラの視点の設定処理
//=============================================================================
void CCamera::SetPosV(const D3DXVECTOR3 posV)
{
	m_PosV = posV;
}

//=============================================================================
//    カメラの注視点の設定処理
//=============================================================================
void CCamera::SetPosR(const D3DXVECTOR3 posR)
{
	m_PosR = posR;
}

//=============================================================================
//    カメラの向きの取得処理
//=============================================================================
void CCamera::SetRot(const D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//=============================================================================
//    カメラを動かすかどうかを取得
//=============================================================================
void CCamera::SetMove(const bool bMove)
{
	m_bMove = bMove;
}

//=============================================================================
//    カメラの視点の設定処理
//=============================================================================
D3DXVECTOR3 CCamera::GetPosV(void)
{
	return m_PosV;
}

//=============================================================================
//    カメラの注視点の取得処理
//=============================================================================
D3DXVECTOR3 CCamera::GetPosR(void)
{
	return m_PosR;
}

//=============================================================================
//    カメラの向きの取得処理
//=============================================================================
D3DXVECTOR3 CCamera::GetRot(void)
{
	return m_Rot;
}