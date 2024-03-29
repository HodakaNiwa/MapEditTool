//*****************************************************************************
//
//     シーンの処理[scene.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "scene.h"
#include "debuglog.h"

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
int     CScene::m_nNumAll;                       // シーンのインスタンス生成数
CScene *CScene::m_apTop[MAX_PRIORITY_NUM] = {};  // 先頭のシーンクラスへのポインタ(開始位置がわからなくなるので保存する)
CScene *CScene::m_apCur[MAX_PRIORITY_NUM] = {};  // 現在の最後尾シーンクラスへのポインタ(新しいシーンクラスへのポインタを追加するために保存)

//=============================================================================
//    コンストラクタ
//=============================================================================
CScene::CScene(int nPriority, OBJTYPE objType)
{
	// 各種値のクリア
	m_pPrev = NULL;            // 自身の前のアドレス
	m_pNext = NULL;            // 自身の次のアドレス
	m_bDeath = false;          // 死亡フラグ
	m_ObjType = objType;       // オブジェクトの種類
	m_nPriority = nPriority;   // 描画優先順位

	if (m_apTop[nPriority] == NULL)
	{// 先頭アドレスが空である
		m_apTop[nPriority] = this; // 自身のポインタを代入
	}
	CScene *pCurOld = m_apCur[nPriority];  // 現在の最後尾を保存(あとで教えるのに使う)
	m_apCur[nPriority] = this;             // 現在の最後尾を更新
	m_apCur[nPriority]->m_pPrev = pCurOld; // 自身の前のアドレスに前回の最後尾を保存する
	if (pCurOld != NULL)
	{// 前回の最後尾アドレスがある
		pCurOld->m_pNext = m_apCur[nPriority]; // 前回の最後尾アドレスに次のアドレスが自身であることを伝える
	}

	m_nNumAll++;  // インスタンス生成数を増やす
}

//=============================================================================
//    デストラクタ
//=============================================================================
CScene::~CScene()
{

}

//=============================================================================
//    シーンをすべて更新する
//=============================================================================
void CScene::UpdateAll(void)
{
	int nCountPriority[MAX_PRIORITY_NUM] = { 0,0,0,0,0,0,0,0 };   // 更新処理を行ったオブジェクトの数(優先順位の数だけ用意)

	CScene *pScene;  // 更新処理の開始ポインタを宣言
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// 描画優先順位の数だけ繰り返し
		pScene = m_apTop[nCntPriority];  // 更新処理の開始ポインタを代入
		while (pScene != NULL)
		{// メモリが空になるまでループ
			CScene *pSceneNext = pScene->m_pNext; // 自身の次のアドレスを保存(更新処理でポインタが空になる可能性があるので)
			pScene->Update();                     // 更新処理
			pScene = pSceneNext;                  // 次の更新処理は自身の次のアドレスの更新処理を行う
			nCountPriority[nCntPriority]++;
		}
	}

	// 死亡フラグチェック
	DeathCheck();
}
//=============================================================================
//    シーンをすべて描画する
//=============================================================================
void CScene::DrawAll(void)
{
	CScene *pScene;  // 描画処理の開始ポインタを宣言
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// 描画優先順位の数だけ繰り返し
		pScene = m_apTop[nCntPriority];  // 描画処理の開始ポインタを代入
		while (pScene != NULL)
		{// メモリが空になるまでループ
			CScene *pSceneNext = pScene->m_pNext; // 自身の次のアドレスを保存(描画処理で自身のポインタが空になる可能性があるので)
			pScene->Draw();                       // 描画処理
			pScene = pSceneNext;                  // 次の描画処理の自身の次のアドレスの描画処理を行う
		}
	}
}
//=============================================================================
//    シーンを全て開放する処理
//=============================================================================
void CScene::ReleaseAll(void)
{
	CScene *pScene;  // 終了処理の開始ポインタを宣言
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// 描画優先順位の数だけ繰り返し
		pScene = m_apTop[nCntPriority];  // 終了処理の開始ポインタを代入
		while (pScene != NULL)
		{// メモリが空になるまでループ
			CScene *pSceneNext = pScene->m_pNext; // 自身の次のアドレスを保存(終了処理で自身のポインタが空になる可能性があるので)
			pScene->Uninit();                     // 終了処理
			pScene = pSceneNext;                  // 次の終了処理の自身の次のアドレスの終了処理を行う
		}
	}
	// 死亡フラグチェック
	DeathCheck();

	// メモリを初期化しておく
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// 描画優先順位の数だけ繰り返し
		m_apTop[nCntPriority] = NULL;   // 先頭アドレス
		m_apCur[nCntPriority] = NULL;   // 現在の最後尾アドレス
	}
	m_nNumAll = 0;   // インスタンス生成数を戻す
}

//=============================================================================
//    死亡フラグを立てる処理
//=============================================================================
void CScene::Release(void)
{
	if (m_bDeath == false)
	{// 死亡フラグがたっていない
		m_bDeath = true;  // 死亡フラグを立てる
	}
	else
	{// 死亡フラグがすでに立っている
		if (m_pPrev != NULL)
		{// 自身の前のアドレスが空ではない
			m_pPrev->m_pNext = m_pNext;
		}
		if (m_pNext != NULL)
		{// 自身の次のアドレスが空ではない
			m_pNext->m_pPrev = m_pPrev;
		}
		if (this == m_apTop[m_nPriority])
		{// 自身のアドレスが先頭アドレスだった
			m_apTop[m_nPriority] = m_pNext;
		}
		if (this == m_apCur[m_nPriority])
		{// 自身のアドレスが現在の最後尾アドレスだった
			m_apCur[m_nPriority] = m_pPrev;
		}
		// メモリの開放
		delete this;
	}

	m_nNumAll--;  // インスタンス生成数を減らす
}

//=============================================================================
//    死亡フラグを確認する処理
//=============================================================================
void CScene::DeathCheck(void)
{
	CScene *pScene;                  // チェック開始ポインタ
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// 描画優先順位の数だけ繰り返し
		pScene = m_apTop[nCntPriority];           // チェック開始ポインタを先頭にする
		while (pScene != NULL)
		{// メモリが空になるまでループ
			CScene *pSceneNext = pScene->m_pNext; // 自身の次のアドレスを保存
			if (pScene->m_bDeath == true)
			{// 死亡フラグがたっている
				pScene->Release();                // リストから削除する
			}
			pScene = pSceneNext;                  // 次の死亡フラグチェックは自身の次のアドレスのチェックを行う
		}
	}
}

//=============================================================================
//    シーンを生成した数を取得
//=============================================================================
int CScene::GetNumAll(void)
{
	return m_nNumAll;
}

//=============================================================================
//    先頭アドレスを取得
//=============================================================================
CScene *CScene::GetTop(int nPriority)
{
	return m_apTop[nPriority];
}

//=============================================================================
//    オブジェクトの種類を設定
//=============================================================================
void CScene::SetObjType(OBJTYPE ObjType)
{
	m_ObjType = ObjType;
}

//=============================================================================
//    オブジェクトの種類を取得
//=============================================================================
CScene::OBJTYPE CScene::GetObjType(void)
{
	return m_ObjType;
}

//=============================================================================
//    自身の次のアドレスを取得
//=============================================================================
CScene *CScene::GetNext(void)
{
	return m_pNext;
}

//=============================================================================
//    自身の前のアドレスを取得
//=============================================================================
CScene *CScene::GetPrev(void)
{
	return m_pPrev;
}