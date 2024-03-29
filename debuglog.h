//*****************************************************************************
//
//     デバッグ情報の処理[debuglog.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _DEBUGLOG_H_
#define _DEBUGLOG_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define MAX_DEBUG_LOG (10000)      // 文字列格納量の最大値

//*****************************************************************************
//     デバッグ情報クラスの定義
//*****************************************************************************
class CDebugProc
{
public:    // 誰でもアクセス可能
	CDebugProc();
	~CDebugProc();
	void Init(void);
	void Uninit(void);
	void Update(void);
	static void Print(char *fmt, ...);
	static void Draw(void);

	static void SetDisp(bool bDisp);
	static bool GetDisp(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	static LPD3DXFONT	m_pFont;		         // フォントへのポインタ
	static char         m_aStr[MAX_DEBUG_LOG];   // 文字列格納用変数
	static bool         m_bDisp;                 // 描画するかしないか
};

#endif