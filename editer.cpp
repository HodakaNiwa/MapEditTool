//*****************************************************************************
//
//     エディターの処理[editer.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "editer.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "debuglog.h"
#include "input.h"
#include "functionlib.h"
#include "map.h"
#include "meshField.h"
#include "meshWall.h"
#include "billboardObject.h"
#include "emitter.h"
#include "enemy.h"
#include "item.h"
#include "modelManager.h"
#include "effectManager.h"
#include "textureManager.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define EDITER_SYSTEM_FILENAME "data/TEXT/system.ini"   // エディターのシステムファイル名
#define EDITER_PLAY_TIMING     (60)                     // 停止状態から再生状態になるまでの時間

// 値を読み取る変数
#define EFFECT_FILENAME        "EFFECT_FILENAME = "     // エフェクトデータのスクリプトファイル名
#define MAP_FILENAME           "MAP_FILENAME = "        // マップデータのスクリプトファイル名
#define NUM_TEXTURE            "NUM_TEXTURE = "         // 読み込むテクスチャの数
#define TEXTURE_FILENAME       "TEXTURE_FILENAME = "    // 読み込むテクスチャのファイル名
#define NUM_MODEL              "NUM_MODEL = "           // 読み込むモデルの数
#define MODEL_FILENAME         "MODEL_FILENAME = "      // 読み込むモデルファイル名

//=============================================================================
//    コンストラクタ
//=============================================================================
CEditer::CEditer()
{
	// 各種値のクリア
	// 共通
	m_nCounter = 0;                                    // 状態を管理するカウンター
	m_pMap = NULL;                                     // マップクラスへのポインタ
	m_pSetObject = NULL;                               // 配置物へのポインタ
	m_pSetObjectMove = NULL;                           // 動く配置物へのポインタ
	m_pSetBillboardObj = NULL;                         // 配置できるビルボードへのポインタ
	m_pSetEmitter = NULL;                              // 配置できるエフェクトへのポインタ
	m_pSetEnemy = NULL;                                // 配置できる敵へのポインタ
	m_pSetWall = NULL;                                 // 配置できる壁へのポインタ
	m_pSetItem = NULL;                                 // 配置できるアイテムへのポインタ
	m_pModelManager = NULL;                            // モデル管轄クラスへのポインタ
	m_pEffectManager = NULL;                           // エフェクト管轄クラスへのポインタ
	m_pEnemyAssmbly = NULL;                            // 敵管轄まとめクラスへのポインタ
	m_Type = TYPE_OBJECT;                              // 現在配置できる配置物の種類
	m_SetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // 配置オブジェクトの座標
	m_SetRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // 配置オブジェクトの向き

	// 配置モデル用
	m_ModelColType = CObject::COL_TYPE_SQUARE;         // 配置できるモデルの当たり判定の種類
	m_ModelScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // 配置できるモデルの大きさの倍率
	m_fModelColRange = 0.0f;                           // 配置できるモデルの当たり判定を取る範囲
	m_ModelMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // 配置できるモデルの動き
	m_nModelMaxMoveCounter = 0;                        // 配置できるモデルの移動量を切り替える時間
	m_ModelRollSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 配置できるモデルの回転するスピード
	m_nSelectModelIdx = 0;                             // 現在配置できるモデルの番号

	// ビルボード用
	m_BillCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);     // 配置できるビルボードの色
	m_fBillRot = 0.0f;                                 // 配置できるビルボードの向き
	m_fBillWidth = 50.0f;                              // 配置できるビルボードの幅
	m_fBillHeight = 50.0f;                             // 配置できるビルボードの高さ
	m_nBillTexIdx = 0;                                 // 配置できるビルボードのテクスチャの番号
	m_bBillLighting = false;                           // 配置できるビルボードのライティングするかしないか
	m_bBillDrawAddtive = false;                        // 配置できるビルボードの加算合成で描画するかしないか

	// エフェクト用
	m_nEffectType = 0;                                 // 配置できるエフェクトの種類番号

	// 敵用
	m_nEnemyType = 0;                                  // 配置できる敵の種類
	m_nEnemyLife = 1;                                  // 配置できる敵の体力
	m_nEnemyScore = 1;                                 // 配置できる敵のスコア

	// 壁用
	m_nWallTexIdx = 0;                                 // 配置できる壁のテクスチャ番号
	m_WallCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);     // 配置できる壁の色
	m_fWallWidth = 10.0f;                              // 配置できる壁の幅
	m_fWallHeight = 10.0f;                             // 配置できる壁の高さ
	m_nWallXBlock = 1;                                 // 配置できる壁の横の分割数
	m_nWallYBlock = 1;                                 // 配置できる壁の縦の分割数
	m_nWallTexSplitU = 1;                              // 配置できる壁のテクスチャU座標の分割数
	m_nWallTexSplitV = 1;                              // 配置できる壁のテクスチャV座標の分割数

	// アイテム用
	m_ItemCol = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);        // 配置できるアイテムの色
	m_fItemColRange = 0.0f;                            // 配置できるアイテムの当たり判定を取る範囲
	m_fItemRot = 0.0f;                                 // 配置できるアイテムの向き
	m_fItemWidth = 2.0f;                               // 配置できるアイテムの幅
	m_fItemHeight = 2.0f;                              // 配置できるアイテムの高さ
	m_nItemTexIdx = 0;                                 // 配置できるアイテムのテクスチャの番号
	m_nItemType = 0;                                   // 配置できるアイテムの種類番号
	m_bItemLighting = false;                           // 配置できるアイテムのライティングするかしないか
	m_bItemDrawAddtive = false;                        // 配置できるアイテムの加算合成で描画するかしないか
}

//=============================================================================
//    デストラクタ
//=============================================================================
CEditer::~CEditer()
{

}

//=============================================================================
//    初期化処理
//=============================================================================
void CEditer::Init(void)
{
	// エディターの初期化情報を外部から読み込む
	FILE *pFile = NULL;   // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態である
		pFile = fopen(EDITER_SYSTEM_FILENAME, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // スクリプト読み込み用の変数を用意
			char *pLine = NULL;                 // 1行分読み取り用
			char *pStrCur = NULL;               // 現在の先頭ポインタ
			char *pStr = NULL;                  // 先頭ポインタ保存用
			int nCntTex = 0;                    // テクスチャを読み込んだ回数
			int nCntModel = 0;                  // モデルを読み込んだ回数
			LPDIRECT3DTEXTURE9 pTexture;        // テクスチャへのポインタ
			LPD3DXMESH pMesh = NULL;            // メッシュ情報へのポインタ
			LPD3DXBUFFER pBuffMat = NULL;       // マテリアル情報へのポインタ
			DWORD nNumMat = 0;                  // マテリアル情報の数
			char aMapFileName[256] = "\0";      // マップデータのスクリプトファイル名
			char aEffectFileName[256] = "\0";   // エフェクトデータのスクリプトファイル名
			if (pLine == NULL && pStr == NULL)
			{// メモリが確保できる状態である
				pLine = new char[256];
				pStr = new char[256];
				if (pLine != NULL && pStr != NULL)
				{// メモリを確保できた
				 // 文字列を初期化
					strcpy(pLine, "\0");
					strcpy(pStr, "\0");

					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
					if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
					{// スクリプト読み込み開始の合図だった
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, MAP_FILENAME) == 0)
							{// マップデータのスクリプトファイル名だった
							    // マップデータのスクリプトファイル名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, MAP_FILENAME);
								strcpy(aMapFileName, pStr);

								// ファイルから設置できるモデル情報を読み取る
								FILE *pFileMap = NULL;  // ファイルポインタ
								if (pFileMap == NULL)
								{// ファイルポインタを確保できる状態にある
									pFileMap = fopen(aMapFileName, "r");
									if (pFileMap != NULL)
									{// ファイルポインタを確保できた
										pStrCur = CFunctionLib::ReadLine(pFileMap, pLine);  // 有効な文字列を見つける
										if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
										{// スクリプト読み込み開始の合図だった
											while (1)
											{// ループ開始
												pStrCur = CFunctionLib::ReadLine(pFileMap, pLine);  // 有効な文字列を見つける
												if (CFunctionLib::Memcmp(pStrCur, NUM_TEXTURE) == 0)
												{// テクスチャの個数情報があった
													m_nNumTex = CFunctionLib::ReadInt(pStrCur, NUM_TEXTURE);
													if (m_nNumTex >= 1)
													{// モデル数が1つ以上ある
														m_pTextureManager = CTextureManager::Create(m_nNumTex);
													}
												}
												else if (CFunctionLib::Memcmp(pStrCur, TEXTURE_FILENAME) == 0)
												{// テクスチャのファイルパス名情報があった
												    // テクスチャのファイルパス名を読み取る
													pStr = CFunctionLib::ReadString(pStrCur, pStr, TEXTURE_FILENAME);

													// テクスチャの読み込み
													D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), pStr, &pTexture);
													if (pTexture != NULL)
													{// テクスチャを読み込めた
														m_pTextureManager->BindTexture(pTexture, nCntTex);
														m_pTextureManager->SetFileName(pStr, nCntTex);
														pTexture = NULL;
														nCntTex++;
													}
												}
												else if (CFunctionLib::Memcmp(pStrCur, NUM_MODEL) == 0)
												{// テクスチャの個数情報があった
													m_nNumModel = CFunctionLib::ReadInt(pStrCur, NUM_MODEL);
													if (m_nNumModel >= 1)
													{// モデル数が1つ以上ある
														m_pModelManager = CModelManager::Create(m_nNumModel);
													}
												}
												else if (CFunctionLib::Memcmp(pStrCur, MODEL_FILENAME) == 0)
												{// モデルのファイルパス名情報があった
												    // モデルのファイルパス名を読み取る
													pStr = CFunctionLib::ReadString(pStrCur, pStr, MODEL_FILENAME);

													// モデルの読み込み
													// xファイルの読み込み
													D3DXLoadMeshFromX(pStr,
														D3DXMESH_SYSTEMMEM,
														CManager::GetRenderer()->GetDevice(),
														NULL,
														&pBuffMat,
														NULL,
														&nNumMat,
														&pMesh);

													// モデル管轄クラスにポインタを設定する
													m_pModelManager->SetMesh(pMesh, nCntModel);
													m_pModelManager->SetMaterial(CManager::GetRenderer()->GetDevice(), pBuffMat, nNumMat, nCntModel);

													// 使用した変数を初期化しておく
													pMesh = NULL;
													pBuffMat = NULL;
													nNumMat = 0;

													nCntModel++;  // モデルを読み込んだ回数を増やす
												}
												else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
												{// スクリプト読み込み終了の合図だった
													break;  // ループ終了
												}
											}
										}
										fclose(pFileMap);
									}
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, EFFECT_FILENAME) == 0)
							{// エフェクトデータのスクリプトファイル名だった
							    // エフェクトデータのスクリプトファイル名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, EFFECT_FILENAME);
								strcpy(aEffectFileName, pStr);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// スクリプト読み込み終了の合図だった
								break;  // ループ終了
							}
						}
					}

					// 必要なクラスを作成する
					// エフェクト管轄クラス
				    m_pEffectManager = CEffectManager::Create(aEffectFileName);

					// マップ
					m_pMap = CMap::Create(aMapFileName);

					// モデル配置用クラス
					m_pSetObject = CSetObject::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f) ,D3DXVECTOR3(0.0f, 0.0f, 0.0f));
					if (m_pSetObject != NULL)
					{
						m_pSetObject->BindModel(m_pModelManager->GetMesh(0), m_pModelManager->GetBuffMat(0), m_pModelManager->GetNumMat(0), m_pModelManager->GetVtxMax(0), m_pModelManager->GetVtxMin(0), m_pModelManager->GetTexture(0));
						m_pSetObject->SetTransParent(0.5f);
					}
					m_ModelScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

					// 敵管轄クラスを見つける
					if (m_pEnemyAssmbly == NULL)
					{// メモリを確保できる状態である
						m_pEnemyAssmbly = new CEnemyAssembly;
						if (m_pEnemyAssmbly != NULL)
						{// メモリを確保できた
							int nCntEnemy = 0;
							CScene *pScene = NULL;
							CScene *pSceneNext = NULL;
							CEnemyManager *pEnemyManager = NULL;
							for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
							{// 優先順位の数だけ繰り返し
								pScene = CScene::GetTop(nCntPriority);
								while (pScene != NULL)
								{// メモリが空になるまで繰り返し
									pSceneNext = pScene->GetNext();
									if (pScene->GetObjType() == CScene::OBJTYPE_ENEMYMANAGER)
									{// 敵管轄クラスへのポインタだった
										pEnemyManager = (CEnemyManager*)pScene;
										if (pEnemyManager != NULL)
										{// 敵管轄クラスのポインタが取得できた
											m_pEnemyAssmbly->SetEnemyManager(pEnemyManager, nCntEnemy);
											nCntEnemy++;
										}
									}
									pScene = pSceneNext;
								}
							}
						}
					}

					// メモリの開放
					if (pStr != NULL)
					{// メモリが確保されている
						delete[] pStr;
						pStr = NULL;
					}
					if (pLine != NULL)
					{// メモリが確保されている
						delete[] pLine;
						pLine = NULL;
					}
				}
			}
		}
	}
}

//=============================================================================
//    終了処理
//=============================================================================
void CEditer::Uninit(void)
{
	// モデル管轄クラスを開放する
	if (m_pModelManager != NULL)
	{
		m_pModelManager->Uninit();

		// メモリの開放
		delete m_pModelManager;
		m_pModelManager = NULL;
	}

	// テクスチャ管轄クラスへのポインタを開放する
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();

		// メモリの開放
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	// 敵管轄まとめクラスへのポインタを空にする
	if (m_pEnemyAssmbly != NULL)
	{
		// メモリの開放
		delete m_pEnemyAssmbly;
		m_pEnemyAssmbly = NULL;
	}

	// 配置物クラスを開放する
	if (m_pSetObject != NULL)
	{
		m_pSetObject->Uninit();
		m_pSetObject = NULL;
	}

	// 動く配置物クラスを開放する
	if (m_pSetObjectMove != NULL)
	{
		m_pSetObjectMove->Uninit();
		m_pSetObjectMove = NULL;
	}

	// 配置ビルボードクラスを開放する
	if (m_pSetBillboardObj != NULL)
	{
		m_pSetBillboardObj->Uninit();
		m_pSetBillboardObj = NULL;
	}

	// 配置用エミッタクラスを開放する
	if (m_pSetEmitter != NULL)
	{
		m_pSetEmitter->Uninit();
		m_pSetEmitter = NULL;
	}

	// 配置用敵クラスを開放する
	if (m_pSetEnemy != NULL)
	{
		m_pSetEnemy->Uninit();
		m_pSetEnemy = NULL;
	}

	// 配置用壁クラスを開放する
	if (m_pSetWall != NULL)
	{
		m_pSetWall->Uninit();
		m_pSetWall = NULL;
	}

	// 配置用アイテムクラスを開放する
	if (m_pSetItem != NULL)
	{
		m_pSetItem->Uninit();
		m_pSetItem = NULL;
	}

	// マップクラスを開放する
	if (m_pMap != NULL)
	{
		m_pMap->Uninit();

		delete m_pMap;
		m_pMap = NULL;
	}
}

//=============================================================================
//    更新処理
//=============================================================================
void CEditer::Update(void)
{
	if (m_Type == TYPE_EFFECT)
	{// エフェクト生成状態だったら
		if (m_EffectState == EFFECTSTATE_STOP)
		{// 再生停止状態だったら
			m_nCounter++;
			if (m_nCounter % EDITER_PLAY_TIMING == 0)
			{// 再生状態になる状態になった
				if (m_pSetEmitter == NULL)
				{
					m_pSetEmitter = m_pEffectManager->SetMapEditEffect(m_SetPos, m_SetRot, m_nEffectType);
					m_EffectState = EFFECTSTATE_PLAY;
					m_nCounter = 0;
				}
			}
		}
	}

	// キーボードの取得
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	if (pKeyboard != NULL)
	{// キーボードが取得できた
		if (pKeyboard->GetTrigger(DIK_RETURN) == true)
		{// ENTERキーが押された
			// オブジェクトを配置する
			if (m_Type == TYPE_OBJECT)
			{
				m_pMap->SetObject(m_SetPos, m_SetRot, m_ModelScale, m_ModelRollSpeed, m_ModelColType, m_fModelColRange, m_nSelectModelIdx);
			}
			else if (m_Type == TYPE_OBJECTMOVE)
			{
				m_pMap->SetObjectMove(m_SetPos, m_SetRot, m_ModelScale, m_ModelRollSpeed, m_nSelectModelIdx, m_ModelMove, m_nModelMaxMoveCounter, m_ModelColType, m_fModelColRange);
			}
			else if (m_Type == TYPE_BILLBOARD)
			{
				m_pMap->SetBillboard(m_SetPos, m_BillCol, m_fBillWidth, m_fBillHeight, m_fBillRot, m_bBillLighting, m_bBillDrawAddtive,m_nBillTexIdx, m_fBillColRange);
			}
			else if (m_Type == TYPE_EFFECT)
			{
				m_pMap->SetEffect(m_SetPos, m_SetRot, m_nEffectType);
			}
			else if (m_Type == TYPE_ENEMY)
			{
				m_pMap->SetEnemy(m_SetPos, m_SetRot, m_nEnemyType, m_nEnemyLife, m_nEnemyScore);
			}
			else if (m_Type == TYPE_WALL)
			{
				m_pMap->SetWall(m_SetPos, m_SetRot, m_WallCol, m_fWallWidth, m_fWallHeight, m_nWallXBlock, m_nWallYBlock, m_nWallTexSplitU, m_nWallTexSplitV, m_nWallTexIdx);
			}
			else if (m_Type == TYPE_ITEM)
			{
				m_pMap->SetItem(m_SetPos, m_ItemCol, m_fItemWidth, m_fItemHeight, m_fItemRot, m_bItemLighting, m_bItemDrawAddtive, m_nItemTexIdx, m_fItemColRange, m_nItemType);
			}
		}
	}

	// オブジェクトを移動させる処理
	ObjectMove();
}

//=============================================================================
//    オブジェクトを動かす処理
//=============================================================================
void CEditer::ObjectMove(void)
{
	// カメラの取得
	CCamera *pCamera = CManager::GetCamera();

	// キーボードの取得
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// マウスの取得
	CInputMouse *pMouse = CManager::GetMouse();

	if (pKeyboard != NULL)
	{// キーボードが取得できた
		if (pKeyboard->GetPress(DIK_A) == true)
		{// 左方向の入力がされた
			if (pKeyboard->GetPress(DIK_W) == true)
			{// 同時に上方向の入力がされた
			    // 移動処理
				m_SetPos.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * 4.0f;
				m_SetPos.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * 4.0f;
			}
			else if (pKeyboard->GetPress(DIK_S) == true)
			{// 同時に下方向の入力がされた
			    // 移動処理
				m_SetPos.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * 4.0f;
				m_SetPos.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * 4.0f;
			}
			else
			{// 何も押されてない
			    // 移動処理
				m_SetPos.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * 4.0f;
				m_SetPos.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * 4.0f;
			}
		}
		else if (pKeyboard->GetPress(DIK_D) == true)
		{// 右方向の入力がされた
			if (pKeyboard->GetPress(DIK_W) == true)
			{// 同時に上方向の入力がされた
			     // 移動処理
				m_SetPos.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * 4.0f;
				m_SetPos.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * 4.0f;
			}
			else if (pKeyboard->GetPress(DIK_S) == true)
			{// 同時に下方向の入力がされた
			    // 移動処理
				m_SetPos.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * 4.0f;
				m_SetPos.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * 4.0f;
			}
			else
			{// 何も押されてない
			    // 移動処理
				m_SetPos.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * 4.0f;
				m_SetPos.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * 4.0f;
			}
		}
		else if (pKeyboard->GetPress(DIK_W) == true)
		{// 上方向の入力がされた
			if (pKeyboard->GetPress(DIK_LSHIFT) == true)
			{
				// 移動処理
				m_SetPos.y += 4.0f;
			}
			else
			{
				// 移動処理
				m_SetPos.x += sinf(pCamera->GetRot().y) * 4.0f;
				m_SetPos.z += cosf(pCamera->GetRot().y) * 4.0f;
			}
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// 下方向の入力がされた
			if (pKeyboard->GetPress(DIK_LSHIFT) == true)
			{
				// 移動処理
				m_SetPos.y -= 4.0f;
			}
			else
			{
				// 移動処理
				m_SetPos.x += sinf(pCamera->GetRot().y + D3DX_PI) * 4.0f;
				m_SetPos.z += cosf(pCamera->GetRot().y + D3DX_PI) * 4.0f;
			}
		}
		else if (pKeyboard->GetPress(DIK_X) == true)
		{// Xキーが押された
		    // 配置物と地面へのポインタ取得する
			CScene *pScene = NULL;               // シーンクラスへのポインタ
			CScene *pSceneNext = NULL;           // 次のシーンクラスへのポインタ
			CObject *pObject = NULL;             // 配置物クラスへのポインタ
			CMeshField *pField = NULL;           // 地面クラスへのポインタ
			for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
			{// 描画優先順位の数だけ繰り返し
				pScene = CScene::GetTop(nCntPriority);
				while (pScene != NULL)
				{// メモリが空になるまで繰り返し
					pSceneNext = pScene->GetNext();
					if (pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD)
					{// 地面クラスへのポインタだったら
						pField = (CMeshField*)pScene;
						if (pField != NULL)
						{// 地面が取得できた
							float fFieldHeight = pField->GetPolyHeight(m_SetPos);
							if (m_SetPos.y < fFieldHeight)
							{// 座標が地面より下になった
								m_SetPos.y = fFieldHeight;
							}
						}
					}
					pScene = pSceneNext;
				}
			}
		}
	}

	if (pMouse != NULL)
	{// マウスが取得できた
	    // 同時に押されたときの位置から今のマウスカーソルの位置から視点注視点を移動させる向きを計算
		if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_2) == true)
		{// マウスのホイールボタンが押された
			float fAngle = atan2f(((float)pMouse->GetPoint().x - (float)pMouse->GetPointOld().x), ((float)pMouse->GetPoint().y - (float)pMouse->GetPointOld().y));
			fAngle -= pCamera->GetRot().y;
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
			fMovement *= (SCREEN_WIDTH / SCREEN_HEIGHT);

			// モデル座標を変える
			m_SetPos.x += sinf(fAngle) * fMovement;
			m_SetPos.z -= cosf(fAngle) * fMovement;
		}
	}

	// 向きを補正する
	// X軸
	if (m_SetRot.x < -D3DX_PI)
	{// 円周率を超えた
		m_SetRot.x += D3DX_PI * 2.0f;
	}
	if (m_SetRot.x > D3DX_PI)
	{// 円周率を超えた
		m_SetRot.x -= D3DX_PI * 2.0f;
	}

	// Y軸
	if (m_SetRot.y < -D3DX_PI)
	{// 円周率を超えた
		m_SetRot.y += D3DX_PI * 2.0f;
	}
	if (m_SetRot.y > D3DX_PI)
	{// 円周率を超えた
		m_SetRot.y -= D3DX_PI * 2.0f;
	}

	// Z軸
	if (m_SetRot.z < -D3DX_PI)
	{// 円周率を超えた
		m_SetRot.z += D3DX_PI * 2.0f;
	}
	if (m_SetRot.z > D3DX_PI)
	{// 円周率を超えた
		m_SetRot.z -= D3DX_PI * 2.0f;
	}


	// 回転するスピードを補正する
	// X軸
	if (m_ModelRollSpeed.x < -D3DX_PI)
	{// 円周率を超えた
		m_ModelRollSpeed.x += D3DX_PI * 2.0f;
	}
	if (m_ModelRollSpeed.x > D3DX_PI)
	{// 円周率を超えた
		m_ModelRollSpeed.x -= D3DX_PI * 2.0f;
	}

	// Y軸
	if (m_ModelRollSpeed.y < -D3DX_PI)
	{// 円周率を超えた
		m_ModelRollSpeed.y += D3DX_PI * 2.0f;
	}
	if (m_ModelRollSpeed.y > D3DX_PI)
	{// 円周率を超えた
		m_ModelRollSpeed.y -= D3DX_PI * 2.0f;
	}

	// Z軸
	if (m_ModelRollSpeed.z < -D3DX_PI)
	{// 円周率を超えた
		m_ModelRollSpeed.z += D3DX_PI * 2.0f;
	}
	if (m_ModelRollSpeed.z > D3DX_PI)
	{// 円周率を超えた
		m_ModelRollSpeed.z -= D3DX_PI * 2.0f;
	}


	// 値の設定
	if (m_Type == TYPE_OBJECT)
	{
		if (m_pSetObject != NULL)
		{
			m_pSetObject->SetPos(m_SetPos);
			m_pSetObject->SetRollSpeed(m_ModelRollSpeed);
		}
	}
	else if (m_Type == TYPE_OBJECTMOVE)
	{
		if (m_pSetObjectMove != NULL)
		{
			m_pSetObjectMove->SetPos(m_SetPos);
			m_pSetObjectMove->SetRollSpeed(m_ModelRollSpeed);
			m_pSetObjectMove->SetMove(m_ModelMove);
			m_pSetObjectMove->SetMaxMoveCounter(m_nModelMaxMoveCounter);
		}
	}
	else if (m_Type == TYPE_BILLBOARD)
	{
		if (m_pSetBillboardObj != NULL)
		{
			m_pSetBillboardObj->SetPos(m_SetPos);
		}
	}
	else if (m_Type == TYPE_EFFECT)
	{
		if (m_pSetEmitter != NULL)
		{
			m_pSetEmitter->SetPos(m_SetPos);
			m_pSetEmitter->SetRot(m_SetRot);
		}
	}
	else if (m_Type == TYPE_ENEMY)
	{
		if (m_pSetEnemy != NULL)
		{
			m_pSetEnemy->SetPos(m_SetPos);
			m_pSetEnemy->SetRot(m_SetRot);
		}
	}
	else if (m_Type == TYPE_WALL)
	{
		if (m_pSetWall != NULL)
		{
			m_pSetWall->SetPos(m_SetPos);
			m_pSetWall->SetRot(m_SetRot);
		}
	}
	else if (m_Type == TYPE_ITEM)
	{
		if (m_pSetItem != NULL)
		{
			m_pSetItem->SetPos(m_SetPos);
		}
	}
}

//=============================================================================
//    モデルを変える処理
//=============================================================================
void CEditer::ModelChange(void)
{
	// 配置物クラスを開放する
	if (m_pSetObject != NULL)
	{
		m_pSetObject->Uninit();
		m_pSetObject = NULL;
	}

	// 動く配置物クラスを開放する
	if (m_pSetObjectMove != NULL)
	{
		m_pSetObjectMove->Uninit();
		m_pSetObjectMove = NULL;
	}

	if (m_Type == TYPE_OBJECT)
	{
		m_pSetObject = CSetObject::Create(m_SetPos, m_SetRot, m_ModelScale, m_ModelRollSpeed);
		m_pSetObject->BindModel(m_pModelManager->GetMesh(m_nSelectModelIdx), m_pModelManager->GetBuffMat(m_nSelectModelIdx), m_pModelManager->GetNumMat(m_nSelectModelIdx), m_pModelManager->GetVtxMax(m_nSelectModelIdx), m_pModelManager->GetVtxMin(m_nSelectModelIdx), m_pModelManager->GetTexture(m_nSelectModelIdx));
		m_pSetObject->SetTransParent(0.5f);
	}
	else if (m_Type == TYPE_OBJECTMOVE)
	{
		m_pSetObjectMove = CSetObjectMove::Create(m_SetPos, m_SetRot, m_ModelScale, m_ModelRollSpeed, m_ModelMove, m_nModelMaxMoveCounter);
		m_pSetObjectMove->BindModel(m_pModelManager->GetMesh(m_nSelectModelIdx), m_pModelManager->GetBuffMat(m_nSelectModelIdx), m_pModelManager->GetNumMat(m_nSelectModelIdx), m_pModelManager->GetVtxMax(m_nSelectModelIdx), m_pModelManager->GetVtxMin(m_nSelectModelIdx), m_pModelManager->GetTexture(m_nSelectModelIdx));
		m_pSetObjectMove->SetVtxMin(m_pModelManager->GetVtxMin(m_nSelectModelIdx));
		m_pSetObjectMove->SetVtxMax(m_pModelManager->GetVtxMax(m_nSelectModelIdx));
		m_pSetObjectMove->SetTransParent(0.5f);
	}
}

//=============================================================================
//    配置するエフェクトの種類を変更する処理
//=============================================================================
void CEditer::EmitterChange(void)
{
	// 既存のエミッタを削除する
	if (m_pSetEmitter != NULL)
	{
		m_pSetEmitter->Uninit();
		m_pSetEmitter = NULL;
	}

	// 新しく作り直す
	m_pSetEmitter = m_pEffectManager->SetMapEditEffect(m_SetPos, m_SetRot, m_nEffectType);
}

//=============================================================================
//    配置する敵の種類を変更する処理
//=============================================================================
void CEditer::EnemyChange(void)
{
	// 既存の敵クラスを削除する
	if (m_pSetEnemy != NULL)
	{
		m_pSetEnemy->Uninit();
		m_pSetEnemy = NULL;
	}

	// 新しい敵を生成する
	m_pSetEnemy = m_pEnemyAssmbly->GetEnemyManager(m_nEnemyType)->SetMapEditEnemy(m_SetPos, m_SetRot, m_nEnemyType, m_nEnemyLife, m_nEnemyScore);
}

//=============================================================================
//    配置するオブジェクトの種類を変更する処理
//=============================================================================
void CEditer::ObjectChange(TYPE typeOld)
{
	// 前回の種類のオブジェクトを削除する
	switch (typeOld)
	{// 前回の種類で処理わけ
	case TYPE_OBJECT:       // 配置用モデル
		if (m_pSetObject != NULL)
		{
			m_pSetObject->Uninit();
			m_pSetObject = NULL;
		}
		break;
	case TYPE_OBJECTMOVE:   // 動く配置用モデル
		if (m_pSetObjectMove != NULL)
		{
			m_pSetObjectMove->Uninit();
			m_pSetObjectMove = NULL;
		}
		break;
	case TYPE_BILLBOARD:    // 配置用ビルボード
		if (m_pSetBillboardObj != NULL)
		{
			m_pSetBillboardObj->Uninit();
			m_pSetBillboardObj = NULL;
		}
		break;
	case TYPE_EFFECT:       // 配置用エフェクト
		if (m_pSetEmitter != NULL)
		{
			m_pSetEmitter->Uninit();
			m_pSetEmitter = NULL;
		}
		break;
	case TYPE_ENEMY:        // 敵
		if (m_pSetEnemy != NULL)
		{
			m_pSetEnemy->Uninit();
			m_pSetEnemy = NULL;
		}
		break;
	case TYPE_WALL:        // 壁
		if (m_pSetWall != NULL)
		{
			m_pSetWall->Uninit();
			m_pSetWall = NULL;
		}
		break;
	case TYPE_ITEM:        // アイテム
		if (m_pSetItem != NULL)
		{
			m_pSetItem->Uninit();
			m_pSetItem = NULL;
		}
		break;
	}

	// 今回の種類のオブジェクトを作成する
	switch (m_Type)
	{// 前回の種類で処理わけ
	case TYPE_OBJECT:       // 配置用モデル
		m_pSetObject = CSetObject::Create(m_SetPos, m_SetRot, m_ModelScale, m_ModelRollSpeed);
		if (m_pSetObject != NULL)
		{
			m_pSetObject->BindModel(m_pModelManager->GetMesh(m_nSelectModelIdx), m_pModelManager->GetBuffMat(m_nSelectModelIdx), m_pModelManager->GetNumMat(m_nSelectModelIdx), m_pModelManager->GetVtxMax(m_nSelectModelIdx), m_pModelManager->GetVtxMin(m_nSelectModelIdx), m_pModelManager->GetTexture(m_nSelectModelIdx));
			m_pSetObject->SetTransParent(0.5f);
		}
		break;
	case TYPE_OBJECTMOVE:   // 動く配置用モデル
		m_pSetObjectMove = CSetObjectMove::Create(m_SetPos, m_SetRot, m_ModelScale, m_ModelRollSpeed, m_ModelMove, m_nModelMaxMoveCounter);
		if (m_pSetObjectMove != NULL)
		{
			m_pSetObjectMove->BindModel(m_pModelManager->GetMesh(m_nSelectModelIdx), m_pModelManager->GetBuffMat(m_nSelectModelIdx), m_pModelManager->GetNumMat(m_nSelectModelIdx), m_pModelManager->GetVtxMax(m_nSelectModelIdx), m_pModelManager->GetVtxMin(m_nSelectModelIdx), m_pModelManager->GetTexture(m_nSelectModelIdx));
			m_pSetObjectMove->SetTransParent(0.5f);
		}
		break;
	case TYPE_BILLBOARD:    // 配置用ビルボード
		m_pSetBillboardObj = CSetBillboardObject::Create(m_SetPos,m_BillCol,m_fBillWidth,m_fBillHeight,m_fBillRot,m_bBillLighting,m_bBillDrawAddtive,m_nBillTexIdx);
		if (m_pSetBillboardObj != NULL)
		{
			m_pSetBillboardObj->BindTexture(m_pTextureManager->GetTexture(m_nBillTexIdx));
		}
		break;
	case TYPE_EFFECT:        // 配置エフェクト
		m_pSetEmitter = m_pEffectManager->SetMapEditEffect(m_SetPos, m_SetRot, m_nEffectType);
		break;
	case TYPE_ENEMY:         // 敵
		m_pSetEnemy = m_pEnemyAssmbly->GetEnemyManager(m_nEnemyType)->SetMapEditEnemy(m_SetPos, m_SetRot, m_nEnemyType, m_nEnemyLife, m_nEnemyScore);
		break;
	case TYPE_WALL:          // 壁
		m_pSetWall = CSetMeshWall::Create(m_SetPos, m_SetRot,m_WallCol, m_fWallWidth, m_fWallHeight, m_nWallXBlock, m_nWallYBlock, m_nWallTexSplitU, m_nWallTexSplitV, m_nWallTexIdx);
		if (m_pSetWall != NULL)
		{
			m_pSetWall->BindTexture(m_pTextureManager->GetTexture(m_nWallTexIdx));
		}
		break;
	case TYPE_ITEM:          //	アイテム
		m_pSetItem = CSetItem::Create(m_SetPos, m_ItemCol, m_fItemWidth, m_fItemHeight, m_fItemRot, m_bItemLighting, m_bItemDrawAddtive, m_nItemTexIdx, m_fItemColRange, m_nItemType);
		if (m_pSetItem != NULL)
		{
			m_pSetItem->BindTexture(m_pTextureManager->GetTexture(m_nItemTexIdx));
		}
		break;
	}
}

//=============================================================================
//    配置オブジェクトの座標を設定する処理
//=============================================================================
void CEditer::SetPos(const D3DXVECTOR3 SetPos)
{
	m_SetPos = SetPos;
}

//=============================================================================
//    配置オブジェクトの向きを設定する処理
//=============================================================================
void CEditer::SetRot(const D3DXVECTOR3 SetRot)
{
	m_SetRot = SetRot;
}

//=============================================================================
//    配置用モデルの種類を設定する処理
//=============================================================================
void CEditer::SetType(const TYPE Type)
{
	m_Type = Type;
}

//=============================================================================
//    エフェクトの状態を設定する処理
//=============================================================================
void CEditer::SetEffectState(const EFFECTSTATE state)
{
	m_EffectState = state;
	if (m_EffectState == EFFECTSTATE_STOP)
	{
		m_pSetEmitter = NULL;
		m_pEffectManager->SetEmitter(NULL);
	}
}

//=============================================================================
//    配置用モデルの大きさの倍率を設定する処理
//=============================================================================
void CEditer::SetModelScale(const D3DXVECTOR3 ModelScale)
{
	m_ModelScale = ModelScale;
}

//=============================================================================
//    配置用モデルの回転スピードを設定する処理
//=============================================================================
void CEditer::SetModelRollSpeed(const D3DXVECTOR3 ModelRollSpeed)
{
	m_ModelRollSpeed = ModelRollSpeed;
}

//=============================================================================
//    配置用モデルの当たり判定の種類を設定する処理
//=============================================================================
void CEditer::SetModelColType(const CObject::COL_TYPE ModelColType)
{
	m_ModelColType = ModelColType;
}

//=============================================================================
//    配置用モデルの当たり判定を取る範囲を設定する処理
//=============================================================================
void CEditer::SetModelColRange(const float fModelColRange)
{
	m_fModelColRange = fModelColRange;
}

//=============================================================================
//    配置用モデルの回転スピードを取得する処理
//=============================================================================
void CEditer::SetModelMove(const D3DXVECTOR3 ModelMove)
{
	m_ModelMove = ModelMove;
}

//=============================================================================
//    配置用モデルの移動量を切り替えるタイミングを取得する処理
//=============================================================================
void CEditer::SetModelMaxMoveCounter(const int nModelMaxMoveCounter)
{
	m_nModelMaxMoveCounter = nModelMaxMoveCounter;
}

//=============================================================================
//    配置できるモデルの種類の数を設定する処理
//=============================================================================
void CEditer::SetNumModel(const int nNumModel)
{
	m_nNumModel = nNumModel;
}

//=============================================================================
//    現在選択されているモデルの番号を設定する処理
//=============================================================================
void CEditer::SetSelectModelIdx(const int nSelectModelIdx)
{
	m_nSelectModelIdx = nSelectModelIdx;
}

//=============================================================================
//    配置できるビルボードの色を設定する処理
//=============================================================================
void CEditer::SetBillCol(const D3DXCOLOR BillCol)
{
	m_BillCol = BillCol;
}

//=============================================================================
//    配置できるビルボードの当たり判定を取る範囲を設定する処理
//=============================================================================
void CEditer::SetBillColRange(const float fBillColRange)
{
	m_fBillColRange = fBillColRange;
}

//=============================================================================
//    配置できるビルボードの向きを設定する処理
//=============================================================================
void CEditer::SetBillRot(const float fBillRot)
{
	m_fBillRot = fBillRot;
}

//=============================================================================
//    配置できるビルボードの幅を取得する処理
//=============================================================================
void CEditer::SetBillWidth(const float fBillWidth)
{
	m_fBillWidth = fBillWidth;
}

//=============================================================================
//    配置できるビルボードの高さを設定する処理
//=============================================================================
void CEditer::SetBillHeight(const float fBillHeight)
{
	m_fBillHeight = fBillHeight;
}

//=============================================================================
//    配置できるビルボードの座標を設定する処理
//=============================================================================
void CEditer::SetBillTexIdx(const int nTexIdx)
{
	m_nBillTexIdx = nTexIdx;
}

//=============================================================================
//    配置できるビルボードの座標を設定する処理
//=============================================================================
void CEditer::SetBillLighting(const bool bLighting)
{
	m_bBillLighting = bLighting;
}

//=============================================================================
//    配置できるビルボードの座標を設定する処理
//=============================================================================
void CEditer::SetBillDrawAddtive(const bool bDrawAddtive)
{
	m_bBillDrawAddtive = bDrawAddtive;
}

//=============================================================================
//    配置できるエフェクトの座標を設定する処理
//=============================================================================
void CEditer::SetEffectType(const int nEffectType)
{
	m_nEffectType = nEffectType;
}

//=============================================================================
//    配置できる敵の種類を設定する処理
//=============================================================================
void CEditer::SetEnemyType(const int nEnemyType)
{
	m_nEnemyType = nEnemyType;
}

//=============================================================================
//    配置できる敵の体力を設定する処理
//=============================================================================
void CEditer::SetEnemyLife(const int nEnemyLife)
{
	m_nEnemyLife = nEnemyLife;
}

//=============================================================================
//    配置できる敵のスコアを設定する処理
//=============================================================================
void CEditer::SetEnemyScore(const int nEnemyScore)
{
	m_nEnemyScore = nEnemyScore;
}

//=============================================================================
//    配置できる壁のテクスチャ番号を設定する処理
//=============================================================================
void CEditer::SetWallTexIdx(const int nWallTexIdx)
{
	m_nWallTexIdx = nWallTexIdx;
}

//=============================================================================
//    配置できる壁の色を設定する処理
//=============================================================================
void CEditer::SetWallCol(const D3DXCOLOR WallCol)
{
	m_WallCol = WallCol;
}

//=============================================================================
//    配置できる壁の1ブロック分の幅を設定する処理
//=============================================================================
void CEditer::SetWallWidth(const float fWidth)
{
	m_fWallWidth = fWidth;
}

//=============================================================================
//    配置できる壁の1ブロック分の高さを設定する処理
//=============================================================================
void CEditer::SetWallHeight(const float fHeight)
{
	m_fWallHeight = fHeight;
}

//=============================================================================
//    配置できる壁の横の分割数を設定する処理
//=============================================================================
void CEditer::SetWallXBlock(const int nXBlock)
{
	m_nWallXBlock = nXBlock;
}

//=============================================================================
//    配置できる壁の縦の分割数を設定する処理
//=============================================================================
void CEditer::SetWallYBlock(const int nYBlock)
{
	m_nWallYBlock = nYBlock;
}

//=============================================================================
//    配置できる壁のテクスチャV座標の分割数を設定する処理
//=============================================================================
void CEditer::SetWallTexSplitU(const int nWallTexSplitU)
{
	m_nWallTexSplitU = nWallTexSplitU;
}

//=============================================================================
//    配置できる壁のテクスチャV座標の分割数を設定する処理
//=============================================================================
void CEditer::SetWallTexSplitV(const int nWallTexSplitV)
{
	m_nWallTexSplitV = nWallTexSplitV;
}

//=============================================================================
//    配置できるアイテムの色を設定する処理
//=============================================================================
void CEditer::SetItemCol(const D3DXCOLOR ItemCol)
{
	m_ItemCol = ItemCol;
}

//=============================================================================
//    配置できるアイテムの当たり判定を取る範囲を設定する処理
//=============================================================================
void CEditer::SetItemColRange(const float fItemColRange)
{
	m_fItemColRange = fItemColRange;
}

//=============================================================================
//    配置できるアイテムの向きを設定する処理
//=============================================================================
void CEditer::SetItemRot(const float fItemRot)
{
	m_fItemRot = fItemRot;
}

//=============================================================================
//    配置できるアイテムの幅を設定する処理
//=============================================================================
void CEditer::SetItemWidth(const float fItemWidth)
{
	m_fItemWidth = fItemWidth;
}

//=============================================================================
//    配置できるアイテムの高さを設定する処理
//=============================================================================
void CEditer::SetItemHeight(const float fItemHeight)
{
	m_fItemHeight = fItemHeight;
}

//=============================================================================
//    配置できるアイテムのテクスチャ番号を設定する処理
//=============================================================================
void CEditer::SetItemTexIdx(const int nTexIdx)
{
	m_nItemTexIdx = nTexIdx;
}

//=============================================================================
//    配置できるアイテムの種類番号を設定する処理
//=============================================================================
void CEditer::SetItemType(const int nType)
{
	m_nItemType = nType;
}

//=============================================================================
//    配置できるアイテムのライティングするかしないかを設定する処理
//=============================================================================
void CEditer::SetItemLighting(const bool bLighting)
{
	m_bItemLighting = bLighting;;
}

//=============================================================================
//    配置できるアイテムの加算合成で描画するかしないかを設定する処理
//=============================================================================
void CEditer::SetItemDrawAddtive(const bool bDrawAddtive)
{
	m_bItemDrawAddtive = bDrawAddtive;
}

//=============================================================================
//    配置オブジェクトの座標を取得する処理
//=============================================================================
D3DXVECTOR3 CEditer::GetPos(void)
{
	return m_SetPos;
}

//=============================================================================
//    配置オブジェクトの向きを取得する処理
//=============================================================================
D3DXVECTOR3 CEditer::GetRot(void)
{
	return m_SetRot;
}

//=============================================================================
//    配置用モデルの種類を取得する処理
//=============================================================================
CEditer::TYPE CEditer::GetType(void)
{
	return m_Type;
}

//=============================================================================
//    エフェクトの状態を取得する処理
//=============================================================================
CEditer::EFFECTSTATE CEditer::GetEffectState(void)
{
	return m_EffectState;
}

//=============================================================================
//    配置できるモデルへのポインタを取得する処理
//=============================================================================
CSetObject *CEditer::GetSetObject(void)
{
	return m_pSetObject;
}

//=============================================================================
//    配置できる動くモデルへのポインタを取得する処理
//=============================================================================
CSetObjectMove *CEditer::GetSetObjectMove(void)
{
	return m_pSetObjectMove;
}

//=============================================================================
//    配置できるビルボードへのポインタを取得する処理
//=============================================================================
CSetBillboardObject *CEditer::GetSetBillObj(void)
{
	return m_pSetBillboardObj;
}

//=============================================================================
//    配置できるエフェクトエミッタへのポインタを取得する処理
//=============================================================================
CEmitter *CEditer::GetSetEmitter(void)
{
	return m_pSetEmitter;
}

//=============================================================================
//    配置できる敵クラスへのポインタを取得する処理
//=============================================================================
CSetEnemy *CEditer::GetEnemy(void)
{
	return m_pSetEnemy;
}

//=============================================================================
//    配置用の壁クラスへのポインタを取得する処理
//=============================================================================
CSetMeshWall *CEditer::GetSetMeshWall(void)
{
	return m_pSetWall;
}

//=============================================================================
//    配置用のアイテムクラスへのポインタを取得する処理
//=============================================================================
CSetItem *CEditer::GetSetItem(void)
{
	return m_pSetItem;
}

//=============================================================================
//    テクスチャ管轄クラスへのポインタを取得する処理
//=============================================================================
CTextureManager *CEditer::GetTextureManager(void)
{
	return m_pTextureManager;
}

//=============================================================================
//    エフェクト管轄クラスへのポインタを取得する処理
//=============================================================================
CEffectManager *CEditer::GetEffectManager(void)
{
	return m_pEffectManager;
}

//=============================================================================
//    敵管轄まとめクラスへのポインタを取得する処理
//=============================================================================
CEnemyAssembly *CEditer::GetEnemyAssembly(void)
{
	return m_pEnemyAssmbly;
}

//=============================================================================
//    マップクラスへのポインタを取得する処理
//=============================================================================
CMap *CEditer::GetMap(void)
{
	return m_pMap;
}

//=============================================================================
//    配置用モデルの当たり判定の種類を取得する処理
//=============================================================================
CObject::COL_TYPE CEditer::GetModelColType(void)
{
	return m_ModelColType;
}

//=============================================================================
//    配置用モデルの当たり判定を取る範囲を取得する処理
//=============================================================================
float CEditer::GetModelColRange(void)
{
	return m_fModelColRange;
}

//=============================================================================
//    配置用モデルの大きさの倍率を取得する処理
//=============================================================================
D3DXVECTOR3 CEditer::GetModelScale(void)
{
	return m_ModelScale;
}

//=============================================================================
//    配置用モデルの回転スピードを取得する処理
//=============================================================================
D3DXVECTOR3 CEditer::GetModelRollSpeed(void)
{
	return m_ModelRollSpeed;
}

//=============================================================================
//    配置用モデルの移動量を取得する処理
//=============================================================================
D3DXVECTOR3 CEditer::GetModelMove(void)
{
	return m_ModelMove;
}

//=============================================================================
//    配置用モデルの移動量を切り替えるタイミングを取得する処理
//=============================================================================
int CEditer::GetModelMaxMoveCounter(void)
{
	return m_nModelMaxMoveCounter;
}

//=============================================================================
//    配置できるモデルの種類の数を取得する処理
//=============================================================================
int CEditer::GetNumModel(void)
{
	return m_nNumModel;
}

//=============================================================================
//    現在選択されているモデルの番号を取得する処理
//=============================================================================
int CEditer::GetSelectModelIdx(void)
{
	return m_nSelectModelIdx;
}

//=============================================================================
//    配置できるビルボードの色を取得する処理
//=============================================================================
D3DXCOLOR CEditer::GetBillCol(void)
{
	return m_BillCol;
}

//=============================================================================
//    配置できるビルボードの当たり判定を取る範囲を取得する処理
//=============================================================================
float CEditer::GetBillColRange(void)
{
	return m_fBillColRange;
}

//=============================================================================
//    配置できるビルボードの番号を取得する処理
//=============================================================================
float CEditer::GetBillRot(void)
{
	return m_fBillRot;
}

//=============================================================================
//    配置できるビルボードの番号を取得する処理
//=============================================================================
float CEditer::GetBillWidth(void)
{
	return m_fBillWidth;
}

//=============================================================================
//    配置できるビルボードの番号を取得する処理
//=============================================================================
float CEditer::GetBillHeight(void)
{
	return m_fBillHeight;
}

//=============================================================================
//    配置できるビルボードの番号を取得する処理
//=============================================================================
int CEditer::GetBillTexIdx(void)
{
	return m_nBillTexIdx;
}

//=============================================================================
//    配置できるビルボードの番号を取得する処理
//=============================================================================
bool CEditer::GetBillLighting(void)
{
	return m_bBillLighting;
}

//=============================================================================
//    配置できるビルボードの加算合成で描画するかしないかを取得する処理
//=============================================================================
bool CEditer::GetBillDrawAddtive(void)
{
	return m_bBillDrawAddtive;
}

//=============================================================================
//    設定できるテクスチャの種類の数を取得する処理
//=============================================================================
int CEditer::GetNumTex(void)
{
	return m_nNumTex;
}

//=============================================================================
//    配置できるエフェクトの種類番号を取得する処理
//=============================================================================
int CEditer::GetEffectType(void)
{
	return m_nEffectType;
}

//=============================================================================
//    配置できる敵の種類を取得する処理
//=============================================================================
int CEditer::GetEnemyType(void)
{
	return m_nEnemyType;
}

//=============================================================================
//    配置できる敵の体力を取得する処理
//=============================================================================
int CEditer::GetEnemyLife(void)
{
	return m_nEnemyLife;
}

//=============================================================================
//    配置できる敵のスコアを取得する処理
//=============================================================================
int CEditer::GetEnemyScore(void)
{
	return m_nEnemyScore;
}

//=============================================================================
//    配置できる壁のテクスチャ番号を取得する処理
//=============================================================================
int CEditer::GetWallTexIdx(void)
{
	return m_nWallTexIdx;
}

//=============================================================================
//    配置できる壁の色を取得する処理
//=============================================================================
D3DXCOLOR CEditer::GetWallCol(void)
{
	return m_WallCol;
}

//=============================================================================
//    配置できる壁の1ブロック分の幅を取得する処理
//=============================================================================
float CEditer::GetWallWidth(void)
{
	return m_fWallWidth;
}

//=============================================================================
//    配置できる壁の1ブロック分の高さを取得する処理
//=============================================================================
float CEditer::GetWallHeight(void)
{
	return m_fWallHeight;
}

//=============================================================================
//    配置できる壁の横の分割数を取得する処理
//=============================================================================
int CEditer::GetWallXBlock(void)
{
	return m_nWallXBlock;
}

//=============================================================================
//    配置できる壁の縦の分割数を取得する処理
//=============================================================================
int CEditer::GetWallYBlock(void)
{
	return m_nWallYBlock;
}

//=============================================================================
//    配置できる壁のテクスチャU座標の分割数を取得する処理
//=============================================================================
int CEditer::GetWallTexSplitU(void)
{
	return m_nWallTexSplitU;
}

//=============================================================================
//    配置できる壁のテクスチャV座標の分割数を取得する処理
//=============================================================================
int CEditer::GetWallTexSplitV(void)
{
	return m_nWallTexSplitV;
}

//=============================================================================
//    配置できるアイテムの色を取得する処理
//=============================================================================
D3DXCOLOR CEditer::GetItemCol(void)
{
	return m_ItemCol;
}

//=============================================================================
//    配置できるアイテムの当たり判定を取る範囲を取得する処理
//=============================================================================
float CEditer::GetItemColRange(void)
{
	return m_fItemColRange;
}

//=============================================================================
//    配置できるアイテムの向きを取得する処理
//=============================================================================
float CEditer::GetItemRot(void)
{
	return m_fItemRot;
}

//=============================================================================
//    配置できるアイテムの幅を取得する処理
//=============================================================================
float CEditer::GetItemWidth(void)
{
	return m_fItemWidth;
}

//=============================================================================
//    配置できるアイテムの高さを取得する処理
//=============================================================================
float CEditer::GetItemHeight(void)
{
	return m_fItemHeight;
}

//=============================================================================
//    配置できるアイテムのテクスチャ番号を取得する処理
//=============================================================================
int CEditer::GetItemTexIdx(void)
{
	return m_nItemTexIdx;
}

//=============================================================================
//    配置できるアイテムの種類番号を取得する処理
//=============================================================================
int CEditer::GetItemType(void)
{
	return m_nItemType;
}

//=============================================================================
//    配置できるアイテムのライティングするかしないかを取得する処理
//=============================================================================
bool CEditer::GetItemLighting(void)
{
	return m_bItemLighting;
}

//=============================================================================
//    配置できるアイテムの加算合成で描画するかしないかを取得する処理
//=============================================================================
bool CEditer::GetItemDrawAddtive(void)
{
	return m_bItemDrawAddtive;
}