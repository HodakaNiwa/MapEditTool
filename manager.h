//*****************************************************************************
//
//     マネージャーの処理[manager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    前方宣言(実体がない宣言)
//*****************************************************************************
class CRenderer;
class CInputKeyboard;
class CInputMouse;
class CInputJoyStick;
class CXInput;
class CLight;
class CCamera;
class CDebugProc;
class CEditer;

//*****************************************************************************
//    マネージャクラスの定義
//*****************************************************************************
class CManager
{
public:    // 誰でもアクセス可能
	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance,HWND hWnd,bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CRenderer *GetRenderer(void);
	static CLight *GetLight(void);
	static CCamera *GetCamera(void);
	static CInputKeyboard *GetKeyboard(void);
	static CInputMouse *GetMouse(void);
	static CInputJoyStick *GetJoyStick(void);
	static CXInput *GetXInput(void);
	static CEditer *GetEditer(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	static CRenderer         *m_pRenderer;  // レンダリングクラス型のポインタ
	static CLight            *m_pLight;     // ライトクラス型のポインタ
	static CCamera           *m_pCamera;    // カメラクラス型のポインタ
	static CInputKeyboard    *m_pKeyboard;  // キーボードクラス型のポインタ
	static CInputMouse       *m_pMouse;     // マウスクラス型のポインタ
	static CInputJoyStick    *m_pJoyStick;  // ジョイスティッククラス型のポインタ
	static CXInput           *m_pXInput;    // XInputクラス型のポインタ
	static CDebugProc        *m_pDebugProc; // デバッグ情報クラス型のポインタ
	static CEditer           *m_pEditer;    // エディタークラス型のポインタ
};

//*****************************************************************************
//    プロトタイプ宣言
//*****************************************************************************
bool GetUpdate(void);

#endif