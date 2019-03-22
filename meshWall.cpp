//*****************************************************************************
//
//     メッシュウォールの処理[meshWall.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "meshWall.h"
#include "manager.h"
#include "renderer.h"
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************


//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************


//*****************************************************************************
//    CMeshWallの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CMeshWall::CMeshWall(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_nTexIdx = 0;                              // メッシュウォールのテクスチャ番号
	m_pVtxBuff = NULL;                          // 頂点バッファへのポインタ
	m_pIdxBuff = NULL;                          // インデックスバッファへのポインタ
	m_pTexture = NULL;	                        // テクスチャへのポインタ
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // メッシュフィールドの座標
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // メッシュフィールドの向き
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // メッシュフィールドの色
	m_fWidth = 0.0f;                            // 1ブロック分の幅
	m_fHeight = 0.0f;                           // 1ブロック分高さ
	m_nXBlock = 0;                              // 横の分割数
	m_nYBlock = 0;                              // 縦の分割数
	m_nNumVertex = 0;                           // 頂点数
	m_nNumIndex = 0;                            // インデックス数
	m_nNumPolygon = 0;                          // ポリゴン数
	D3DXMatrixIdentity(&m_MtxWorld);            // ワールドマトリックス
}

//=============================================================================
//    デストラクタ
//=============================================================================
CMeshWall::~CMeshWall()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CMeshWall *CMeshWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nYBlock, int nTexSplitU, int nTexSplitV, int nTexIdx, int nPriority)
{
	CMeshWall *pMeshWall = NULL;           // メッシュウォールクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pMeshWall == NULL)
		{// メモリが空になっている
			pMeshWall = new CMeshWall(nPriority);
			if (pMeshWall != NULL)
			{// インスタンスを生成できた
			    // 各種値の代入
				pMeshWall->SetTexIdx(nTexIdx);            // メッシュウォールのテクスチャ番号
				pMeshWall->SetPos(pos);                   // メッシュウォールの座標
				pMeshWall->SetRot(rot);                   // メッシュウォールの向き
				pMeshWall->SetCol(col);                   // メッシュウォールの色
				pMeshWall->SetWidth(fWidth);              // 1ブロック分の幅
				pMeshWall->SetHeight(fHeight);            // 1ブロック分の高さ
				pMeshWall->SetXBlock(nXBlock);            // 横の分割数
				pMeshWall->SetYBlock(nYBlock);            // 縦の分割数
				pMeshWall->SetTexSplitU(nTexSplitU);      // テクスチャU座標の分割数
				pMeshWall->SetTexSplitV(nTexSplitV);      // テクスチャV座標の分割数

				if (FAILED(pMeshWall->Init()))
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

	return pMeshWall;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CMeshWall::Init(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // 作成に必要な頂点数,インデックス数,ポリゴン数を計算
			m_nNumVertex = (m_nXBlock + 1) * (m_nYBlock + 1);                                                               // 頂点数を計算
			m_nNumIndex = ((m_nXBlock + 1) * (m_nYBlock + 1)) + (2 * (m_nYBlock - 1)) + (m_nXBlock + 1) * (m_nYBlock - 1);  // インデックス数を設定
			m_nNumPolygon = ((m_nXBlock * m_nYBlock) * 2) + ((m_nYBlock - 1) * 4);                                          // 描画するポリゴン数を設定

			// 頂点バッファの生成
			MakeVertex(pDevice);

			// インデックスバッファの生成
			MakeIndex(pDevice);
		}
		else
		{// デバイスを取得できなかった
			return E_FAIL;
		}
	}
	else
	{// レンダリングクラスが生成されていない
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CMeshWall::Uninit(void)
{
	// 頂点バッファの破棄
	DIRECT_RELEASE(m_pVtxBuff);

	// インデックスバッファの破棄
	DIRECT_RELEASE(m_pIdxBuff);

	// リリース処理
	CScene::Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CMeshWall::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CMeshWall::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // ワールドマトリックスの設定処理
			SetMtxWorld(pDevice);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

			// インデックスバッファをデバイスのデータストリームにバインド
			pDevice->SetIndices(m_pIdxBuff);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, m_pTexture);

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);
		}
	}
}

//=============================================================================
//    ワールドマトリックス計算処理
//=============================================================================
void CMeshWall::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot, mtxTrans; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CMeshWall::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 生成に必要な変数を宣言
	D3DXCOLOR col = GetCol();
	float XPos = -(m_fWidth * m_nXBlock) / 2;  // X座標を左に設定
	float YPos = (m_fHeight * m_nYBlock) / 2;  // Y座標を奥に設定
	float fTexU = 0.0f;                        // テクスチャのU座標を右上に設定
	float fTexV = 0.0f;                        // テクスチャのV座標を右上に設定

	for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
	{// 垂直方向の分割数 + 1だけ繰り返し
		for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
		{// 水平方向の分割数 + 1だけ繰り返し
		    // 頂点座標
			pVtx[nCntH].pos = D3DXVECTOR3(XPos, YPos, 0.0f);

			// 法線ベクトル
			pVtx[nCntH].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// 頂点カラー
			pVtx[nCntH].col = col;

			// テクスチャ座標
			pVtx[nCntH].tex = D3DXVECTOR2(fTexU, fTexV);

			XPos += m_fWidth;                              // X座標を横に進める
			fTexU += (1.0f / (m_nXBlock)) * m_nTexSplitU;  // テクスチャU座標を進める
		}
		XPos -= m_fWidth * (m_nXBlock + 1);                               // X座標を進めていた分戻す
		YPos -= m_fHeight;                                                // Y座標を下に進める
		fTexU -= ((1.0f / (m_nXBlock)) * m_nTexSplitU) * (m_nXBlock + 1); // テクスチャU座標を進めていた分戻す
		fTexV += ((1.0f / m_nYBlock) * m_nTexSplitV);                     // テクスチャV座標を進める

		// ポインタを進める
		pVtx += m_nXBlock + 1;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}

//=============================================================================
//    インデックスバッファ生成処理
//=============================================================================
void CMeshWall::MakeIndex(const LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	WORD *pIdx;       // インデックスデータへのポインタ
	int nCntIdx = 0;  // インデックス番号

	// インデックスバッファをロックし,インデックスデータへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdxY = 0; nCntIdxY < m_nYBlock; nCntIdxY++)
	{// 縦の分割数分繰り返し
		for (int nCntIdxX = 0; nCntIdxX < m_nXBlock + 1; nCntIdxX++, nCntIdx++)
		{// 横の分割数+１繰り返し
		 // 上下の二つ分インデックス番号を設定
			pIdx[0] = nCntIdx + m_nXBlock + 1; // 下側
			pIdx[1] = nCntIdx;                 // 上側

			pIdx += 2;  // 2つ分進める
			if (nCntIdxY < m_nYBlock - 1 && nCntIdxX == m_nXBlock)
			{// 1 , 下側が縦の分割数の最下層ではない
			 // 2 , 横の分割数分設定が終わった
				pIdx[0] = nCntIdx;                       // 上側
				pIdx[1] = nCntIdx + (m_nXBlock + 1) + 1; // 次の下側

				pIdx += 2; // 2つ分進める
			}
		}
	}

	// インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();
}

//=============================================================================
//    テクスチャ番号設定処理
//=============================================================================
void CMeshWall::SetTexIdx(const int nTexIdx)
{
	m_nTexIdx = nTexIdx;
}

//=============================================================================
//    テクスチャ設定処理
//=============================================================================
void CMeshWall::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    頂点バッファ設定処理
//=============================================================================
void CMeshWall::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
//    インデックスバッファ設定処理
//=============================================================================
void CMeshWall::SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff)
{
	m_pIdxBuff = pIdxBuff;
}

//=============================================================================
//    メッシュウォールの座標設定処理
//=============================================================================
void CMeshWall::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    メッシュウォールの向き設定処理
//=============================================================================
void CMeshWall::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    メッシュウォールの色設定処理
//=============================================================================
void CMeshWall::SetCol(const D3DXCOLOR col)
{
	// 色を設定
	m_Col = col;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	    // 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
		{// 垂直方向の分割数 + 1だけ繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// 水平方向の分割数 + 1だけ繰り返し
			    // 頂点カラー
				pVtx[nCntH].col = m_Col;
			}
			// ポインタを進める
			pVtx += m_nXBlock + 1;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    1ブロック分の幅設定処理
//=============================================================================
void CMeshWall::SetWidth(const float fWidth)
{
	// 幅を設定
	m_fWidth = fWidth;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	    // 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float XPos = -(m_fWidth * m_nXBlock) / 2; // X座標を左に設定

		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
		{// 垂直方向の分割数 + 1だけ繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// 水平方向の分割数 + 1だけ繰り返し
				pVtx[nCntH].pos.x = XPos;

				XPos += m_fWidth;   // X座標を横に進める
			}
			XPos -= m_fWidth * (m_nXBlock + 1); // X座標を進めていた分戻す

			// ポインタを進める
			pVtx += m_nXBlock + 1;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    1ブロック分の高さ設定処理
//=============================================================================
void CMeshWall::SetHeight(const float fHeight)
{
	// 高さを設定
	m_fHeight = fHeight;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	    // 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float YPos = (m_fHeight * m_nYBlock) / 2; // Y座標を奥に設定

		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
		{// 垂直方向の分割数 + 1だけ繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// 水平方向の分割数 + 1だけ繰り返し
				pVtx[nCntH].pos.y = YPos;
			}
			YPos -= m_fHeight;   // Y座標を下に進める

			// ポインタを進める
			pVtx += m_nXBlock + 1;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    横の分割数設定処理
//=============================================================================
void CMeshWall::SetXBlock(const int nXBlock)
{
	m_nXBlock = nXBlock;

	// 頂点バッファの破棄
	DIRECT_RELEASE(m_pVtxBuff);

	// インデックスバッファの破棄
	DIRECT_RELEASE(m_pIdxBuff);

	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // 作成に必要な頂点数,インデックス数,ポリゴン数を計算
			m_nNumVertex = (m_nXBlock + 1) * (m_nYBlock + 1);                                                               // 頂点数を計算
			m_nNumIndex = ((m_nXBlock + 1) * (m_nYBlock + 1)) + (2 * (m_nYBlock - 1)) + (m_nXBlock + 1) * (m_nYBlock - 1);  // インデックス数を設定
			m_nNumPolygon = ((m_nXBlock * m_nYBlock) * 2) + ((m_nYBlock - 1) * 4);                                          // 描画するポリゴン数を設定

			// 頂点バッファの生成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&m_pVtxBuff,
				NULL);

			// 頂点情報の設定
			VERTEX_3D *pVtx;

			// 頂点バッファをロックし,頂点データへのポインタを取得
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// 生成に必要な変数を宣言
			D3DXCOLOR col = GetCol();
			float XPos = -(m_fWidth * m_nXBlock) / 2;  // X座標を左に設定
			float YPos = (m_fHeight * m_nYBlock) / 2;  // Y座標を奥に設定
			float fTexU = 0.0f;                        // テクスチャのU座標を右上に設定
			float fTexV = 0.0f;                        // テクスチャのV座標を右上に設定

			for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
			{// 垂直方向の分割数 + 1だけ繰り返し
				for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
				{// 水平方向の分割数 + 1だけ繰り返し
				    // 頂点座標
					pVtx[nCntH].pos = D3DXVECTOR3(XPos, YPos, 0.0f);

					// 法線ベクトル
					pVtx[nCntH].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

					// 頂点カラー
					pVtx[nCntH].col = col;

					// テクスチャ座標
					pVtx[nCntH].tex = D3DXVECTOR2(fTexU, fTexV);

					XPos += m_fWidth;                              // X座標を横に進める
					fTexU += (1.0f / (m_nXBlock)) * m_nTexSplitU;  // テクスチャU座標を進める
				}
				XPos -= m_fWidth * (m_nXBlock + 1);                               // X座標を進めていた分戻す
				YPos -= m_fHeight;                                                // Y座標を下に進める
				fTexU -= ((1.0f / (m_nXBlock)) * m_nTexSplitU) * (m_nXBlock + 1); // テクスチャU座標を進めていた分戻す
				fTexV += ((1.0f / m_nYBlock) * m_nTexSplitV);                     // テクスチャV座標を進める

				// ポインタを進める
				pVtx += m_nXBlock + 1;
			}

			// 頂点バッファをアンロックする
			m_pVtxBuff->Unlock();

			// インデックスバッファの生成
			MakeIndex(pDevice);
		}
	}
}

//=============================================================================
//    縦の分割数設定処理
//=============================================================================
void CMeshWall::SetYBlock(const int nYBlock)
{
	m_nYBlock = nYBlock;

	// 頂点バッファの破棄
	DIRECT_RELEASE(m_pVtxBuff);

	// インデックスバッファの破棄
	DIRECT_RELEASE(m_pVtxBuff);

	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // 作成に必要な頂点数,インデックス数,ポリゴン数を計算
			m_nNumVertex = (m_nXBlock + 1) * (m_nYBlock + 1);                                                               // 頂点数を計算
			m_nNumIndex = ((m_nXBlock + 1) * (m_nYBlock + 1)) + (2 * (m_nYBlock - 1)) + (m_nXBlock + 1) * (m_nYBlock - 1);  // インデックス数を設定
			m_nNumPolygon = ((m_nXBlock * m_nYBlock) * 2) + ((m_nYBlock - 1) * 4);                                          // 描画するポリゴン数を設定

			// 頂点バッファの生成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&m_pVtxBuff,
				NULL);

			// 頂点情報の設定
			VERTEX_3D *pVtx;

			// 頂点バッファをロックし,頂点データへのポインタを取得
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// 生成に必要な変数を宣言
			D3DXCOLOR col = GetCol();
			float XPos = -(m_fWidth * m_nXBlock) / 2;  // X座標を左に設定
			float YPos = (m_fHeight * m_nYBlock) / 2;  // Y座標を奥に設定
			float fTexU = 0.0f;                        // テクスチャのU座標を右上に設定
			float fTexV = 0.0f;                        // テクスチャのV座標を右上に設定

			for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
			{// 垂直方向の分割数 + 1だけ繰り返し
				for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
				{// 水平方向の分割数 + 1だけ繰り返し
				 // 頂点座標
					pVtx[nCntH].pos = D3DXVECTOR3(XPos, YPos, 0.0f);

					// 法線ベクトル
					pVtx[nCntH].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

					// 頂点カラー
					pVtx[nCntH].col = col;

					// テクスチャ座標
					pVtx[nCntH].tex = D3DXVECTOR2(fTexU, fTexV);

					XPos += m_fWidth;                              // X座標を横に進める
					fTexU += (1.0f / (m_nXBlock)) * m_nTexSplitU;  // テクスチャU座標を進める
				}
				XPos -= m_fWidth * (m_nXBlock + 1);                               // X座標を進めていた分戻す
				YPos -= m_fHeight;                                                // Y座標を下に進める
				fTexU -= ((1.0f / (m_nXBlock)) * m_nTexSplitU) * (m_nXBlock + 1); // テクスチャU座標を進めていた分戻す
				fTexV += ((1.0f / m_nYBlock) * m_nTexSplitV);                     // テクスチャV座標を進める

				// ポインタを進める
				pVtx += m_nXBlock + 1;
			}

			// 頂点バッファをアンロックする
			m_pVtxBuff->Unlock();

			// インデックスバッファの生成
			MakeIndex(pDevice);
		}
	}
}

//=============================================================================
//    テクスチャU座標の分割数処理
//=============================================================================
void CMeshWall::SetTexSplitU(const int nTexSplitU)
{
	m_nTexSplitU = nTexSplitU;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	    // 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fTexU = 0.0f;   // テクスチャU座標

		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
		{// 垂直方向の分割数 + 1だけ繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// 水平方向の分割数 + 1だけ繰り返し
				pVtx[nCntH].tex.x = fTexU;
				fTexU += (1.0f / (m_nXBlock)) * m_nTexSplitU;
			}
			fTexU -= ((1.0f / (m_nXBlock)) * m_nTexSplitU) * (m_nXBlock + 1);

			// ポインタを進める
			pVtx += m_nXBlock + 1;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    テクスチャV座標の分割数設定処理
//=============================================================================
void CMeshWall::SetTexSplitV(const int nTexSplitV)
{
	m_nTexSplitV = nTexSplitV;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	    // 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fTexV = 0.0f;   // テクスチャV座標

		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
		{// 垂直方向の分割数 + 1だけ繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// 水平方向の分割数 + 1だけ繰り返し
				pVtx[nCntH].tex.y = fTexV;
			}
			fTexV += ((1.0f / m_nYBlock) * m_nTexSplitV);

			// ポインタを進める
			pVtx += m_nXBlock + 1;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ワールドマトリックス設定処理
//=============================================================================
void CMeshWall::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    テクスチャ番号取得処理
//=============================================================================
int CMeshWall::GetTexIdx(void)
{
	return m_nTexIdx;
}

//=============================================================================
//    頂点バッファ取得処理
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CMeshWall::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
//    インデックスバッファ取得処理
//=============================================================================
LPDIRECT3DINDEXBUFFER9 CMeshWall::GetIdxBuff(void)
{
	return m_pIdxBuff;
}

//=============================================================================
//    テクスチャ取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 CMeshWall::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    メッシュフィールドの座標取得処理
//=============================================================================
D3DXVECTOR3 CMeshWall::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    メッシュフィールドの向き取得処理
//=============================================================================
D3DXVECTOR3 CMeshWall::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    メッシュフィールドの色取得処理
//=============================================================================
D3DXCOLOR CMeshWall::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    1ブロック分の幅取得処理
//=============================================================================
float CMeshWall::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    1ブロック分の高さ取得処理
//=============================================================================
float CMeshWall::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    横の分割数取得処理
//=============================================================================
int CMeshWall::GetXBlock(void)
{
	return m_nXBlock;
}

//=============================================================================
//    奥行の分割数取得処理
//=============================================================================
int CMeshWall::GetYBlock(void)
{
	return m_nYBlock;
}

//=============================================================================
//    テクスチャU座標の分割数取得処理
//=============================================================================
int CMeshWall::GetTexSplitU(void)
{
	return m_nTexSplitU;
}

//=============================================================================
//    テクスチャV座標の分割数取得処理
//=============================================================================
int CMeshWall::GetTexSplitV(void)
{
	return m_nTexSplitV;
}

//=============================================================================
//    頂点数取得処理
//=============================================================================
int CMeshWall::GetNumVertex(void)
{
	return m_nNumVertex;
}

//=============================================================================
//    インデックス数取得処理
//=============================================================================
int CMeshWall::GetNumIndex(void)
{
	return m_nNumIndex;
}

//=============================================================================
//    ポリゴン数取得処理
//=============================================================================
int CMeshWall::GetNumPolygon(void)
{
	return m_nNumPolygon;
}

//=============================================================================
//    ワールドマトリックス取得処理
//=============================================================================
D3DXMATRIX CMeshWall::GetMtxWorld(void)
{
	return m_MtxWorld;
}


//*****************************************************************************
//    CSetMeshWallの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CSetMeshWall::CSetMeshWall(int nPriority, OBJTYPE objType) : CMeshWall(nPriority, objType)
{
	// 各種値のクリア
}

//=============================================================================
//    デストラクタ
//=============================================================================
CSetMeshWall::~CSetMeshWall()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CSetMeshWall *CSetMeshWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nYBlock, int nTexSplitU, int nTexSplitV, int nTexIdx, int nPriority)
{
	CSetMeshWall *pSetMeshWall = NULL;     // 配置用メッシュウォールクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pSetMeshWall == NULL)
		{// メモリが空になっている
			pSetMeshWall = new CSetMeshWall(nPriority);
			if (pSetMeshWall != NULL)
			{// インスタンスを生成できた
			    // 各種値の代入
				pSetMeshWall->SetTexIdx(nTexIdx);            // 配置用メッシュウォールのテクスチャ番号
				pSetMeshWall->SetPos(pos);                   // 配置用メッシュウォールの座標
				pSetMeshWall->SetRot(rot);                   // 配置用メッシュウォールの向き
				pSetMeshWall->SetCol(col);                   // 配置用メッシュウォールの色
				pSetMeshWall->SetWidth(fWidth);              // 1ブロック分の幅
				pSetMeshWall->SetHeight(fHeight);            // 1ブロック分の高さ
				pSetMeshWall->SetXBlock(nXBlock);            // 横の分割数
				pSetMeshWall->SetYBlock(nYBlock);            // 縦の分割数
				pSetMeshWall->SetTexSplitU(nTexSplitU);      // テクスチャU座標の分割数
				pSetMeshWall->SetTexSplitV(nTexSplitV);      // テクスチャV座標の分割数

				if (FAILED(pSetMeshWall->Init()))
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

	return pSetMeshWall;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CSetMeshWall::Init(void)
{
	// 色を透明にしておく
	D3DXCOLOR col = GetCol();
	col.a = -0.7f;
	SetCol(col);

	// 共通の初期化処理
	if (FAILED(CMeshWall::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CSetMeshWall::Uninit(void)
{
	// 共通の終了処理
	CMeshWall::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CSetMeshWall::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CSetMeshWall::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// レンダリングクラスが取得できた
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			DWORD Lighting;

			// ライティングをしない設定に
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			pDevice->SetRenderState(D3DRS_LIGHTING, false);

			// 共通の描画処理
			CMeshWall::Draw();

			// ライティングの設定を戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
		}
	}
}