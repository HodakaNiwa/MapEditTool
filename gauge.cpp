//*****************************************************************************
//
//     ゲージの処理[gauge.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "gauge.h"
#include "manager.h"
#include "renderer.h"
#include "functionlib.h"
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define GAUGE_LIFE_CUTTIMING  (60)    // 体力ゲージを減らしが始まるタイミング

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************


//*****************************************************************************
//    CGaugeの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CGauge::CGauge(int nPriority, OBJTYPE objType) : CSceneBillboard(nPriority, objType)
{
	// 各種値のクリア
	m_fGaugeRate = 0.0f;   // ゲージの割合
	m_fRightWidth = 0.0f;  // ゲージの左側座標
}

//=============================================================================
//    デストラクタ
//=============================================================================
CGauge::~CGauge()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CGauge *CGauge::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fWidth, float fHeight, int nPriority)
{
	CGauge *pGauge = NULL;                 // ゲージクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pGauge == NULL)
		{// メモリが空になっている
			pGauge = new CGauge(nPriority);
			if (pGauge != NULL)
			{// インスタンスを生成できた
			    // 各種値の代入
				pGauge->SetPos(pos);         // ポリゴンの座標
				pGauge->SetCol(col);         // ポリゴンの色
				pGauge->SetRot(fRot);        // ポリゴンの向き
				pGauge->SetWidth(fWidth);    // ポリゴンの幅
				pGauge->SetHeight(fHeight);  // ポリゴンの高さ

				if (FAILED(pGauge->Init()))
				{// 初期化に失敗した
					return NULL;
				}
			}
			else
			{// インスタンスを生成できなかった
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// これ以上シーンを作成できない
		return NULL;
	}

	return pGauge;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CGauge::Init(void)
{
	// 初期化に失敗した
	if (FAILED(CSceneBillboard::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CGauge::Uninit(void)
{
	// 共通の描画処理
	CSceneBillboard::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CGauge::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CGauge::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			DWORD Lighting;

			// ライティングの設定を外す
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			pDevice->SetRenderState(D3DRS_LIGHTING, false);

			// 共通の描画処理
			CSceneBillboard::Draw();

			// ライティングの設定を元に戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
		}
	}
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CGauge::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-GetWidth(), GetHeight(), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(GetWidth(), GetHeight(), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-GetWidth(), -GetHeight(), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f);

	// 頂点カラー
	pVtx[0].col = GetCol();
	pVtx[1].col = GetCol();
	pVtx[2].col = GetCol();
	pVtx[3].col = GetCol();

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	// 頂点バッファを設定する
	SetVtxBuff(pVtxBuff);

	// 各種値を格納しておく
	m_fRightWidth = -GetWidth();  // ゲージの左側オフセット
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CGauge::SetGaugeRate(const float fGaugeRate)
{
	m_fGaugeRate = fGaugeRate;
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CGauge::SetRightWidth(const float fRightWidth)
{
	m_fRightWidth = fRightWidth;
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
float CGauge::GetGaugeRate(void)
{
	return m_fGaugeRate;
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
float CGauge::GetRightWidth(void)
{
	return m_fRightWidth;
}



//*****************************************************************************
//    CGaugeLifeの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CGaugeLife::CGaugeLife(int nPriority, OBJTYPE objType) : CGauge(nPriority, objType)
{
	// 各種値のクリア
	m_nCounter = 0;            // ゲージの減らしを管理するカウンター
	m_State = STATE_NONE;      // 状態
	m_nLife = 0;               // 体力
	m_pRedBill = NULL;         // 体力ゲージが減った時に表示する赤いゲージ
	m_pWhiteBill = NULL;       // 体力ゲージのフレーム
}

//=============================================================================
//    デストラクタ
//=============================================================================
CGaugeLife::~CGaugeLife()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CGaugeLife *CGaugeLife::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fWidth, float fHeight, int nLife, int nPriority)
{
	CGaugeLife *pGaugeLife = NULL;         // 体力ゲージクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pGaugeLife == NULL)
		{// メモリが空になっている
			pGaugeLife = new CGaugeLife(nPriority);
			if (pGaugeLife != NULL)
			{// インスタンスを生成できた
			    // 各種値の代入
				pGaugeLife->SetPos(pos);         // ポリゴンの座標
				pGaugeLife->SetCol(col);         // ポリゴンの色
				pGaugeLife->SetRot(fRot);        // ポリゴンの向き
				pGaugeLife->SetWidth(fWidth);    // ポリゴンの幅
				pGaugeLife->SetHeight(fHeight);  // ポリゴンの高さ
				pGaugeLife->SetLife(nLife);      // 体力

				if (FAILED(pGaugeLife->Init()))
				{// 初期化に失敗した
					return NULL;
				}
			}
			else
			{// インスタンスを生成できなかった
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// これ以上シーンを作成できない
		return NULL;
	}

	return pGaugeLife;  // インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CGaugeLife::Init(void)
{
	// 初期化に失敗した
	if (FAILED(CGauge::Init()))
	{
		return E_FAIL;
	}

	// 体力ゲージのフレームを生成する
	if (m_pWhiteBill == NULL)
	{
		m_pWhiteBill = CGauge::Create(GetPos(), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, (GetWidth() * 1.04f), (GetHeight() * 1.1f),2);
	}

	// 後ろの赤いゲージを生成する
	if (m_pRedBill == NULL)
	{
		m_pRedBill = CGauge::Create(GetPos(), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, GetWidth(), GetHeight(), 2);
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CGaugeLife::Uninit(void)
{
	// 体力ゲージのフレーム開放処理
	if (m_pWhiteBill != NULL)
	{
		m_pWhiteBill->Uninit();
		m_pWhiteBill = NULL;
	}

	// 赤いゲージの開放処理
	if (m_pRedBill != NULL)
	{
		m_pRedBill->Uninit();
		m_pRedBill = NULL;
	}

	// 共通の描画処理
	CGauge::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CGaugeLife::Update(void)
{
	if (m_State == STATE_CUT)
	{// ゲージを減らす状態だったら
		m_nCounter++;  // カウンターを増やす
		if (m_nCounter >= GAUGE_LIFE_CUTTIMING)
		{// ゲージを減らすタイミングになっている
			// 後ろの赤いゲージを減らす
			if (m_pRedBill != NULL)
			{// 赤いゲージが生成されている
				// 赤いゲージの幅を少しずつ減らす
				float fWidth = m_pRedBill->GetWidth();
				fWidth -= 0.5f;
				if (fWidth <= GetWidth())
				{// 赤いゲージの幅が体力ゲージを下回った
					fWidth = GetWidth();   // 幅を戻す
					m_State = STATE_NONE;  // 状態を戻す
					m_nCounter = 0;        // カウンターを戻す
				}
				m_pRedBill->SetWidth(fWidth);

				// ポリゴンの幅を減らす
				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pRedBill->GetVtxBuff();
				if (pVtxBuff != NULL)
				{// 頂点バッファが生成されている
					// 頂点情報の設定
					VERTEX_3D *pVtx;

					// 頂点バッファをロックし,頂点データへのポインタを取得
					pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

					// 頂点座標
					pVtx[0].pos = D3DXVECTOR3(GetRightWidth(), GetHeight(), 0.0f);
					pVtx[1].pos = D3DXVECTOR3(fWidth, GetHeight(), 0.0f);
					pVtx[2].pos = D3DXVECTOR3(GetRightWidth(), -GetHeight(), 0.0f);
					pVtx[3].pos = D3DXVECTOR3(fWidth, -GetHeight(), 0.0f);

					// 頂点バッファをアンロックする
					pVtxBuff->Unlock();
				}
			}
		}
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CGaugeLife::Draw(void)
{
	// 共通の描画処理
	CGauge::Draw();
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CGaugeLife::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-GetWidth(), GetHeight(), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(GetWidth(), GetHeight(), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-GetWidth(), -GetHeight(), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f);

	// 頂点カラー
	pVtx[0].col = GetCol();
	pVtx[1].col = GetCol();
	pVtx[2].col = GetCol();
	pVtx[3].col = GetCol();

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	// 頂点バッファを設定する
	SetVtxBuff(pVtxBuff);

	// 各種値を格納しておく
	SetRightWidth(-GetWidth());                   // ゲージの左側オフセット
	SetGaugeRate((GetWidth() * 2.0f) / m_nLife);  // ゲージ一本分の割合
}

//=============================================================================
//    ゲージを減らす処理
//=============================================================================
void CGaugeLife::CutGauge(const int nCutValue)
{
	// 状態を設定
	m_State = STATE_CUT;

	// ゲージの幅を減らす
	float fWidth = GetWidth();  // 幅
	fWidth -= GetGaugeRate() * nCutValue;
	SetWidth(fWidth);

	// ポリゴンの幅を減らす
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// 頂点バッファが生成されている
		// 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(GetRightWidth(), GetHeight(), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(GetWidth(), GetHeight(), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(GetRightWidth(), -GetHeight(), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f);

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ゲージを増やす処理
//=============================================================================
void CGaugeLife::AddGauge(const int nAddValue)
{
	// ゲージの幅を増やす
	float fWidth = GetWidth();  // 幅
	fWidth += GetGaugeRate() * nAddValue;
	SetWidth(fWidth);

	// ポリゴンの幅を増やす
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// 頂点バッファが生成されている
	    // 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(GetRightWidth(), GetHeight(), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(GetWidth(), GetHeight(), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(GetRightWidth(), -GetHeight(), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f);

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ゲージの座標をずらす処理
//=============================================================================
void CGaugeLife::SetGaugePos(const D3DXVECTOR3 pos)
{
	SetPos(pos);

	// 赤いゲージが生成されている
	if (m_pRedBill != NULL)
	{
		m_pRedBill->SetPos(pos);
	}

	// ゲージのフレームが生成されている
	if (m_pWhiteBill != NULL)
	{
		m_pWhiteBill->SetPos(pos);
	}
}

//=============================================================================
//    状態設定処理
//=============================================================================
void CGaugeLife::SetState(const STATE State)
{
	m_State = State;
}

//=============================================================================
//    体力設定処理
//=============================================================================
void CGaugeLife::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    状態取得処理
//=============================================================================
CGaugeLife::STATE CGaugeLife::GetState(void)
{
	return m_State;
}

//=============================================================================
//    体力取得処理
//=============================================================================
int CGaugeLife::GetLife(void)
{
	return m_nLife;
}