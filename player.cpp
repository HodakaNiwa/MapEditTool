//*****************************************************************************
//
//     プレイヤーの処理[player.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "modelManager.h"
#include "motion.h"
#include "camera.h"
#include "input.h"
#include "effectManager.h"
#include "object.h"
#include "enemy.h"
#include "orbitEffect.h"
#include "meshField.h"
#include "textureManager.h"
#include "functionlib.h"
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
// 値読み込みをする際の目印となる文字列
// 共通
#define POS               "POS = "                // 座標を読み取る合図
#define ROT               "ROT = "                // 向きを読み取る合図
#define MOVE              "MOVE = "               // 移動量を読み取る合図

// その他
#define NUM_TEXTURE       "NUM_TEXTURE = "        // 読み込むテクスチャの数
#define TEXTURE_FILENAME  "TEXTURE_FILENAME = "   // 読み込むテクスチャのファイル名
#define NUM_MODEL         "NUM_MODEL = "          // 読み込むモデルの数
#define MODEL_FILENAME    "MODEL_FILENAME = "     // 読み込むモデルのファイル名

// キャラクター情報
#define CHARACTERSET      "CHARACTERSET"          // キャラクター情報読み込み開始の合図
#define END_CHARACTERSET  "END_CHARACTERSET"      // キャラクター情報読み込み終了の合図
#define JUMP              "JUMP = "               // ジャンプ量
#define GRAVITY           "GRAVITY = "            // 重力量
#define INERTIA           "INERTIA = "            // 慣性量
#define RIVISION_ANGLE    "RIVISION_ANGLE = "     // 向きを補正する倍率
#define COL_HEIGHT        "COL_HEIGHT = "         // 当たり判定をとる高さ
#define COL_RANGE         "COL_RANGE = "          // 当たり判定をとる範囲
#define NUM_PARTS         "NUM_PARTS = "          // キャラクターのパーツ数

// パーツ情報
#define PARTSSET          "PARTSSET"              // パーツ情報読み込み開始の合図
#define END_PARTSSET      "END_PARTSSET"          // パーツ情報読み込み終了の合図
#define INDEX             "INDEX = "              // 番号を読み取る合図
#define PARENT            "PARENT = "             // 親パーツの番号を読み取る合図

// モーション情報
#define MOTIONSET         "MOTIONSET"             // モーション情報読み込み開始の合図
#define END_MOTIONSET     "END_MOTIONSET"         // モーション情報読み込み終了の合図
#define COLLISIONSET      "COLLISIONSET"          // モーションの当たり判定情報読み込み開始の合図
#define END_COLLISIONSET  "END_COLLISIONSET"      // モーションの当たり判定情報読み込み終了の合図
#define ORBITSET          "ORBITSET"              // モーションの軌跡情報読み込み開始の合図
#define END_ORBITSET      "END_ORBITSET"          // モーションの軌跡情報読み込み終了の合図
#define RANGE             "RANGE = "              // 範囲を読み取る合図
#define XBLOCK            "XBLOCK = "             // 軌跡の横の分割数を読み取る合図
#define ZBLOCK            "ZBLOCK = "             // 軌跡の奥行の分割数を読み取る合図
#define OFFSET            "OFFSET = "             // オフセットを読み取る合図
#define LOOP              "LOOP = "               // モーションがループ再生するかどうかを読み取る合図
#define BLEND             "BLEND = "              // モーションブレンドの倍率を読み取る合図
#define NUM_KEY           "NUM_KEY = "            // モーションのキーフレームの数を読み取る合図

// 攻撃モーション情報
#define ATTACKSET         "ATTACKSET"             // 攻撃モーション情報読み込み開始の合図
#define END_ATTACKSET     "END_ATTACKSET"         // 攻撃モーション情報読み込み終了の合図
#define NEXT_MOTION       "NEXT_MOTION = "        // 次のモーションの番号

// 当たり判定情報
#define NUM_COL           "NUM_COL = "            // モーションの当たり判定情報の数を読み取る合図
#define COLLISION         "COLLISION"             // 当たり判定情報読み込み開始の合図
#define END_COLLISION     "END_COLLISION"         // 当たり判定情報読み込み終了の合図

// 軌跡情報
#define NUM_ORBIT         "NUM_ORBIT = "          // モーションの軌跡情報の数を読み取る合図
#define ORBIT		      "ORBIT"                 // 軌跡情報読み込み開始の合図
#define END_ORBIT	      "END_ORBIT"             // 軌跡情報読み込み終了の合図
#define TEX_INDEX         "TEX_INDEX = "          // 軌跡に張り付けるテクスチャの番号を読み取る合図
#define OFFSET1           "OFFSET1 = "            // １個目のオフセットを読み取る合図
#define OFFSET2           "OFFSET2 = "            // ２個目のオフセットを読み取る合図
#define OFFSETAMP1        "OFFSETAMP1 = "         // 軌跡のオフセット距離増幅値を読み取る合図(１個目)
#define OFFSETAMP2        "OFFSETAMP2 = "         // 軌跡のオフセット距離増幅値を読み取る合図(２個目)
#define COLUP             "COLUP = "              // 軌跡の頂点上側の色
#define COLDOWN           "COLDOWN = "            // 軌跡の頂点下側の色
#define ALPHA_DECAY_UP    "ALPHA_DECAY_UP = "     // 軌跡の頂点上側の透明度減衰値
#define ALPHA_DECAY_DOWN  "ALPHA_DECAY_DOWN = "   // 軌跡の頂点下側の透明度減衰値
#define START             "START = "              // 軌跡を出し始めるタイミングを読み取る合図
#define FINISH            "FINISH = "             // 軌跡をしまうタイミングを読み取る合図

// キーフレーム情報
#define KEYSET            "KEYSET"                // キーフレーム情報読み込み開始の合図
#define END_KEYSET        "END_KEYSET"            // キーフレーム情報読み込み終了の合図
#define FRAME             "FRAME = "              // キーフレームの再生フレーム数を読み取る合図

// キー情報
#define KEY               "KEY"                   // キー情報読み込み開始の合図
#define END_KEY           "END_KEY"               // キー情報読み込み終了の合図

//*****************************************************************************
//     CPlayerManagerの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CPlayerManager::CPlayerManager(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_pTextureManager = NULL;     // テクスチャ管轄クラスへのポインタ
	strcpy(m_aFileName, "\0");    // プレイヤーデータのスクリプトファイル名
	m_pModelManager = NULL;       // モデル管轄クラスへのポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CPlayerManager::~CPlayerManager()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CPlayerManager *CPlayerManager::Create(char *pFileName, int nPriority)
{
	CPlayerManager *pPlayerManager = NULL;  // プレイヤー管轄クラス型のポインタ
	int nIdxScene = CScene::GetNumAll();    // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pPlayerManager == NULL)
		{// メモリが空になっている
			pPlayerManager = new CPlayerManager(nPriority);
			if (pPlayerManager != NULL)
			{// インスタンスを生成できた
				pPlayerManager->SetFileName(pFileName);
				if (FAILED(pPlayerManager->Init()))
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

	return pPlayerManager;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CPlayerManager::Init(void)
{
	// プレイヤーのパーツ情報を抜き出す
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態である
		pFile = fopen(m_aFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // スクリプト読み込み用の変数を用意
			char *pLine = NULL;                         // 1行分読み取り用
			char *pStrCur = NULL;                       // 現在の先頭ポインタ
			char *pStr = NULL;                          // 先頭ポインタ保存用
			int nNumTexture = 0;                        // 読み込むテクスチャの数
			int nNumModel = 0;                          // 読み込むモデル数
			int nNumParts = 0;                          // 読み込むパーツ数
			int nCntTex = 0;                            // テクスチャを読み込んだ回数を数える
			int nCntModel = 0;                          // モデルを読み込んだ回数を数える
			int nCntParts = 0;                          // パーツ情報を読み込んだ回数を数える
			int nCntMotion = 0;                         // モーションを読み込んだ回数を数える
			int *pNumParent = NULL;                     // 親モデルの番号
			CMotionManager *pMotionManager = NULL;      // モーション管理クラスへのポインタ
			CMotion *pMotion[CPlayer::STATE_MAX] = {};  // モーションクラスへのポインタ
			CModel **apModel = NULL;                    // モデルクラスへのポインタ
			LPDIRECT3DTEXTURE9 pTexture = NULL;         // テクスチャ情報へのポインタ
			LPD3DXMESH pMesh = NULL;                    // メッシュ情報へのポインタ
			LPD3DXBUFFER pBuffMat = NULL;               // マテリアル情報へのポインタ
			DWORD nNumMat = 0;                          // マテリアル情報の数
			float fAccel = 0.0f;                        // プレイヤーの加速度情報
			float fInertia = 0.0f;                      // プレイヤーの慣性情報
			float fGravity = 0.0f;                      // プレイヤーの重力情報
			float fJumpPower = 0.0f;                    // プレイヤーのジャンプ量情報
			float fRivisionRot = 0.0f;                  // プレイヤーの向きを補正する倍率情報
			float fColHeight = 0.0f;                    // プレイヤーの当たり判定を取る高さ情報
			float fColRange = 0.0f;                     // プレイヤーの当たり判定を取る範囲情報

			if (pLine == NULL && pStr == NULL)
			{// メモリが確保できる状態である
				pLine = new char[256];
				pStr = new char[256];
				strcpy(pLine, "\0");
				strcpy(pStr, "\0");
				if (pLine != NULL && pStr != NULL)
				{// メモリが確保されている
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
					if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
					{// スクリプト読み込み開始の合図だった
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, NUM_TEXTURE) == 0)
							{// 読み込むテクスチャ数だった
								strcpy(pStr, pStrCur);
								nNumTexture = CFunctionLib::ReadInt(pStrCur, NUM_TEXTURE);
								if (nNumTexture >= 1)
								{// 読み込むテクスチャ数が1つ以上ある
									m_pTextureManager = CTextureManager::Create(nNumTexture);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, TEXTURE_FILENAME) == 0)
							{// テクスチャのファイルパス名を読み込んだ
								if (m_pTextureManager != NULL && nCntTex < nNumTexture)
								{// テクスチャ管轄クラスへのポインタが確保されている
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
							}
							else if (CFunctionLib::Memcmp(pStrCur, NUM_MODEL) == 0)
							{// 読み込むモデル数だった
								strcpy(pStr, pStrCur);
								nNumModel = CFunctionLib::ReadInt(pStrCur, NUM_MODEL);
								if (nNumModel >= 1)
								{// モデル数が1つ以上ある
									pNumParent = new int[nNumModel];
									m_pModelManager = CModelManager::Create(nNumModel);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, MODEL_FILENAME) == 0)
							{// モデルのファイルパス名を読み込んだ
								if (m_pModelManager != NULL && nCntModel < nNumModel)
								{// モデル管轄クラスへのポインタが確保されている
									// モデルのファイルパス名を読み取る
									pStr = CFunctionLib::ReadString(pStrCur, pStr, MODEL_FILENAME);

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
							}
							else if (CFunctionLib::Memcmp(pStrCur, CHARACTERSET) == 0)
							{// キャラクター情報読み込み開始の合図があった
								apModel = ReadCharacterSet(pLine, pStrCur, pFile, &nNumParts, pNumParent, &fAccel, &fInertia, &fJumpPower, &fRivisionRot, &fGravity, &fColHeight, &fColRange);
								nCntParts++;
							}
							else if (CFunctionLib::Memcmp(pStrCur, MOTIONSET) == 0)
							{// モーション情報読み込み開始の合図があった
								pMotion[nCntMotion] = ReadMotionSet(pLine, pStrCur, pFile, &nNumParts, pMotion[nCntMotion], nCntMotion);
								nCntMotion++;
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// スクリプト読み込み終了の合図だった
								break;  // ループ終了
							}
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

			// 必要なクラスを作成しておく
			// モーション管轄クラス
			if (pMotionManager == NULL)
			{// ポインタが確保されている
				pMotionManager = CMotionManager::Create(CPlayer::STATE_MAX, nNumParts);
				if (pMotionManager != NULL)
				{// モーション管理クラスが作成できた
					for (int nCntMotion = 0; nCntMotion < CPlayer::STATE_MAX; nCntMotion++)
					{// 状態の数だけ繰りかえし
						pMotionManager->SetMotion(pMotion[nCntMotion], nCntMotion);
					}
				}
			}

			// プレイヤークラスを作成する
			CreatePlayer(apModel, pMotionManager, nNumParts, pNumParent, fAccel, fInertia, fJumpPower, fRivisionRot, fGravity, fColHeight, fColRange);

			// 読み取り用のポインタを開放しておく
			if (apModel != NULL)
			{// メモリが確保されている
				for (int nCntModel = 0; nCntModel < nNumParts; nCntModel++)
				{// 使用できるモデル数分繰り返し
					if (apModel[nCntModel] != NULL)
					{// ポインタが確保されている
						apModel[nCntModel]->Uninit();

						// メモリの開放
						delete apModel[nCntModel];
						apModel[nCntModel] = NULL;
					}
				}
				// メモリの開放
				delete[] apModel;
				apModel = NULL;
			}

			// モーションの破棄
			if (pMotionManager != NULL)
			{// ポインタが確保されている
				pMotionManager->Uninit();

				// メモリの開放
				delete pMotionManager;
				pMotionManager = NULL;
			}

			// 親モデルの番号
			if (pNumParent != NULL)
			{// メモリが確保されている
				delete[] pNumParent;
				pNumParent = NULL;
			}
			fclose(pFile);
		}
		else
		{// ファイルポインタを確保できなかった
			return E_FAIL;
		}
	}
	else
	{// ファイルポインタを確保できる状態でない
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CPlayerManager::Uninit(void)
{
	// モデル管轄クラスの破棄
	if (m_pModelManager != NULL)
	{// メモリが確保されている
		// 終了処理
		m_pModelManager->Uninit();

		// メモリの開放
		delete m_pModelManager;
		m_pModelManager = NULL;
	}

	// 自身のポインタを破棄する
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CPlayerManager::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CPlayerManager::Draw(void)
{

}

//=============================================================================
//    プレイヤークラスを作製する処理
//=============================================================================
void CPlayerManager::CreatePlayer(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fAccel, const float fInertia, const float fJumpPower, const float fRivisionRot, const float fGravity, const float fColHeight, const float fColRange)
{
	CModel **apModelCpy = NULL;                     // モデルクラスコピー用
	CMotionAttack *pMotionAttack = NULL;            // 攻撃モーションデータクラスコピー用
	CMotionCollision **pMotionCollision = NULL;     // モーションの当たり判定データクラスコピー用
	CMotionOrbit **pMotionOrbit = NULL;             // モーションの軌跡データクラスコピー用
	CMotion *pMotionCpy[CPlayer::STATE_MAX] = {};   // モージョンクラスコピー用
	CMotionManager *pMotionManagerCpy = NULL;       // モーション管轄クラスコピー用
	CPlayer *pPlayer = NULL;                        // プレイヤークラスへのポインタ

	if (apModel != NULL && pMotionManager != NULL && m_pModelManager != NULL)
	{// モデルへのポインタとモーションクラスへのポインタとモデル管轄クラスへのポインタが作成されている
	    // モデル情報をコピーする
		if (apModelCpy == NULL)
		{// メモリが確保されている
			apModelCpy = new CModel*[nNumParts];
			if (apModelCpy != NULL)
			{// メモリが確保できた
				for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
				{// パーツ数分繰り返し
					apModelCpy[nCntParts] = NULL;
					if (apModelCpy[nCntParts] == NULL)
					{// メモリが確保できる状態である
						apModelCpy[nCntParts] = CModel::Create(apModel[nCntParts]->GetPos(), apModel[nCntParts]->GetRot(), m_pModelManager->GetMesh(nCntParts), m_pModelManager->GetBuffMat(nCntParts), m_pModelManager->GetNumMat(nCntParts), m_pModelManager->GetVtxMax(nCntParts), m_pModelManager->GetVtxMin(nCntParts), m_pModelManager->GetTexture(nCntParts));
						if (apModelCpy[nCntParts] != NULL)
						{// メモリが確保できた
							if (pNumParent[nCntParts] != -1)
							{// 親モデルが存在する
								apModelCpy[nCntParts]->SetParent(apModelCpy[pNumParent[nCntParts]]);
							}
						}
					}
				}
			}
		}

		// モーション情報をコピーする
		if (pMotionManagerCpy == NULL)
		{// メモリが確保できる状態である
			pMotionManagerCpy = CMotionManager::Create(CPlayer::STATE_MAX, nNumParts);
			if (pMotionManagerCpy != NULL)
			{// モーション管理クラスが作成できた
				for (int nCntMotion = 0; nCntMotion < CPlayer::STATE_MAX; nCntMotion++)
				{// プレイヤーの状態の数だけ繰り返し
					if (pMotionCpy[nCntMotion] == NULL)
					{// メモリを確保できる状態である
						pMotionCpy[nCntMotion] = CMotion::Create(nNumParts, pMotionManager->GetMotion()[nCntMotion]->GetNumKey(), pMotionManager->GetMotion()[nCntMotion]->GetBlendMag(), pMotionManager->GetMotion()[nCntMotion]->GetLoop(), pMotionManager->GetMotion()[nCntMotion]->GetPlayBackKey());
						if (pMotionCpy[nCntMotion] != NULL)
						{// メモリが確保できた
							for (int nCntKey = 0; nCntKey < pMotionManager->GetMotion()[nCntMotion]->GetNumKey(); nCntKey++)
							{// キーフレームの総数分繰り返し
								for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
								{// キーフレームの総数分繰り返し
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetAddPos());
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetDestRot());
								}
							}

							// 攻撃モーションデータクラスのコピー
							if (pMotionManager->GetMotion()[nCntMotion]->GetAttack() != NULL)
							{// 攻撃モーションデータクラスが生成されている
								pMotionAttack = new CMotionAttack;
								if (pMotionAttack != NULL)
								{// メモリが確保できた
									// 各種値の設定
									pMotionAttack->SetNextMotionIdx(pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetNextMotionIdx());
									pMotionAttack->SetStartTiming(pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetStartTiming());
									pMotionAttack->SetFinishTiming(pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetFinishTiming());
									pMotionCpy[nCntMotion]->SetAttack(pMotionAttack);
								}
							}

							// 当たり判定データクラスのコピー
							if (pMotionManager->GetMotion()[nCntMotion]->GetNumColData() >= 1)
							{// 当たり判定データが１個以上ある
								pMotionCollision = new CMotionCollision*[pMotionManager->GetMotion()[nCntMotion]->GetNumColData()];
								if (pMotionCollision != NULL)
								{// メモリが確保できた
									for (int nCntCol = 0; nCntCol < pMotionManager->GetMotion()[nCntMotion]->GetNumColData(); nCntCol++)
									{// 当たり判定データの数だけ繰り返し
										pMotionCollision[nCntCol] = new CMotionCollision;
										if (pMotionCollision[nCntCol] != NULL)
										{// メモリが確保できた
											pMotionCollision[nCntCol]->SetModelIdx(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetModelIdx());
											pMotionCollision[nCntCol]->SetOffsetPos(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetOffsetPos());
											pMotionCollision[nCntCol]->SetRange(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetRange());
											pMotionCollision[nCntCol]->SetStartTiming(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetStartTiming());
											pMotionCollision[nCntCol]->SetFinishTiming(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetFinishTiming());
										}
									}
								}
								pMotionCpy[nCntMotion]->SetCollision(pMotionCollision);
								pMotionCpy[nCntMotion]->SetNumColData(pMotionManager->GetMotion()[nCntMotion]->GetNumColData());
							}

							// 軌跡データクラスのコピー
							if (pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData() >= 1)
							{// 当たり判定データが１個以上ある
								pMotionOrbit = new CMotionOrbit*[pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData()];
								if (pMotionOrbit != NULL)
								{// メモリが確保できた
									for (int nCntOrbit = 0; nCntOrbit < pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData(); nCntOrbit++)
									{// 当たり判定データの数だけ繰り返し
										pMotionOrbit[nCntOrbit] = new CMotionOrbit;
										if (pMotionOrbit[nCntOrbit] != NULL)
										{// メモリが確保できた
											pMotionOrbit[nCntOrbit]->SetModelIdx(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetModelIdx());
											pMotionOrbit[nCntOrbit]->SetTexIdx(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetTexIdx());
											pMotionOrbit[nCntOrbit]->SetOffsetPos1(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetPos1());
											pMotionOrbit[nCntOrbit]->SetOffsetPos2(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetPos2());
											pMotionOrbit[nCntOrbit]->SetOffsetAmp1(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetAmp1());
											pMotionOrbit[nCntOrbit]->SetOffsetAmp2(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetAmp2());
											pMotionOrbit[nCntOrbit]->SetColUp(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetColUp());
											pMotionOrbit[nCntOrbit]->SetColDown(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetColDown());
											pMotionOrbit[nCntOrbit]->SetAlphaDecayUp(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetAlphaDecayUp());
											pMotionOrbit[nCntOrbit]->SetAlphaDecayDown(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetAlphaDecayDown());
											pMotionOrbit[nCntOrbit]->SetXBlock(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetXBlock());
											pMotionOrbit[nCntOrbit]->SetYBlock(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetYBlock());
											pMotionOrbit[nCntOrbit]->SetStartTiming(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetStartTiming());
											pMotionOrbit[nCntOrbit]->SetFinishTiming(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetFinishTiming());
										}
									}
								}
								pMotionCpy[nCntMotion]->SetOrbit(pMotionOrbit);
								pMotionCpy[nCntMotion]->SetNumOrbitData(pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData());
							}

							// モーション管轄クラスにポインタを渡す
							pMotionManagerCpy->SetMotion(pMotionCpy[nCntMotion], nCntMotion);
						}
					}
				}
			}
		}

		// プレイヤークラスを作成する
		if (pPlayer == NULL)
		{// メモリが確保できる状態である
			pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), apModelCpy, pMotionManagerCpy, m_pTextureManager, nNumParts, fAccel, fInertia, fJumpPower, fGravity, fRivisionRot, fColHeight, fColRange);
		}
	}
}

//=============================================================================
//    プレイヤー情報を読み取る処理
//=============================================================================
CModel **CPlayerManager::ReadCharacterSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, float *pAccel, float *pInertia, float *pJumpPower, float *pRivisionRot, float *pGravity, float *pColHeight, float *pColRange)
{
	int nCntParts = 0;
	int nNumParts = 0;
	CModel **apModel = NULL;
	while (1)
	{// ループ開始
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
		if (CFunctionLib::Memcmp(pStrCur, MOVE) == 0)
		{// 移動量がある
			*pAccel = CFunctionLib::ReadFloat(pStrCur, MOVE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, JUMP) == 0)
		{// ジャンプ量がある
			*pJumpPower = CFunctionLib::ReadFloat(pStrCur, JUMP);
		}
		else if (CFunctionLib::Memcmp(pStrCur, GRAVITY) == 0)
		{// 重力量がある
			*pGravity = CFunctionLib::ReadFloat(pStrCur, GRAVITY);
		}
		else if (CFunctionLib::Memcmp(pStrCur, INERTIA) == 0)
		{// 慣性量がある
			*pInertia = CFunctionLib::ReadFloat(pStrCur, INERTIA);
		}
		else if (CFunctionLib::Memcmp(pStrCur, RIVISION_ANGLE) == 0)
		{// 向きを補正する倍率量がある
			*pRivisionRot = CFunctionLib::ReadFloat(pStrCur, RIVISION_ANGLE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL_HEIGHT) == 0)
		{//	当たり判定を取る高さがある
			*pColHeight = CFunctionLib::ReadFloat(pStrCur, COL_HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL_RANGE) == 0)
		{// 当たり判定を取る範囲がある
			*pColRange = CFunctionLib::ReadFloat(pStrCur, COL_RANGE);
		}
		if (CFunctionLib::Memcmp(pStrCur, NUM_PARTS) == 0)
		{// パーツ数情報がある
			*pNumParts = (CFunctionLib::ReadInt(pStrCur, NUM_PARTS));
			if (apModel == NULL)
			{// メモリが確保できる状態である
				apModel = new CModel*[*pNumParts];
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, PARTSSET) == 0)
		{// パーツ情報読み込み開始の合図だった
			if (nCntParts < *pNumParts)
			{// まだパーツ情報を読み込める
				apModel[nCntParts] = ReadPartsSet(pLine, pStrCur, pFile, pNumParts, pNumParent, apModel);
				nCntParts++;
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_CHARACTERSET) == 0)
		{// キャラクター情報読み込み終了の合図だった
			break;  // ループ終了
		}
	}

	return apModel;
}

//=============================================================================
//    パーツ情報を読み取る処理
//=============================================================================
CModel *CPlayerManager::ReadPartsSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, CModel **apModel)
{
	int nIdx = 0;                                      // パーツ番号情報
	int nParent = 0;                                   // 親モデルのパーツ番号
	CModel *pModel = NULL;                             // モデルクラスへのポインタ
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // モデルの座標情報
	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // モデルの向き情報
	while (1)
	{// ループ開始
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
		if (CFunctionLib::Memcmp(pStrCur, INDEX) == 0)
		{// パーツ番号情報がある
			nIdx = CFunctionLib::ReadInt(pStrCur, INDEX);
		}
		else if (CFunctionLib::Memcmp(pStrCur, PARENT) == 0)
		{// 親モデルのパーツ番号情報がある
			pNumParent[nIdx] = CFunctionLib::ReadInt(pStrCur, PARENT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// モデルの座標情報がある
			Pos = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
		{// モデルの向き情報がある
			Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_PARTSSET) == 0)
		{// パーツ情報読み込み終了の合図だった
			break;  // ループ終了
		}
	}

	if (pModel == NULL)
	{// メモリを確保できる状態である
		pModel = CModel::Create(Pos, Rot, m_pModelManager->GetMesh(nIdx), m_pModelManager->GetBuffMat(nIdx), m_pModelManager->GetNumMat(nIdx), m_pModelManager->GetVtxMax(nIdx), m_pModelManager->GetVtxMin(nIdx), m_pModelManager->GetTexture(nIdx));
		if (pModel != NULL)
		{// モデルポインタが確保された
			if (nParent != -1)
			{// 親モデルが存在する
				pModel->SetParent(apModel[nParent]);
			}
		}
	}

	return pModel;
}

//=============================================================================
//    モーション情報を読み取る処理
//=============================================================================
CMotion *CPlayerManager::ReadMotionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, CMotion *pMotion, int nCntMotion)
{
	int nCntKeySet = 0;                           // キーフレームフレーム情報を読み込んだ回数
	CMotionAttack *pMotionAttack = NULL;          // 攻撃モーションデータクラスへのポインタ
	CMotionCollision **pMotionCollision = NULL;   // モーションの当たり判定データクラスへのポインタ
	int nNumColData = 0;                          // 当たり判定データの数
	CMotionOrbit **pMotionOrbit = NULL;           // モーションの軌跡データクラスへのポインタ
	int nNumOrbitData = 0;                        // 軌跡データの数
	bool bLoop = false;                           // ループするかしないか
	float fBlendMag = 0.0f;                       // ブレンド倍率
	int nNumKey = 0;                              // キーフレームの総数
	int *pPlaybackKey = NULL;                     // 再生フレーム数(複数あるので動的確保)
	D3DXVECTOR3 **pAddPos = NULL;                 // モデルの座標
	D3DXVECTOR3 **pDestRot = NULL;                // モデルの向き
	while (1)
	{// ループ開始
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
		if (CFunctionLib::Memcmp(pStrCur, LOOP) == 0)
		{// ループするかしないかの情報がある
			bLoop = CFunctionLib::ReadBool(pStrCur, LOOP);
		}
		else if (CFunctionLib::Memcmp(pStrCur, BLEND) == 0)
		{// ブレンド倍率の情報がある
			fBlendMag = CFunctionLib::ReadFloat(pStrCur, BLEND);
		}
		else if (CFunctionLib::Memcmp(pStrCur, NUM_KEY) == 0)
		{// キーフレームの総数の情報がある
			nNumKey = (CFunctionLib::ReadInt(pStrCur, NUM_KEY));
			if (nNumKey >= 1)
			{// 1個以上キーフレームがある
			    // 必要な変数を動的に確保する
			    // 再生フレーム数
				pPlaybackKey = new int[nNumKey];

				// 座標
				if (pAddPos == NULL)
				{// メモリが確保できる状態である
					pAddPos = new D3DXVECTOR3*[nNumKey];
					if (pAddPos != NULL)
					{// メモリが確保できた
						for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
						{// キーフレームの総数分繰り返し
							pAddPos[nCntKey] = new D3DXVECTOR3[*pNumParts];
						}
					}
				}

				if (pDestRot == NULL)
				{// メモリが確保できる状態である
				 // 向き
					pDestRot = new D3DXVECTOR3*[nNumKey];
					if (pDestRot != NULL)
					{// メモリが確保できた
						for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
						{// キーフレームの総数分繰り返し
							pDestRot[nCntKey] = new D3DXVECTOR3[*pNumParts];
						}
					}
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, ATTACKSET) == 0)
		{// 当たり判定情報読み込み開始の合図だった
			pMotionAttack = ReadMotionAttackSet(pLine, pStrCur, pFile);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COLLISIONSET) == 0)
		{// 当たり判定情報読み込み開始の合図だった
			pMotionCollision = ReadMotionCollisionSet(pLine, pStrCur, pFile, &nNumColData);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ORBITSET) == 0)
		{// 軌跡情報読み込み開始の合図だった
			pMotionOrbit = ReadMotionOrbitSet(pLine, pStrCur, pFile, &nNumOrbitData);
		}
		else if (CFunctionLib::Memcmp(pStrCur, KEYSET) == 0)
		{// キーフレーム情報読み込み開始の合図だった
			ReadKeySet(pLine, pStrCur, pFile, nCntKeySet, pPlaybackKey, pAddPos[nCntKeySet], pDestRot[nCntKeySet]);
			nCntKeySet++;
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_MOTIONSET) == 0)
		{// モーション情報読み込み終了の合図だった
			break;  // ループ終了
		}
	}

	if (pMotion == NULL)
	{// メモリが確保できる状態である
		pMotion = CMotion::Create(*pNumParts, nNumKey, fBlendMag, bLoop, pPlaybackKey);
		if (pMotion != NULL)
		{// メモリが確保できた
			// 攻撃モーションデータクラスを設定する
			pMotion->SetAttack(pMotionAttack);

		    // 当たり判定データクラスを設定する
			pMotion->SetCollision(pMotionCollision);
			pMotion->SetNumColData(nNumColData);

			// 軌跡データクラスを設定する
			pMotion->SetOrbit(pMotionOrbit);
			pMotion->SetNumOrbitData(nNumOrbitData);

			for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
			{// キーフレームの総数分繰り返し
				for (int nCntParts = 0; nCntParts < *pNumParts; nCntParts++)
				{// キーフレームの総数分繰り返し
					pMotion->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(pAddPos[nCntKey][nCntParts]);
					pMotion->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(pDestRot[nCntKey][nCntParts]);
				}
			}
		}
	}

	// メモリを確保した分開放する
	// 再生フレーム数
	if (pPlaybackKey != NULL)
	{// メモリが確保されている
		delete[] pPlaybackKey;
		pPlaybackKey = NULL;
	}

	// 座標
	if (pAddPos != NULL)
	{// メモリが確保できた
		for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
		{// キーフレームの総数分繰り返し
			delete[] pAddPos[nCntKey];
			pAddPos[nCntKey] = NULL;
		}
		delete[] pAddPos;
		pAddPos = NULL;
	}

	// 向き
	if (pDestRot != NULL)
	{// メモリが確保できた
		for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
		{// キーフレームの総数分繰り返し
			delete[] pDestRot[nCntKey];
			pDestRot[nCntKey] = NULL;
		}
		delete[] pDestRot;
		pDestRot = NULL;
	}

	return pMotion;
}

//=============================================================================
//    モーションの攻撃モーション情報を読み取る処理
//=============================================================================
CMotionAttack *CPlayerManager::ReadMotionAttackSet(char *pLine, char *pStrCur, FILE *pFile)
{
	CMotionAttack *pMotionAttack = NULL;   // 攻撃モーションデータクラスへのポインタ
	pMotionAttack = new CMotionAttack;     // メモリを確保する
	if (pMotionAttack != NULL)
	{// メモリを確保できた
		while (1)
		{// ループ開始
			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
			if (CFunctionLib::Memcmp(pStrCur, NEXT_MOTION) == 0)
			{// 次のモーションの番号情報だった
				pMotionAttack->SetNextMotionIdx(CFunctionLib::ReadInt(pStrCur, NEXT_MOTION));
			}
			else if (CFunctionLib::Memcmp(pStrCur, START) == 0)
			{// 軌跡を出し始めるタイミング情報だった
				pMotionAttack->SetStartTiming(CFunctionLib::ReadInt(pStrCur, START));
			}
			else if (CFunctionLib::Memcmp(pStrCur, FINISH) == 0)
			{// 軌跡をしまうタイミング情報だった
				pMotionAttack->SetFinishTiming(CFunctionLib::ReadInt(pStrCur, FINISH));
			}
			else if (CFunctionLib::Memcmp(pStrCur, END_ATTACKSET) == 0)
			{// 攻撃モーションデータ読み込み終了の合図だった
				break;  // ループ終了
			}
		}
	}

	return pMotionAttack;
}

//=============================================================================
//    モーションの当たり判定情報を読み取る処理
//=============================================================================
CMotionCollision **CPlayerManager::ReadMotionCollisionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumColData)
{
	int nCntCollision = 0;                       // 当たり判定データを読み込んだ回数
	CMotionCollision **pMotionCollision = NULL;  // モーションの当たり判定データクラスへのポインタ

	while (1)
	{// ループ開始
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
		if (CFunctionLib::Memcmp(pStrCur, NUM_COL) == 0)
		{// 当たり判定データの数がある
			*pNumColData = CFunctionLib::ReadInt(pStrCur, NUM_COL);
			if (*pNumColData >= 1)
			{// 当たり判定データが１個以上ある
				pMotionCollision = new CMotionCollision*[*pNumColData];
				if (pMotionCollision != NULL)
				{// メモリが確保できた
					for (int nCntCol = 0; nCntCol < *pNumColData; nCntCol++)
					{// 当たり判定データの数だけ繰り返し
						pMotionCollision[nCntCol] = new CMotionCollision;
					}
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, COLLISION) == 0)
		{// 当たり判定データ読み込み開始の合図だった
			while (1)
			{// ループ開始
				pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
				if (CFunctionLib::Memcmp(pStrCur, INDEX) == 0)
				{// 当たり判定を取るモデルの番号情報だった
					pMotionCollision[nCntCollision]->SetModelIdx(CFunctionLib::ReadInt(pStrCur, INDEX));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSET) == 0)
				{// 当たり判定を取るモデルからのオフセット距離情報だった
					pMotionCollision[nCntCollision]->SetOffsetPos(CFunctionLib::ReadVector3(pStrCur, OFFSET));
				}
				else if (CFunctionLib::Memcmp(pStrCur, RANGE) == 0)
				{// 当たり判定を取る範囲情報だった
					pMotionCollision[nCntCollision]->SetRange(CFunctionLib::ReadFloat(pStrCur, RANGE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, START) == 0)
				{// 軌跡を出し始めるタイミング情報だった
					pMotionCollision[nCntCollision]->SetStartTiming(CFunctionLib::ReadInt(pStrCur, START));
				}
				else if (CFunctionLib::Memcmp(pStrCur, FINISH) == 0)
				{// 軌跡をしまうタイミング情報だった
					pMotionCollision[nCntCollision]->SetFinishTiming(CFunctionLib::ReadInt(pStrCur, FINISH));
				}
				else if (CFunctionLib::Memcmp(pStrCur, END_COLLISION) == 0)
				{// 当たり判定データ読み込み終了の合図だった
					nCntCollision++;
					break;  // ループ終了
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_COLLISIONSET) == 0)
		{// 当たり判定情報読み込み終了の合図だった
			break;  // ループ終了
		}
	}

	return pMotionCollision;
}

//=============================================================================
//    モーションの軌跡情報を読み取る処理
//=============================================================================
CMotionOrbit **CPlayerManager::ReadMotionOrbitSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumOrbitData)
{
	int nCntOrbit = 0;                   // 軌跡データを読み込んだ回数
	CMotionOrbit **pMotionOrbit = NULL;  // モーションの軌跡データクラスへのポインタ

	while (1)
	{// ループ開始
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
		if (CFunctionLib::Memcmp(pStrCur, NUM_ORBIT) == 0)
		{// 軌跡データの数がある
			*pNumOrbitData = CFunctionLib::ReadInt(pStrCur, NUM_ORBIT);
			if (*pNumOrbitData >= 1)
			{// 当たり判定データが１個以上ある
				pMotionOrbit = new CMotionOrbit*[*pNumOrbitData];
				if (pMotionOrbit != NULL)
				{// メモリが確保できた
					for (int nCntOr = 0; nCntOr < *pNumOrbitData; nCntOr++)
					{// 当たり判定データの数だけ繰り返し
						pMotionOrbit[nCntOr] = new CMotionOrbit;
					}
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, ORBIT) == 0)
		{// 軌跡データ読み込み開始の合図だった
			while (1)
			{// ループ開始
				pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
				if (CFunctionLib::Memcmp(pStrCur, INDEX) == 0)
				{// 軌跡を出すモデルの番号情報だった
					pMotionOrbit[nCntOrbit]->SetModelIdx(CFunctionLib::ReadInt(pStrCur, INDEX));
				}
				else if (CFunctionLib::Memcmp(pStrCur, TEX_INDEX) == 0)
				{// 軌跡に張り付けるテクスチャの番号情報だった
					pMotionOrbit[nCntOrbit]->SetTexIdx(CFunctionLib::ReadInt(pStrCur, TEX_INDEX));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSET1) == 0)
				{// 軌跡を出すモデルからのオフセット距離情報(１個目)だった
					pMotionOrbit[nCntOrbit]->SetOffsetPos1(CFunctionLib::ReadVector3(pStrCur, OFFSET1));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSET2) == 0)
				{// 軌跡を出すモデルからのオフセット距離情報(２個目)だった
					pMotionOrbit[nCntOrbit]->SetOffsetPos2(CFunctionLib::ReadVector3(pStrCur, OFFSET2));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSETAMP1) == 0)
				{// 軌跡のオフセット距離増幅値(1個目)だった
					pMotionOrbit[nCntOrbit]->SetOffsetAmp1(CFunctionLib::ReadVector3(pStrCur, OFFSETAMP1));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSETAMP2) == 0)
				{// 軌跡のオフセット距離増幅値(2個目)だった
					pMotionOrbit[nCntOrbit]->SetOffsetAmp2(CFunctionLib::ReadVector3(pStrCur, OFFSETAMP2));
				}
				else if (CFunctionLib::Memcmp(pStrCur, COLUP) == 0)
				{// 軌跡の頂点上側の色だった
					pMotionOrbit[nCntOrbit]->SetColUp(CFunctionLib::ReadVector4(pStrCur, COLUP));
				}
				else if (CFunctionLib::Memcmp(pStrCur, COLDOWN) == 0)
				{// 軌跡の頂点下側の色だった
					pMotionOrbit[nCntOrbit]->SetColDown(CFunctionLib::ReadVector4(pStrCur, COLDOWN));
				}
				else if (CFunctionLib::Memcmp(pStrCur, ALPHA_DECAY_UP) == 0)
				{// 軌跡の頂点上側の透明度減衰値だった
					pMotionOrbit[nCntOrbit]->SetAlphaDecayUp(CFunctionLib::ReadFloat(pStrCur, ALPHA_DECAY_UP));
				}
				else if (CFunctionLib::Memcmp(pStrCur, ALPHA_DECAY_DOWN) == 0)
				{// 軌跡の頂点下側の透明度減衰値だった
					pMotionOrbit[nCntOrbit]->SetAlphaDecayDown(CFunctionLib::ReadFloat(pStrCur, ALPHA_DECAY_DOWN));
				}
				else if (CFunctionLib::Memcmp(pStrCur, XBLOCK) == 0)
				{// 軌跡の横の分割数情報だった
					pMotionOrbit[nCntOrbit]->SetXBlock(CFunctionLib::ReadInt(pStrCur, XBLOCK));
				}
				else if (CFunctionLib::Memcmp(pStrCur, ZBLOCK) == 0)
				{// 軌跡の縦の分割数情報だった
					pMotionOrbit[nCntOrbit]->SetYBlock(CFunctionLib::ReadInt(pStrCur, ZBLOCK));
				}
				else if (CFunctionLib::Memcmp(pStrCur, START) == 0)
				{// 軌跡を出し始めるタイミング情報だった
					pMotionOrbit[nCntOrbit]->SetStartTiming(CFunctionLib::ReadInt(pStrCur, START));
				}
				else if (CFunctionLib::Memcmp(pStrCur, FINISH) == 0)
				{// 軌跡をしまうタイミング情報だった
					pMotionOrbit[nCntOrbit]->SetFinishTiming(CFunctionLib::ReadInt(pStrCur, FINISH));
				}
				else if (CFunctionLib::Memcmp(pStrCur, END_ORBIT) == 0)
				{// 軌跡データ読み込み終了の合図だった
					nCntOrbit++;
					break;  // ループ終了
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_ORBITSET) == 0)
		{// 軌跡情報読み込み終了の合図だった
			break;  // ループ終了
		}
	}

	return pMotionOrbit;
}

//=============================================================================
//    キーフレーム情報を読み取る処理
//=============================================================================
void CPlayerManager::ReadKeySet(char *pLine, char *pStrCur, FILE *pFile, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
{
	int nCntKey = 0;
	while (1)
	{// ループ開始
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
		if (CFunctionLib::Memcmp(pStrCur, FRAME) == 0)
		{// 再生フレーム数がある
			pPlaybackKey[nCntKeySet] = CFunctionLib::ReadInt(pStrCur, FRAME);
		}
		if (CFunctionLib::Memcmp(pStrCur, KEY) == 0)
		{// キー情報読み込み開始の合図だった
			ReadKey(pLine, pStrCur, pFile, nCntKey, AddPos, DestRot);
			nCntKey++;
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_KEYSET) == 0)
		{// キーフレーム情報読み込み終了の合図だった
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    キー情報を読み取る処理
//=============================================================================
void CPlayerManager::ReadKey(char *pLine, char *pStrCur, FILE *pFile, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
{
	while (1)
	{// ループ開始
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
		if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// 座標がある
			AddPos[nCntKey] = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
		{// 向きがある
			DestRot[nCntKey] = CFunctionLib::ReadVector3(pStrCur, ROT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_KEY) == 0)
		{// キー情報読み込み終了の合図だった
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    プレイヤーデータのスクリプトファイル名を設定する処理
//=============================================================================
void CPlayerManager::SetFileName(char *pFileName)
{
	strcpy(m_aFileName, pFileName);
}

//=============================================================================
//    モデル管轄クラスへのポインタを設定する処理
//=============================================================================
void CPlayerManager::SetModelManager(CModelManager *pModelManager)
{
	m_pModelManager = pModelManager;
}

//=============================================================================
//    プレイヤーデータのスクリプトファイル名を取得する処理
//=============================================================================
char *CPlayerManager::GetFileName(void)
{
	return m_aFileName;
}

//=============================================================================
//    モデル管轄クラスへのポインタを取得する処理
//=============================================================================
CModelManager *CPlayerManager::GetModelManager(void)
{
	return m_pModelManager;
}


//*****************************************************************************
//     CPlayerの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CPlayer::CPlayer(int nPriority, OBJTYPE objType) : CCharacter(nPriority, objType)
{
	// 各種値の設定
	m_State = STATE_NORMAL;                  // 状態
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 移動量
	m_fAccel = 0.0f;                         // 加速度
	m_fInertia = 0.0f;                       // 慣性
	m_fJumpPower = 0.0f;                     // ジャンプ量
	m_fRivisionRot = 0.0f;                   // 向きを補正する倍率
	m_bJump = false;                         // ジャンプしているかどうか
	m_bAction = false;                       // アクションしているかどうか
	m_bOrbit = false;                        // 軌跡を出しているかどうか
	m_pEffectManager = NULL;                 // エフェクト管轄クラスへのポインタ
	m_pOrbitEffect = NULL;                   // 軌跡エフェクトクラスへのポインタ
	m_bBooking = false;                      // アクションの予約が入っているかどうか
}

//=============================================================================
//    デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, CTextureManager *pTextureManager, int nNumParts, float fAccel, float fInertia, float fJumpPower, float fGravity, float fRivisionRot, float fColHeight, float fColRange,int nPriority)
{
	CPlayer *pPlayer = NULL;               // プレイヤークラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pPlayer == NULL)
		{// メモリが空になっている
			pPlayer = new CPlayer(nPriority);
			if (pPlayer != NULL)
			{// インスタンスを生成できた
				if (FAILED(pPlayer->Init(pos, rot, apModel, pMotionManager, pTextureManager, nNumParts, fAccel, fInertia, fJumpPower, fGravity, fRivisionRot, fColHeight, fColRange)))
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

	return pPlayer;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, CTextureManager *pTextureManager,int nNumParts ,float fAccel, float fInertia, float fJumpPower, float fGravity, float fRivisionRot, float fColHeight, float fColRange)
{
	// 各種値の設定
	SetPos(pos);                        // 現在の座標
	SetRot(rot);                        // 現在の向き
	SetNumParts(nNumParts);             // パーツ数
	SetModel(apModel);                  // モデルクラスへのポインタ
	SetMotionManager(pMotionManager);   // モーション管轄クラスへのポインタ
	SetTextureManager(pTextureManager); // テクスチャ管轄クラスへのポインタ
	SetAccel(fAccel);                   // 加速度
	SetInertia(fInertia);               // 慣性
	SetGravity(fGravity);               // 重力
	SetJumpPower(fJumpPower);           // ジャンプ力
	SetRivisionRot(fRivisionRot);       // 向きを補正する倍率
	SetColHeight(fColHeight);           // 当たり判定を取る高さ
	SetColRange(fColRange);             // 当たり判定を取る範囲

	// エフェクト管轄クラスへのポインタを入手しておく
	if (m_pEffectManager == NULL)
	{// ポインタが確保できる状態である
		CScene *pScene = NULL;
		CScene *pSceneNext = NULL;
		for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
		{// 優先順位の数だけ繰り返し
			pScene = GetTop(nCntPriority);
			while (pScene != NULL)
			{// メモリが空になるまで繰り返し
				pSceneNext = pScene->GetNext();
				if (pScene->GetObjType() == OBJTYPE_EFFECTMANAGER)
				{// エフェクト管轄クラスへのポインタだった
					m_pEffectManager = (CEffectManager*)pScene;
				}
				pScene = pSceneNext;
			}
		}
	}

	// モデルの位置をニュートラルの位置にする
	SetDefaultPos();

	// モデルの向きをニュートラルの向きにする
	SetDefaultRot();

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	// エフェクト管轄クラスへのポインタを空にしておく
	m_pEffectManager = NULL;

	// 軌跡エフェクトクラスへのポインタを空にしておく
	m_pOrbitEffect = NULL;

	// 共通の終了処理
	CCharacter::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CPlayer::Update(void)
{
	// 前回乗っていたオブジェクトへのポインタを保存
	SetLandSceneOld(GetLandScene());

	// 乗っているオブジェクトへのポインタを空にする
	SetLandScene(NULL);

	// 前回の座標を保存
	SetPosOld(GetPos());

	if (m_State != STATE_LANDING)
	{// アクション状態でないかつ着地状態でない
		if (m_bAction != true)
		{// アクション状態でない
			// 移動量計算処理
			InputMove();
		}
	}

	// 移動処理
	Movement();

	// 当たり判定処理
	Collision();

	if (m_State != STATE_LANDING)
	{// 着地状態でない
	    // アクション処理
		Action();
	}

	// 状態遷移処理
	Statement();

	// エフェクトを作成する処理
	CreateEffect();

	if (GetMotionManager() != NULL)
	{// モーション管理クラスが生成されている
		GetMotionManager()->Update(GetModel());
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	// 共通の描画処理
	CCharacter::Draw();
}

//=============================================================================
//    入力による移動量計算処理
//=============================================================================
void CPlayer::InputMove(void)
{
	// 各種値の取得
	D3DXVECTOR3 DestRot = GetDestRot();   // 目的の向き

	// キーボードを取得
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// コントローラーを取得
	CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(0);

	// カメラを取得
	CCamera *pCamera = CManager::GetCamera();

	if (pKeyboard->GetPress(DIK_A) == true)
	{// 左方向の入力がされた
		if (pKeyboard->GetPress(DIK_W) == true)
		{// 同時に上方向の入力がされた
		    // 移動処理
			m_Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * m_fAccel;
			m_Move.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * m_fAccel;

			// 目的の角度変更
			DestRot.y = pCamera->GetRot().y + (D3DX_PI * 0.75f);
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// 同時に下方向の入力がされた
		    // 移動処理
			m_Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * m_fAccel;
			m_Move.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * m_fAccel;

			// 目的の角度変更
			DestRot.y = pCamera->GetRot().y + (D3DX_PI * 0.25f);
		}
		else
		{// 何も押されてない
		    // 移動処理
			m_Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * m_fAccel;
			m_Move.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * m_fAccel;

			// 目的の角度変更
			DestRot.y = pCamera->GetRot().y + (D3DX_PI * 0.5f);
		}
	}
	else if (pKeyboard->GetPress(DIK_D) == true)
	{// 右方向の入力がされた
		if (pKeyboard->GetPress(DIK_W) == true)
		{// 同時に上方向の入力がされた
		    // 移動処理
			m_Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * m_fAccel;
			m_Move.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * m_fAccel;

			// 目的の角度変更
			DestRot.y = pCamera->GetRot().y + (D3DX_PI * -0.75f);
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// 同時に下方向の入力がされた
		    // 移動処理
			m_Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * m_fAccel;
			m_Move.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * m_fAccel;

			// 目的の角度変更
			DestRot.y = pCamera->GetRot().y + (D3DX_PI * -0.25f);
		}
		else
		{// 何も押されてない
		    // 移動処理
			m_Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * m_fAccel;
			m_Move.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * m_fAccel;

			// 目的の角度変更
			DestRot.y = pCamera->GetRot().y + (D3DX_PI * -0.5f);
		}
	}
	else if (pKeyboard->GetPress(DIK_W) == true)
	{// 上方向の入力がされた
	    // 移動処理
		m_Move.x += sinf(pCamera->GetRot().y) * m_fAccel;
		m_Move.z += cosf(pCamera->GetRot().y) * m_fAccel;

		// 目的の角度変更
		DestRot.y = pCamera->GetRot().y + D3DX_PI;
	}
	else if (pKeyboard->GetPress(DIK_S) == true)
	{// 下方向の入力がされた
	    // 移動処理
		m_Move.x += sinf(pCamera->GetRot().y + D3DX_PI) * m_fAccel;
		m_Move.z += cosf(pCamera->GetRot().y + D3DX_PI) * m_fAccel;

		// 目的の角度変更
		DestRot.y = pCamera->GetRot().y;
	}
	else
	{
		if (pJoyStick != NULL)
		{// ジョイスティックが取得できた
			if (pJoyStick->GetLeftAxisX() >= DI_STICK_AXIS_MIN || pJoyStick->GetLeftAxisX() <= -DI_STICK_AXIS_MIN
				|| pJoyStick->GetLeftAxisY() >= DI_STICK_AXIS_MIN || pJoyStick->GetLeftAxisY() <= -DI_STICK_AXIS_MIN)
			{// 左アナログ入力がされている
			    // スティックの倒された長さの比率を計算
				float fLengthX = (float)(sqrt(pJoyStick->GetLeftAxisX() * pJoyStick->GetLeftAxisX() + pJoyStick->GetLeftAxisY() * pJoyStick->GetLeftAxisY()) / DI_JOY_STICK_INPUT);

				// コサインカーブで比率を補正
				float fRivision = cosf(sqrtf(2) - fLengthX);
				if (fRivision < 1.0)
				{// 倍率が1を超えていない
					fRivision += 0.09f;  // 足りない分を補正
					if (fRivision >= 1.0f)
					{// 倍率が1を超えてしまった
						fRivision = 1.0f;
					}
				}

				// 移動量計算処理
				m_Move.x += sinf(pJoyStick->GetLeftAxis() - pCamera->GetRot().y) * (m_fAccel * fRivision);
				m_Move.z -= cosf(pJoyStick->GetLeftAxis() - pCamera->GetRot().y) * (m_fAccel * fRivision);

				// 目的の角度変更
				DestRot.y = pCamera->GetRot().y - pJoyStick->GetLeftAxis();
			}
			else if (pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_28) == true || pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_29) == true
				|| pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_30) == true || pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_31) == true)
			{// 十字キー入力がされている
			    // 移動量計算処理
				m_Move.x += sinf(pCamera->GetRot().y + pJoyStick->GetRadian()) * (m_fAccel);
				m_Move.z += cosf(pCamera->GetRot().y + pJoyStick->GetRadian()) * (m_fAccel);

				// 目的の角度変更
				DestRot.y = pCamera->GetRot().y + (pJoyStick->GetRadian() - D3DX_PI);
			}
		}
	}

	// 各種値の設定
	SetDestRot(DestRot);    // 目的の向き
}

//=============================================================================
//    計算された移動量を座標情報に加える処理
//=============================================================================
void CPlayer::Movement(void)
{
	// 各種値の取得
	D3DXVECTOR3 pos = GetPos();           // 現在の座標
	D3DXVECTOR3 rot = GetRot();           // 現在の向き
	D3DXVECTOR3 DestRot = GetDestRot();   // 目的の向き
	D3DXVECTOR3 DiffRot = GetDiffRot();   // 目的の向きと現在の向きの差分
	float fGravity = GetGravity();        // 重力

	// 現在の座標に移動量を加える
	pos += m_Move;

	// 重力を加える
	m_Move.y += fGravity;

	// 移動量に慣性を加える
	m_Move.x += (0.0f - m_Move.x) * m_fInertia;
	m_Move.z += (0.0f - m_Move.z) * m_fInertia;

	// 角度の修正
	DiffRot.y = DestRot.y - rot.y;   // 現在の向きと目的の向きの差分を計算

	if (DiffRot.y > D3DX_PI)
	{// 差分がD3DX_PIを超えた
		DiffRot.y -= D3DX_PI * 2.0f;
	}
	if (DiffRot.y < -D3DX_PI)
	{// 差分が-D3DX_PIを超えた
		DiffRot.y += D3DX_PI * 2.0f;
	}

	// 現在の向きに目的の向きとの差分を倍率で補正する
	rot.y += DiffRot.y * m_fRivisionRot;

	if (rot.y > D3DX_PI)
	{// 現在の向きがD3DX_PIを超えた
		rot.y -= D3DX_PI * 2.0f;
	}
	if (rot.y < -D3DX_PI)
	{// 現在の向きが-D3DX_PIを超えた
		rot.y += D3DX_PI * 2.0f;
	}

	// 各種値の設定
	SetPos(pos);          // 現在の座標
	SetRot(rot);          // 現在の向き
	SetDiffRot(DiffRot);  // 目的の向きと現在の向きの差分
}

//=============================================================================
//    当たり判定処理
//=============================================================================
void CPlayer::Collision(void)
{
	bool bLand = false;   // 乗っているかどうか

	// 各種値の取得
	D3DXVECTOR3 pos = GetPos();          // 現在の座標

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
			if (bLand != true)
			{// まだ何かに乗っていない
				if (pScene->GetObjType() == OBJTYPE_MESHFIELD)
				{// 地面クラスへのポインタだったら
					pField = (CMeshField*)pScene;
					if (pField != NULL)
					{// 地面が取得できた
						float fFieldHeight = pField->GetPolyHeight(pos);
						if (pos.y < fFieldHeight)
						{// 座標が地面より下になった
							m_Move.y = 0.0f;
							pos.y = fFieldHeight;
							if (m_bJump == true)
							{
								Landing();
							}
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_OBJECT)
				{// 配置物だったら
					pObject = (CObject*)pScene;
					if (pObject != NULL)
					{// 配置物が取得できた
						if (pObject->Collision(&pos, &GetPosOld(), &m_Move, GetColHeight(), GetColRange(), this) == true)
						{// 配置物の上に乗っている
							if (GetLand() == false)
							{// 着地していなかったら
								SetLand(true);
								Landing();
							}

							if (GetLandScene() == NULL && GetLand() == true && m_bJump == false)
							{// 何かに乗っていた
								pos.y = pObject->GetPos().y + pObject->GetVtxMax().y;
							}
							SetLandScene((CScene*)pObject);
							bLand = true;
						}
					}
				}
			}
			// 次のオブジェクトへのポインタを取得
			pScene = pSceneNext;
		}
	}

	// 各種値の設定
	SetPos(pos);         // 現在の座標
	SetLand(bLand);      // オブジェクトに乗っているかどうか
}

//=============================================================================
//    アクション処理
//=============================================================================
void CPlayer::Action(void)
{
	// キーボードを取得
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// コントローラーを取得
	CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(0);

	if (pKeyboard->GetTrigger(DIK_RETURN) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_2) == true)
	{// ジャンプボタンが押された
		if (m_bJump == false)
		{// ジャンプしていない
		    // 軌跡を消す
			if (m_pOrbitEffect != NULL)
			{
				m_bOrbit = false;
				m_pOrbitEffect->Uninit();
				m_pOrbitEffect = NULL;
			}

			m_Move.y += m_fJumpPower;
			m_bJump = true;
			m_State = STATE_JUMP;
			GetMotionManager()->SwitchMotion(GetModel(), m_State);
		}
	}

	else if (pKeyboard->GetTrigger(DIK_SPACE) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_3) == true)
	{// ジャンプボタンが押された
		if (m_bAction == false)
		{// アクションをしていない
			if (m_State == STATE_MOVE)
			{// 動いている状態だったら
				if (GetLand() == true)
				{// 何かに乗っている
					CalcOffsetRot();
				}
			}

			m_bAction = true;
			m_State = STATE_ATTACK_0;
			GetMotionManager()->SwitchMotion(GetModel(), m_State);
			if (m_pOrbitEffect != NULL)
			{
				m_pOrbitEffect->Uninit();
				m_pOrbitEffect = NULL;
			}
		}
		else
		{// 攻撃をすでに行っている
			if (GetMotionManager()->GetMotion()[m_State]->GetAttack() != NULL)
			{// 次のモーションに派生できるモーションだった
				// 各種データを用意
				int nNextMotionIdx = GetMotionManager()->GetMotion()[m_State]->GetAttack()->GetNextMotionIdx();
				int nStartTiming = GetMotionManager()->GetMotion()[m_State]->GetAttack()->GetStartTiming();
				int nFinishTiming = GetMotionManager()->GetMotion()[m_State]->GetAttack()->GetFinishTiming();
				if (GetMotionManager()->GetAllCounter() >= nStartTiming && GetMotionManager()->GetAllCounter() <= nFinishTiming)
				{// 次のモーションに派生できるフレーム内で入力が行われたか予約が入っている
					// 軌跡を消す
					if (m_pOrbitEffect != NULL)
					{
						m_bOrbit = false;
						m_pOrbitEffect->Uninit();
						m_pOrbitEffect = NULL;
					}
					m_State = (STATE)nNextMotionIdx;
					GetMotionManager()->SwitchMotion(GetModel(), m_State);
				}
				else if(m_bBooking == false)
				{// まだ予約が入っていない
					if (GetMotionManager()->GetAllCounter() <= nFinishTiming)
					{// まだ次のモーションに派生できる
						m_bBooking = true;   // 予約を入れておく
					}
				}
			}
		}
	}

	if (m_bBooking == true)
	{
		CDebugProc::Print("アクション予約");
		if (GetMotionManager()->GetMotion()[m_State]->GetAttack() != NULL)
		{// 次のモーションに派生できるモーションだった
		    // 各種データを用意
			int nNextMotionIdx = GetMotionManager()->GetMotion()[m_State]->GetAttack()->GetNextMotionIdx();
			int nStartTiming = GetMotionManager()->GetMotion()[m_State]->GetAttack()->GetStartTiming();
			int nFinishTiming = GetMotionManager()->GetMotion()[m_State]->GetAttack()->GetFinishTiming();
			if (GetMotionManager()->GetAllCounter() >= nStartTiming)
			{// 次のモーションに派生できるフレームになっている
			    // 軌跡を消す
				if (m_pOrbitEffect != NULL)
				{
					m_bOrbit = false;
					m_pOrbitEffect->Uninit();
					m_pOrbitEffect = NULL;
				}
				m_State = (STATE)nNextMotionIdx;
				GetMotionManager()->SwitchMotion(GetModel(), m_State);

				// 予約は外しておく
				m_bBooking = false;
			}
		}
	}
}

//=============================================================================
//    状態遷移処理
//=============================================================================
void CPlayer::Statement(void)
{
	if (m_State != STATE_JUMP && m_State != STATE_LANDING && m_bAction == false)
	{// ジャンプしていないかつ着地状態でないかつアクションをしていない
		if (m_Move.x <= 0.1f && m_Move.x >= -0.1f
			&& m_Move.z <= 0.1f && m_Move.z >= -0.1f)
		{// 移動していない
			if (m_State != STATE_NORMAL)
			{// 通常状態ではない
				// 状態を通常の状態に
				m_State = STATE_NORMAL;

				// モーション切り替え処理
				GetMotionManager()->SwitchMotion(GetModel(), m_State);

				if (GetLand() == true)
				{// 何かに乗っている
					CalcOffsetRot();
				}
			}
		}
		else
		{// 移動している
			if (m_State != STATE_MOVE)
			{// 歩く状態ではない
				m_State = STATE_MOVE;

				// モーション切り替え処理
				GetMotionManager()->SwitchMotion(GetModel(), m_State);
			}
		}
	}
	else
	{// ジャンプしているか着地状態の時
		if (m_State == STATE_LANDING)
		{// 着地状態の時
			if (GetMotionManager()->GetMotion()[m_State]->GetState() == CMotion::STATE_STOP)
			{// モーション再生が終了した
				m_State = STATE_NORMAL;

				// モーション切り替え処理
				GetMotionManager()->SwitchMotion(GetModel(), m_State);
			}
		}
		if (m_bAction == true)
		{// アクションをしている
			if (GetMotionManager()->GetMotion()[m_State]->GetCollision() != NULL)
			{// 当たり判定データクラスが生成されている
				for (int nCntCol = 0; nCntCol < GetMotionManager()->GetMotion()[m_State]->GetNumColData(); nCntCol++)
				{// 当たり判定データの数だけ繰り返し
					// 当たり判定に必要なデータを宣言
					int nModelIdx = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetModelIdx();
					float fRange = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetRange();
					D3DXVECTOR3 Offset = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetOffsetPos();
					int nStartTiming = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetStartTiming();
					int nFinishTiming = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetFinishTiming();
					if (GetMotionManager()->GetAllCounter() >= nStartTiming && GetMotionManager()->GetAllCounter() <= nFinishTiming)
					{// 当たり判定を発生させるタイミングならば
						// 攻撃時の当たり判定を発生させる
						AttackCollision(nModelIdx, fRange, Offset);
					}
				}
			}

			if (GetMotionManager()->GetMotion()[m_State]->GetOrbit() != NULL)
			{// 軌跡データクラスが生成されている
				if (GetMotionManager()->GetAllCounter() >= GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetStartTiming() && m_bOrbit == false)
				{// 軌跡を出すタイミングになった
					if (m_pOrbitEffect == NULL)
					{// オービットエフェクトクラスへのポインタが空になっている
						// 軌跡に必要な情報を宣言
						int nModelIdx = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetModelIdx();
						int nTexIdx = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetTexIdx();
						D3DXVECTOR3 OffSetPos1 = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetOffsetPos1();
						D3DXVECTOR3 OffSetPos2 = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetOffsetPos2();
						D3DXVECTOR3 OffSetAmp1 = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetOffsetAmp1();
						D3DXVECTOR3 OffSetAmp2 = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetOffsetAmp2();
						D3DXCOLOR ColUp = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetColUp();
						D3DXCOLOR ColDown = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetColDown();
						float fAlphaDecayUp = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetAlphaDecayUp();
						float fAlphaDecayDown = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetAlphaDecayDown();
						int nXBlock = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetXBlock();
						int nYBlock = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetYBlock();
						D3DXMATRIX *pMtxParent = GetModel(nModelIdx)->GetMtxWorldPointer();
						m_pOrbitEffect = COrbitEffect::Create(OffSetPos1, OffSetPos2, OffSetAmp1, OffSetAmp2, ColUp, ColDown, fAlphaDecayUp, fAlphaDecayDown, nXBlock, nYBlock, pMtxParent);
						if (m_pOrbitEffect != NULL)
						{// 軌跡を生成できた
							m_pOrbitEffect->BindTexture(GetTextureManager()->GetTexture(nTexIdx));
						}
						m_bOrbit = true;
					}
				}
				if (GetMotionManager()->GetAllCounter() >= GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetFinishTiming())
				{// 軌跡をしまうタイミングになった
					if (m_pOrbitEffect != NULL)
					{
						m_pOrbitEffect->Uninit();
						m_pOrbitEffect = NULL;
					}
				}
			}

			if (GetMotionManager()->GetMotion()[m_State]->GetState() == CMotion::STATE_STOP)
			{// モーション再生が終了した
				m_bAction = false;
				m_bOrbit = false;
				m_State = STATE_NORMAL;
				m_bBooking = false;

				// 軌跡を開放しておく
				if (m_pOrbitEffect != NULL)
				{
					m_pOrbitEffect->Uninit();
					m_pOrbitEffect = NULL;
				}

				// モーション切り替え処理
				GetMotionManager()->SwitchMotion(GetModel(), m_State);
			}
		}
	}
}


//=============================================================================
//    着地したときの処理
//=============================================================================
void CPlayer::Landing(void)
{
	// ジャンプしていない状態に
	m_bJump = false;

	if (m_bAction == false)
	{// アクションをしていない
		if (m_Move.x <= 0.1f && m_Move.x >= -0.1f
			&& m_Move.z <= 0.1f && m_Move.z >= -0.1f)
		{// 移動していない
			// 着地のモーションに切り替える
			m_State = STATE_LANDING;
			GetMotionManager()->SwitchMotion(GetModel(), m_State);
		}
		else
		{// 移動している
			m_State = STATE_MOVE;

			// モーション切り替え処理
			GetMotionManager()->SwitchMotion(GetModel(), m_State);
		}
	}

	if (GetLand() == true)
	{// オフセット位置をワールドマトリックスを使って割り出す
		CalcOffsetRot();
	}
}

//=============================================================================
//    攻撃時の当たり判定処理
//=============================================================================
bool CPlayer::AttackCollision(int nModelIdx, float fRange, D3DXVECTOR3 Offset)
{
	bool bHit = false;                 // 攻撃がヒットしたかどうか
	float fEnemyRange = 0.0f;          // 敵の当たり判定を取る範囲
	D3DXVECTOR3 OffSet = Offset;       // 当たり判定をとる場所(親モデルからのオフセット)
	float fHitRange = 0.0f;            // 当たり判定を取る長さ
	float fLength = 0.0f;              // 当たり判定を取る場所と敵との距離

	// 当たり判定を取る座標を確定させる
	CModel *pModel = GetModel(nModelIdx);
	if (pModel != NULL)
	{
		D3DXMATRIX mtxWorld = pModel->GetMtxWorld();
		D3DXVec3TransformCoord(&OffSet, &OffSet, &mtxWorld);
	}

	// 判定開始
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	CEnemy *pEnemy = NULL;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// 優先順位の数だけ繰り返し
		pScene = GetTop(nCntPriority);
		while (pScene != NULL)
		{// メモリが空になるまで繰り返し
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_ENEMY)
			{// 敵クラスへのポインタだった
				pEnemy = (CEnemy*)pScene;
				if (pEnemy != NULL)
				{// 敵クラスのポインタが取得できた
					// 敵の当たり判定を取る範囲を取得
					fEnemyRange = pEnemy->GetColRange();

				    // 当たり判定を取る長さを計算
					fHitRange = sqrtf((fEnemyRange + fRange) * (fEnemyRange + fRange));

					// 当たり判定を取る座標と敵の座標との距離を測る
					D3DXMATRIX EnemyMtxWorld;
					D3DXVECTOR3 EnemyPos;
					CModel *pModel = pEnemy->GetModel(0);
					if (pModel != NULL)
					{
						EnemyMtxWorld = pModel->GetMtxWorld();
						EnemyPos = D3DXVECTOR3(EnemyMtxWorld._41, EnemyMtxWorld._42, EnemyMtxWorld._43);
						fLength = sqrtf((EnemyPos.x - OffSet.x) * (EnemyPos.x - OffSet.x) + (EnemyPos.y - OffSet.y) * (EnemyPos.y - OffSet.y) + (EnemyPos.z - OffSet.z) * (EnemyPos.z - OffSet.z));
					}

					if (fLength <= fHitRange)
					{// 当たり判定を取る座標と敵の座標との距離が当たり判定を取る長さより短い
						if (pEnemy->GetState() != CEnemy::STATE_DAMAGE)
						{// 敵の状態がダメージ状態でない
							D3DXVECTOR3 EffectPos = D3DXVECTOR3(pEnemy->GetModel()[0]->GetMtxWorld()._41, pEnemy->GetModel()[0]->GetMtxWorld()._42, pEnemy->GetModel()[0]->GetMtxWorld()._43);
							m_pEffectManager->SetEffect(EffectPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEffectManager::EFFECT_TYPE_SLASH);
							m_pEffectManager->SetEffect(EffectPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEffectManager::EFFECT_TYPE_SLASHPAR);
							pEnemy->Hit(1);
							bHit = true;
						}
						CDebugProc::Print("攻撃がヒットしています\n");
					}
				}
			}
			pScene = pSceneNext;
		}
	}

	return bHit;  // ヒットしたかどうかを返す
}

//=============================================================================
//    エフェクトを作る処理
//=============================================================================
void CPlayer::CreateEffect(void)
{
	if (m_pEffectManager != NULL)
	{// エフェクト管轄クラスへのポインタが入手出来ている
		if (m_State == STATE_MOVE)
		{// 状態が動く状態である
			if (GetMotionManager()->GetCounter() == 0 && GetMotionManager()->GetKey() == 0
				|| GetMotionManager()->GetCounter() == 0 && GetMotionManager()->GetKey() == 2)
			{// エフェクトを作るタイミングになった
				m_pEffectManager->SetEffect(GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEffectManager::EFFECT_TYPE_DASH);
			}
		}
	}
}

//=============================================================================
//    乗っているオブジェクトからのオフセット向きを計算する処理
//=============================================================================
void CPlayer::CalcOffsetRot(void)
{
	if (GetLandScene() != NULL)
	{// プレイヤーが乗っているクラスへのポインタが取得できている
		if (GetLandScene()->GetObjType() == OBJTYPE_OBJECT)
		{// 配置物クラスだったら
			CObject *pObject = (CObject*)GetLandScene();
			D3DXVECTOR3 ObjRot = pObject->GetRot();
			D3DXVECTOR3 OffSetRot;
			OffSetRot = GetRot() - ObjRot;
			if (OffSetRot.y > D3DX_PI)
			{// 円周率を超えた
				OffSetRot.y -= D3DX_PI * 2.0f;
			}
			if (OffSetRot.y < -D3DX_PI)
			{// 円周率を超えた
				OffSetRot.y += D3DX_PI * 2.0f;
			}
			SetLandOffsetRot(OffSetRot);
		}
	}
}

//=============================================================================
//    状態を取得する処理
//=============================================================================
void CPlayer::SetState(const STATE State)
{
	m_State = State;
}

//=============================================================================
//    移動量を設定する処理
//=============================================================================
void CPlayer::SetMove(const D3DXVECTOR3 move)
{
	m_Move = move;
}

//=============================================================================
//    加速度を設定する処理
//=============================================================================
void CPlayer::SetAccel(const float fAccel)
{
	m_fAccel = fAccel;
}

//=============================================================================
//    慣性を設定する処理
//=============================================================================
void CPlayer::SetInertia(const float fInertia)
{
	m_fInertia = fInertia;
}

//=============================================================================
//    ジャンプ量を設定する処理
//=============================================================================
void CPlayer::SetJumpPower(const float fJumpPower)
{
	m_fJumpPower = fJumpPower;
}

//=============================================================================
//    向きを補正する倍率を設定する処理
//=============================================================================
void CPlayer::SetRivisionRot(const float fRivisionRot)
{
	m_fRivisionRot = fRivisionRot;
}

//=============================================================================
//    ジャンプしているかどうかを設定する処理
//=============================================================================
void CPlayer::SetJump(const bool bJump)
{
	m_bJump = bJump;
}

//=============================================================================
//    アクションしているかどうかを設定する処理
//=============================================================================
void CPlayer::SetAction(const bool bAction)
{
	m_bAction = bAction;
}

//=============================================================================
//    移動量を取得する処理
//=============================================================================
D3DXVECTOR3 CPlayer::GetMove(void)
{
	return m_Move;
}

//=============================================================================
//    状態を取得する処理
//=============================================================================
CPlayer::STATE CPlayer::GetState(void)
{
	return m_State;
}

//=============================================================================
//    加速度を取得する処理
//=============================================================================
float CPlayer::GetAccel(void)
{
	return m_fAccel;
}

//=============================================================================
//    慣性を取得する処理
//=============================================================================
float CPlayer::GetInertia(void)
{
	return m_fInertia;
}

//=============================================================================
//    ジャンプ量を取得する処理
//=============================================================================
float CPlayer::GetJumpPower(void)
{
	return m_fJumpPower;
}

//=============================================================================
//    向きを補正する倍率を取得する処理
//=============================================================================
float CPlayer::GetRivisionRot(void)
{
	return m_fRivisionRot;
}

//=============================================================================
//    ジャンプしているかどうかを取得する処理
//=============================================================================
bool CPlayer::GetJump(void)
{
	return m_bJump;
}

//=============================================================================
//    アクションしているかどうかを取得する処理
//=============================================================================
bool CPlayer::GetAction(void)
{
	return m_bAction;
}